#include "ESP32-OnvifServer.h"

// Constructor with default values
ONVIFServer::ONVIFServer()
  : name("Onvif_Server"),
    manufacturer("XenoioneX"),
    model("esp32s3"),
    version("1.0"),
    serial("0123456789"),
    hardware("ESP32"),
    location("Home"),
    timezone("CET-1CEST,M3.5.0,M10.5.0/3"),
    enableEvents(true),
    enablePTZ(true),
    enableAudio(true),
    enableTwoWayAudio(true),
    blockedIPCount(0), // Initialize to 0
    onvifHandle(NULL),
    onvifBuffer(NULL),
    helloTimer(NULL),
    deviceUUID("00000000-0000-0000-0000-000000000000"),
    ipAddress("0.0.0.0"),
    sock(-1)
{
  
}

template<typename... Args>
void ONVIFServer::setBlockedIPs(Args... ips) {
  blockedIPCount = 0; // Reset count
  setBlockedIPsHelper(ips...);
}

// Explicit instantiation of the template method
template void ONVIFServer::setBlockedIPs<const char*, const char*, const char*>(const char*, const char*, const char*);

ONVIFServer::~ONVIFServer() {
  stopOnvif();
}

esp_err_t ONVIFServer::onvifHandler(httpd_req_t *req) {
  uint8_t buf[100] = {0};
  esp_err_t ret = ESP_OK; // Initialize ret
  uint8_t content[1500] = {0};

  // Handle snapshot request
  if (strcmp(req->uri, "/onvif/snapshot") == 0) {
    camera_fb_t * fb = esp_camera_fb_get();
    if (!fb) {
      LOG_ERR("Camera capture failed");
      httpd_resp_send_500(req);
      return ESP_FAIL;
    }

    httpd_resp_set_type(req, "image/jpeg");
    httpd_resp_send(req, (const char *)fb->buf, fb->len);
    esp_camera_fb_return(fb);
    return ESP_OK;
  }

  int received = httpd_req_recv(req, (char *)content, sizeof(content) - 1);
  if (received <= 0) {
    if (ret == HTTPD_SOCK_ERR_TIMEOUT) {
      httpd_resp_send_408(req);
    } else {
      httpd_resp_send_err(req, HTTPD_500_INTERNAL_SERVER_ERROR, "Failed to receive request body");
    }
    return ESP_FAIL;
  }
  content[received] = '\0'; // Null-terminate the received content

  // Extract action from XML body
  char* body_start = strstr((char*)content, ":Body");
  if (body_start) {
    // Move to end of <*:Body ...>
    body_start = strchr(body_start, '>');
    if (body_start) {
      body_start++; // Move past '>'
      char* action_start = strstr(body_start, "<");
      if (action_start) {
        action_start++;
        char* action_end = strchr(action_start, ' ');
        if (!action_end) {
          action_end = strchr(action_start, '>');
        }
        if (action_end) {
          size_t action_len = action_end - action_start;
          strncpy((char *)buf, action_start, action_len);
          buf[action_len] = '\0';
        }
      }
    }
  }

  // Extract URI to determine the specific service being requested
  const char* uri = req->uri;

  // Generate response using the action in buffer and the specific service URI
  // Pass the full request body as third parameter
  onvifServiceResponse((char*)buf, uri, (char*)content);
  
  if (strcmp((char*)onvifBuffer, "UNKNOWN") == 0) {
    LOG_ERR("Unknown ONVIF Action: %s at URI: %s", buf, uri);
    httpd_resp_send_404(req);
    return ESP_FAIL;
  }

  // Set the correct Content-Type header
  httpd_resp_set_type(req, "application/soap+xml; charset=utf-8");
  httpd_resp_send(req, (char*)onvifBuffer, HTTPD_RESP_USE_STRLEN);

  return ESP_OK;
}

void ONVIFServer::setup_onvif_server(httpd_handle_t server) {
  // Handler for HTTP_POST method
  httpd_uri_t onvifPostUri = {
    .uri = "/onvif/*",
    .method = HTTP_POST,
    .handler = [](httpd_req_t *req) -> esp_err_t {
      ONVIFServer* server = reinterpret_cast<ONVIFServer*>(req->user_ctx);
      return server->onvifHandler(req);
    },
    .user_ctx = this
  };

  httpd_register_uri_handler(server, &onvifPostUri);

  // Handler for HTTP_GET method
  httpd_uri_t onvifGetUri = {
    .uri = "/onvif/*",
    .method = HTTP_GET,
    .handler = [](httpd_req_t *req) -> esp_err_t {
      ONVIFServer* server = reinterpret_cast<ONVIFServer*>(req->user_ctx);
      return server->onvifHandler(req);
    },
    .user_ctx = this
  };

  httpd_register_uri_handler(server, &onvifGetUri);
}

