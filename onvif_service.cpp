#include "ESP32-OnvifServer.h"

#include "service_headers.h"
#include "discovery_service.h"
#include "device_service.h"
#include "media_service.h"
#include "events_service.h"
#include "imaging_service.h"
#include "ptz_service.h"

void ONVIFServer::extractMessageID(const char* packetData, char* messageID, size_t messageIDSize) {
  const char* startTag = "MessageID";
  const char* startPtr = strstr(packetData, startTag);
  if (!startPtr) {
    LOG_ERR("MessageID tag not found");
    messageID[0] = '\0';
    return;
  }
  startPtr = strchr(startPtr, '>') + 1;
  while (*startPtr == ' ' || *startPtr == '\n' || *startPtr == '\r') {
    startPtr++;
  }
  const char* endPtr = strstr(startPtr, "</");
  if (!endPtr) {
    LOG_ERR("MessageID end tag not found");
    messageID[0] = '\0';
    return;
  }

  // Check for prefixes and remove them from startPtr
  const char urnPrefix[] = "urn:";
  const char uuidPrefix[] = "uuid:";
  
  if (strncmp(startPtr, urnPrefix, strlen(urnPrefix)) == 0) {
    startPtr += strlen(urnPrefix);
  }
  
  if (strncmp(startPtr, uuidPrefix, strlen(uuidPrefix)) == 0) {
    startPtr += strlen(uuidPrefix);
  }
  
  size_t len = endPtr - startPtr;
  if (len >= messageIDSize) {
    LOG_ERR("MessageID is too long: %.*s", (int)len, startPtr);
    messageID[0] = '\0';
    return;
  }
  
  // Copy the cleaned message ID into the buffer
  strncpy(messageID, startPtr, len);
  messageID[len] = '\0';
}



void ONVIFServer::generateUUID(char* uuid, size_t uuidSize) {
  snprintf(uuid, uuidSize, "%08lx-%04lx-%04lx-%04lx-%012llx",
           (unsigned long)esp_random(), esp_random() & 0xFFFF,
           (esp_random() & 0x0FFF) | 0x4000,
           (esp_random() & 0x3FFF) | 0x8000,
           ((unsigned long long)esp_random() << 32) | esp_random());
}

void ONVIFServer::generateDeviceUUID(char* uuid, size_t uuidSize) {
  uint32_t chipId = ESP.getEfuseMac(); // Get the chip ID
  snprintf(uuid, uuidSize, "%08lx-%04lx-%04lx-%04lx-%012lx",
           (unsigned long)((chipId >> 16) & 0xFFFF),
           (unsigned long)(chipId & 0xFFFF),
           (unsigned long)((chipId >> 16) & 0xFFFF),
           (unsigned long)(chipId & 0xFFFF),
           (unsigned long)(chipId));
}

void ONVIFServer::populateOnvifResponse(const char* mainHeader, const char* templateStr, ...) {
  if (onvifBuffer == NULL) {
    LOG_ERR("ONVIF Buffer not allocated! Unable to create response, starting Onvif.");
    return;
  }

  snprintf((char*)onvifBuffer, ONVIF_BUFFER_SIZE, "%s", mainHeader);
  va_list args;
  va_start(args, templateStr);
  vsnprintf((char*)onvifBuffer + strlen((char*)onvifBuffer), ONVIF_BUFFER_SIZE - strlen((char*)onvifBuffer), templateStr, args);
  va_end(args);

  strncat((char*)onvifBuffer, footer, ONVIF_BUFFER_SIZE - strlen((char*)onvifBuffer) - 1);
}

void ONVIFServer::sendMatch(const char* messageID, const char* relatesToID, const char* action) {
  const char* fullAction;
  if (strcmp(action, "probe") == 0) {
    fullAction = "http://schemas.xmlsoap.org/ws/2005/04/discovery/ProbeMatches";
  } else if (strcmp(action, "resolve") == 0) {
    fullAction = "http://schemas.xmlsoap.org/ws/2005/04/discovery/ResolveMatches";
  } else {
    LOG_ERR("Wrong action passed!");
    return;
  }
  populateOnvifResponse(discoverNS, onvifMatch, messageID, relatesToID, fullAction,
                        (strcmp(action, "probe") == 0 ? "ProbeMatches" : "ResolveMatches"),
                        (strcmp(action, "probe") == 0 ? "ProbeMatch" : "ResolveMatch"),
                        deviceUUID, APP_NAME, CAM_BOARD, ipAddress,
                        (strcmp(action, "probe") == 0 ? "ProbeMatch" : "ResolveMatch"),
                        (strcmp(action, "probe") == 0 ? "ProbeMatches" : "ResolveMatches"));
}

void ONVIFServer::sendMessage(const char* messageType) {
  char messageID[37];
  generateUUID(messageID, sizeof(messageID));
  populateOnvifResponse(discoverNS, messageType, messageID, deviceUUID, APP_NAME, CAM_BOARD, ipAddress);
  sendto(sock, (char*)onvifBuffer, strlen((char*)onvifBuffer), 0, (const struct sockaddr*)&addr, sizeof(addr));
}

void ONVIFServer::parseAndApplySettings(const char* requestBody) {
  LOG_INF("Received request body: %s", requestBody);

  auto extractValue = [](const char* xml, const char* tag) -> int {
    const char* start = strstr(xml, tag);
    if (start) {
      start = strchr(start, '>') + 1;
      const char* end = strchr(start, '<');
      if (end) {
        char value[16];
        strncpy(value, start, end - start);
        value[end - start] = '\0';
        return atoi(value);
      }
    }
    return -1;
  };

  sensor_t* s = esp_camera_sensor_get();
  if (!s) {
    LOG_ERR("Failed to get camera sensor");
    return;
  }
  int brightness = extractValue(requestBody, "Brightness");
  if (brightness != -1) {
    if (s->set_brightness(s, brightness) == 0) {
      LOG_INF("Brightness set to %d", brightness);
    } else {
      LOG_ERR("Failed to set Brightness to %d", brightness);
    }
  }

  int contrast = extractValue(requestBody, "Contrast");
  if (contrast != -1) {
    if (s->set_contrast(s, contrast) == 0) {
      LOG_INF("Contrast set to %d", contrast);
    } else {
      LOG_ERR("Failed to set Contrast to %d", contrast);
    }
  }

  int saturation = extractValue(requestBody, "ColorSaturation");
  if (saturation != -1) {
    if (s->set_saturation(s, saturation) == 0) {
      LOG_INF("Color Saturation set to %d", saturation);
    } else {
      LOG_ERR("Failed to set Color Saturation to %d", saturation);
    }
  }

  int sharpness = extractValue(requestBody, "Sharpness");
  if (sharpness != -1) {
    if (s->set_sharpness(s, sharpness) == 0) {
      LOG_INF("Sharpness set to %d", sharpness);
    } else {
      LOG_ERR("Failed to set Sharpness to %d", sharpness);
    }
  }

  // Exposure settings
  if (strstr(requestBody, "Exposure")) {
    if (strstr(requestBody, "Mode>AUTO")) {
      if (s->set_exposure_ctrl(s, 1) == 0) {
        LOG_INF("Exposure set to AUTO");
      } else {
        LOG_ERR("Failed to set Exposure to AUTO");
      }
    } else if (strstr(requestBody, "Mode>MANUAL")) {
      if (s->set_exposure_ctrl(s, 0) == 0) {
        LOG_INF("Exposure set to MANUAL");
      } else {
        LOG_ERR("Failed to set Exposure to MANUAL");
      }
    }

    int gain = extractValue(requestBody, "Gain");
    if (gain != -1) {
      if (s->set_agc_gain(s, gain) == 0) {
        LOG_INF("Gain set to %d", gain);
      } else {
        LOG_ERR("Failed to set Gain to %d", gain);
      }
    }

    int exposureTime = extractValue(requestBody, "ExposureTime");
    if (exposureTime != -1) {
      if (s->set_aec_value(s, exposureTime) == 0) {
        LOG_INF("Exposure Time set to %d", exposureTime);
      } else {
        LOG_ERR("Failed to set Exposure Time to %d", exposureTime);
      }
    }
  }

  // White Balance settings
  if (strstr(requestBody, "WhiteBalance")) {
    if (strstr(requestBody, "Mode>AUTO")) {
      if (s->set_whitebal(s, 1) == 0) {
        LOG_INF("White Balance set to AUTO");
      } else {
        LOG_ERR("Failed to set White Balance to AUTO");
      }
    } else if (strstr(requestBody, "Mode>MANUAL")) {
      if (s->set_whitebal(s, 0) == 0) {
        LOG_INF("White Balance set to MANUAL");
      } else {
        LOG_ERR("Failed to set White Balance to MANUAL");
      }
    }
  }
}