void ONVIFServer::startOnvif() {
  strncpy(ipAddress, WiFi.localIP().toString().c_str(), sizeof(ipAddress));
  generateDeviceUUID(deviceUUID, sizeof(deviceUUID));

  if (psramFound()) {
    onvifBuffer = (uint8_t*)ps_malloc(ONVIF_BUFFER_SIZE);
    if (onvifBuffer == NULL) {
      LOG_ERR("ONVIF Buffer allocation failed!");
    }
  } else {
    LOG_ERR("ONVIF Buffer allocation failed! PSRAM not found");
  }

  if (onvifBuffer == NULL) {
    onvifBuffer = (uint8_t*)malloc(ONVIF_BUFFER_SIZE);
    if (onvifBuffer == NULL) {
      LOG_ERR("ONVIF Buffer allocation failed in regular RAM!");
    }
  }

  sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if (sock < 0) {
    LOG_ERR("UDP Socket creation failed: errno %d", errno);
  }

  memset(&addr, 0, sizeof(addr));
  addr.sin_family = AF_INET;
  addr.sin_port = htons(ONVIF_PORT);
  addr.sin_addr.s_addr = inet_addr(ONVIF_IP);

  if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    LOG_ERR("UDP Socket binding failed: errno %d", errno);
    close(sock);
  }

  if (!setNonBlocking(sock)) {
    LOG_ERR("Failed to set socket to non-blocking mode");
    close(sock);
  }

  struct ip_mreq mreq;
  mreq.imr_multiaddr.s_addr = inet_addr(ONVIF_IP);
  mreq.imr_interface.s_addr = htonl(INADDR_ANY);
  if (setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0) {
    LOG_ERR("Joining multicast group failed: errno %d", errno);
    close(sock);
  }

  // configTime(0, 0, "pool.ntp.org"); // Set the NTP server
  // setenv("TZ", timezone, 1); // Set the timezone
  // tzset(); // Apply the timezone

  esp_timer_create_args_t timerArgs = {
    .callback = &ONVIFServer::sendHelloWrapper,
    .arg = this,
    .name = "helloTimer"
  };
  esp_timer_create(&timerArgs, &helloTimer);
  esp_timer_start_periodic(helloTimer, ONVIF_HELLO_INTERVAL * 1000000);

  xTaskCreate([](void *pvParameters) { 
    ONVIFServer* server = reinterpret_cast<ONVIFServer*>(pvParameters); 
    server->onvifTask(pvParameters); 
  }, "onvifTask", ONVIF_STACK_SIZE, this, ONVIF_PRI, &onvifHandle);

  LOG_INF("ONVIF Started");
  LOG_DBG("Listening on IP: %s, Port: %d", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
}

void ONVIFServer::stopOnvif() {
  // Delete onvif task
  if (onvifHandle != NULL) {
    vTaskDelete(onvifHandle);
    onvifHandle = NULL;
  }
  // Remove the onvif hello timer
  if (helloTimer != NULL) {
    esp_timer_stop(helloTimer);
    esp_timer_delete(helloTimer);
    helloTimer = NULL;
  }
  // Close udp port
  if (sock >= 0) {
    sendMessage("Bye"); // Send Bye message before closing the socket
    close(sock);
    sock = -1;
  }
  // Free the buffer
  if (onvifBuffer != NULL) {
    free(onvifBuffer);
    onvifBuffer = NULL;
  }
}

bool ONVIFServer::isBlocked(const char* ip) {
  for (size_t i = 0; i < blockedIPCount; ++i) {
    if (strcmp(ip, blockedIPs[i]) == 0) {
      return true;
    }
  }
  return false;
}

bool ONVIFServer::setNonBlocking(int sock) {
  int flags = fcntl(sock, F_GETFL, 0);
  if (flags == -1) {
    LOG_ERR("Failed to get socket flags");
    return false;
  }
  if (fcntl(sock, F_SETFL, flags | O_NONBLOCK) == -1) {
    LOG_ERR("Failed to set socket to non-blocking mode");
    return false;
  }
  LOG_INF("Socket set to non-blocking mode");
  return true;
}

void ONVIFServer::onvifTask(void *pvParameters) {
  struct sockaddr_in client_addr;
  char recv_buffer[2048];
  int32_t n;

  while (1) {
    // Handle incoming UDP packets
    socklen_t addr_len = sizeof(client_addr);
    n = recvfrom(sock, recv_buffer, sizeof(recv_buffer) - 1, 0, (struct sockaddr *)&client_addr, &addr_len);
    if (n < 0) {
      if (errno != EWOULDBLOCK) {
        LOG_ERR("UDP Receive failed: errno %d", errno);
      }
    } else {
      char sender_ip[INET_ADDRSTRLEN];
      inet_ntop(AF_INET, &(client_addr.sin_addr), sender_ip, INET_ADDRSTRLEN);
      if (!isBlocked(sender_ip)) {
        recv_buffer[n] = '\0';
        process_packet(recv_buffer, n, &client_addr, sock);
      }
    }
  }

  stopOnvif(); // Clean up when exiting the task
  vTaskDelete(NULL);
}