void ONVIFServer::onvifServiceResponse(const char* action, const char* uri, const char* requestBody) {
  LOG_INF("Onvif request: %s at URI: %s", action, uri);

  if (strstr(uri, "/device_service")) {
    // Device services
    if (strstr(action, "GetDeviceInformation")) {
      populateOnvifResponse(deviceHeader, getDeviceInfo, onvifManufacturer, onvifModel, APP_VER, "123456", "HW123456");
    } else if (strstr(action, "GetCapabilities")) {
      populateOnvifResponse(maxHeader, getCapabilities, ipAddress, ipAddress, ipAddress, ipAddress, ipAddress, ipAddress, ipAddress, ipAddress, ipAddress, ipAddress, ipAddress);
    } else if (strstr(action, "GetServices")) {
      populateOnvifResponse(deviceHeader, getServices, ipAddress, ipAddress, ipAddress, ipAddress, ipAddress, ipAddress, ipAddress, ipAddress, ipAddress, ipAddress, ipAddress, ipAddress, ipAddress, ipAddress, ipAddress, ipAddress, ipAddress, ipAddress, ipAddress, ipAddress);
    } else if (strstr(action, "GetScopes")) {
      populateOnvifResponse(deviceHeader, getScopes);
    } else if (strstr(action, "GetZeroConfiguration")) {
      populateOnvifResponse(deviceHeader, getZeroConfig, ipAddress);
    } else if (strstr(action, "GetNetworkInterfaces")) {
      populateOnvifResponse(deviceHeader, getNetworkInterfaces, "24:62:AB:D5:4C:18", ipAddress);
    } else if (strstr(action, "GetDNS")) {
      populateOnvifResponse(deviceHeader, getDNS);
    } else if (strstr(action, "GetSystemDateAndTime")) {
      populateOnvifResponse(deviceHeader, getSystemDateAndTime);
    } else {
      snprintf((char*)onvifBuffer, ONVIF_BUFFER_SIZE, "UNKNOWN");
    }
  } else if (strstr(uri, "/media_service") || strstr(uri, "/media2_service")) { // Add media2_service handling
    // Media services
    if (strcmp(action, "GetProfile") == 0) {
      populateOnvifResponse(mediaHeader, getProfiles, "Profile", "Profile", "Profile", "Profile");
    } else if (strcmp(action, "GetProfiles") == 0) {
      if (strstr(uri, "/media2_service")) {
        // Handle Media2 version response
        populateOnvifResponse(mediaHeader, getProfilesMedia2);
      } else {
        // Handle Media1 version response
        populateOnvifResponse(mediaHeader, getProfiles, "Profiles", "Profiles", "Profiles", "Profiles");
      }
    } else if (strstr(action, "GetStreamUri")) {
      populateOnvifResponse(mediaHeader, getStreamUri, ipAddress);
    } else if (strstr(action, "GetSnapshotUri")) {
      populateOnvifResponse(mediaHeader, getSnapshotUri, ipAddress);
    } else if (strstr(action, "GetVideoSources")) {
      populateOnvifResponse(maxHeader, getVideoSources);
    } else if (strstr(action, "GetVideoSourceConfiguration")) {
      populateOnvifResponse(maxHeader, getVideoSourceConfiguration);
    } else if (strstr(action, "GetVideoEncoderConfigurationOptions")) {
      populateOnvifResponse(maxHeader, getVideoEncoderConfigurationOptions);
    } else if (strstr(action, "GetAudioDecoderConfigurations")) {
      populateOnvifResponse(mediaHeader, getAudioDecoderConfigurations);
    } else {
      snprintf((char*)onvifBuffer, ONVIF_BUFFER_SIZE, "UNKNOWN");
    }
  } else if (strstr(uri, "/image_service")) {
    // Imaging services
    if (strstr(action, "GetImagingSettings")) {
      sensor_t* s = esp_camera_sensor_get();
      if (!s) {
        LOG_ERR("Failed to get camera sensor");
        return;
      }

      int brightness = s->status.brightness;
      int contrast = s->status.contrast;
      int colorSaturation = s->status.saturation;
      int sharpness = s->status.sharpness;
      const char* exposureMode = s->status.aec ? "AUTO" : "MANUAL";
      int minExposureTime = s->status.aec2;
      int maxExposureTime = s->status.aec_value;
      int gain = s->status.agc_gain;
      const char* whiteBalanceMode = s->status.awb ? "AUTO" : "MANUAL";

      populateOnvifResponse(maxHeader, getImagingSettings,
                            brightness, contrast, colorSaturation, sharpness,
                            exposureMode, minExposureTime, maxExposureTime, gain,
                            whiteBalanceMode);
    } else if (strstr(action, "GetOptions")) {
      populateOnvifResponse(maxHeader, getImagingOptions);
    } else if (strstr(action, "GetMoveOptions")) {
      populateOnvifResponse(maxHeader, getMoveOptions);
    } else if (strstr(action, "SetImagingSettings")) {
      if (requestBody) parseAndApplySettings(requestBody);
      populateOnvifResponse(maxHeader, setImagingSettings);
    } else {
      snprintf((char*)onvifBuffer, ONVIF_BUFFER_SIZE, "UNKNOWN");
    }
  } else {
    snprintf((char*)onvifBuffer, ONVIF_BUFFER_SIZE, "UNKNOWN");
  }
}

void ONVIFServer::process_packet(const char *packet_data, size_t len, const struct sockaddr_in *sender_addr, int sock) {
  if (onvifBuffer == NULL) {
    LOG_ERR("ONVIF Buffer not allocated!");
    return;
  }
  char messageID[42];
  char relatesToID[37];
  generateUUID(messageID, sizeof(messageID));
  extractMessageID(packet_data, relatesToID, sizeof(relatesToID));
  // Check for Probe
  if (strstr(packet_data, "Probe") != NULL) {
    sendMatch(messageID, relatesToID, "probe");
  }
  // Check for Resolve
  else if (strstr(packet_data, "Resolve") != NULL) {
    sendMatch(messageID, relatesToID, "resolve");
  }
  // Send response
  if (strlen((char*)onvifBuffer) <= 4096) { // have to check max size again was 1444
    sendto(sock, (char*)onvifBuffer, strlen((char*)onvifBuffer), 0, (struct sockaddr *)sender_addr, sizeof(*sender_addr));
  } else {
    LOG_ERR("UDP Failed to send response packet too large");
  }
}

void ONVIFServer::sendHelloWrapper(void* arg) {
  ONVIFServer* server = static_cast<ONVIFServer*>(arg);
  server->sendHello();
}

void ONVIFServer::sendHello() {
  sendMessage("Hello");
}
