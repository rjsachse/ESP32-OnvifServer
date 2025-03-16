#include "ESP32-OnvifServer.h"
//#include "lwip/apps/sntp.h" // Include SNTP header
#include "esp_sntp.h"
#include "service_headers.h"
#include "discovery_service.h"
#include "device_service.h"
#include "media_service.h"
#include "media2_service.h"
#include "events_service.h"
#include "imaging_service.h"
#include "ptz_service.h"

// const char* ONVIFServer::getResolutions() {
//   sensor_t *s = esp_camera_sensor_get();
//   if (!s) {
//     LOG_ERR("Camera sensor not detected");
//     return nullptr;
//   }
//   std::vector<Resolution> resolutions = getCameraResolutions(static_cast<camera_pid_t>(s->id.PID));

//   static char resolutionsXml[2560] = "";
//   char resolutionEntry[110];

//   resolutionsXml[0] = '\0'; // Clear the string

//   for (const auto& res : resolutions) {
//     snprintf(resolutionEntry, sizeof(resolutionEntry), 
//       "<tt:ResolutionsAvailable>"
//       "<tt:Width>%d</tt:Width>"
//       "<tt:Height>%d</tt:Height>"
//       "</tt:ResolutionsAvailable>", 
//       res.width, res.height);
//     strncat(resolutionsXml, resolutionEntry, sizeof(resolutionsXml) - strlen(resolutionsXml) - 1);
//   }
//   return resolutionsXml;
// }

const char* ONVIFServer::getResolutions() {
  sensor_t *s = esp_camera_sensor_get();
  if (!s) {
      LOG_ERR("Camera sensor not detected");
      return nullptr;
  }

  // Get the sensor information
  camera_sensor_info_t* info = esp_camera_sensor_get_info(&s->id);
  if (!info) {
      LOG_ERR("Failed to get sensor information");
      return nullptr;
  }

  // Get the resolutions
  std::vector<resolution_info_t> resolutions;
  framesize_t max_size = info->max_size;
  for (int fs = 0; fs <= max_size; ++fs) {
      resolutions.push_back(resolution[fs]);
  }

  // Generate XML string
  static char resolutionsXml[2560] = "";
  char resolutionEntry[110];

  resolutionsXml[0] = '\0'; // Clear the string

  for (const auto& res : resolutions) {
      snprintf(resolutionEntry, sizeof(resolutionEntry), 
          "<tt:ResolutionsAvailable>"
          "<tt:Width>%d</tt:Width>"
          "<tt:Height>%d</tt:Height>"
          "</tt:ResolutionsAvailable>", 
          res.width, res.height);
      strncat(resolutionsXml, resolutionEntry, sizeof(resolutionsXml) - strlen(resolutionsXml) - 1);
  }
  return resolutionsXml;
}

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

char* skipNamespace(const char* tag) {
    //LOG_INF("[ONVIF] Skipping namespace for tag: %s", tag);

    const char* colon = strchr(tag, ':');
    const char* space = strchr(tag, ' ');
    const char* slash = strchr(tag, '/');

    // Determine the nearest invalid character after the colon
    const char* boundary = tag + strlen(tag); // Default to the end of the string
    if (space && space < boundary) {
        boundary = space;
    }
    if (slash && slash < boundary) {
        boundary = slash;
    }

    // Check if the colon is valid (i.e., before the boundary)
    if (colon && colon < boundary) {
        //LOG_INF("[ONVIF] Namespace found, skipping to: %s", colon + 1);
        return (char*)(colon + 1); // Skip past the colon
    }

    //LOG_INF("[ONVIF] No namespace found, using tag: %s", tag);
    return (char*)tag; // No namespace, return the original tag
}



char* findBodyStart(const char* requestBody) {
  //LOG_INF("Finding <Body> tag in request body.");

  char* body_start = strstr((char*)requestBody, ":Body");
  if (body_start) {
    body_start = strchr(body_start, '>'); // Find '>'
    if (body_start) {
      //LOG_INF("Found <Body> tag, moving to content at: %s", body_start + 1);
      return body_start + 1; // Move past '>'
    }
  }
  
  //LOG_INF("No <Body> tag found.");
  return NULL; // Body tag not found
}

char* extractAction(char* body_start) {
  //LOG_INF("[ONVIF] Extracting action from body content: %s", body_start);

  // Locate the opening of the action tag
  char* action_start = strstr(body_start, "<");
  if (action_start) {
    action_start++; // Move past '<'
    //LOG_INF("[ONVIF] Found action start: %s", action_start);

    // Locate the closing angle bracket ('>') to isolate just the main tag
    char* tag_end = strchr(action_start, '>');
    if (tag_end) {
      *tag_end = '\0'; // Temporarily null-terminate the tag for processing
    } else {
      //LOG_INF("[ONVIF] No closing bracket found for action tag.");
      return NULL;
    }

    // Skip namespace prefix, if present
    action_start = skipNamespace(action_start);

    if (tag_end) {
      *tag_end = '>'; // Restore the original string after processing
    }

    // Ensure the tag ends correctly (no nested content included)
    char* action_end = strpbrk(action_start, " />"); // Stop at space, '/', or end
    if (action_end) {
      size_t action_len = action_end - action_start;
      char* action = (char*)malloc(action_len + 1);
      if (action) {
        strncpy(action, action_start, action_len);
        action[action_len] = '\0'; // Null-terminate the action name
        //LOG_INF("[ONVIF] Successfully extracted action: %s", action);
        return action;
      } else {
        LOG_INF("[ONVIF] Memory allocation failed for action.");
      }
    } else {
      LOG_INF("[ONVIF] No valid end of action tag found.");
    }
  } else {
    LOG_INF("[ONVIF] No action tag found in body.");
  }

  return NULL; // Action tag not found or invalid
}




char* ONVIFServer::getAction(const char* requestBody) {
  //LOG_INF("Starting action extraction for request body.");

  // Step 1: Find the start of the <Body> section
  char* body_start = findBodyStart(requestBody);
  if (!body_start) {
    LOG_INF("Failed to find <Body> section.");
    return NULL; // No <Body> tag found
  }

  // Step 2: Extract the action name from the <Body> section
  char* action = extractAction(body_start);
  if (!action) {
    LOG_INF("Failed to extract action from body content.");
  }

  return action;
}


// char* ONVIFServer::getAction(const char* requestBody) {
//   char* action = NULL;

//   // Locate the <*:Body> tag
//   char* body_start = strstr((char*)requestBody, ":Body");
//   if (body_start) {
//     body_start = strchr(body_start, '>'); // Move to the end of <*:Body>
//     if (body_start) {
//       body_start++; // Move past '>'

//       // Locate the opening tag of the action
//       char* action_start = strstr(body_start, "<");
//       if (action_start) {
//         action_start++; // Move past '<'

//         // Skip namespace prefix (e.g., "tds:")
//         char* colon = strchr(action_start, ':');
//         if (colon && colon < strchr(action_start, ' ') && colon < strchr(action_start, '>')) {
//           action_start = colon + 1; // Move past the colon
//         }

//         // Find the end of the action tag name
//         char* action_end = strpbrk(action_start, " />"); // Stop at space, '/', or '>'
//         if (action_end) {
//           size_t action_len = action_end - action_start;
//           action = (char*)malloc(action_len + 1);
//           if (action) {
//             strncpy(action, action_start, action_len);
//             action[action_len] = '\0'; // Null-terminate the action string
//           }
//         }
//       }
//     }
//   }

//   return action;
// }

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

char* ONVIFServer::populateSection(const char* format, ...) {
  char* buffer = (char*)malloc(ONVIF_PARTS_BUFFER_SIZE);
  if (!buffer) {
    LOG_ERR("Failed to allocate memory for section buffer");
    return NULL;
  }
  va_list args;
  va_start(args, format);
  vsnprintf(buffer, ONVIF_PARTS_BUFFER_SIZE, format, args);
  va_end(args);
  allocatedBuffers.push_back(buffer); // Add buffer to the list
  return buffer;
}


void ONVIFServer::buildOnvifResponse(const std::vector<const char*>& parts) {
  if (onvifBuffer == NULL) {
    LOG_ERR("ONVIF Buffer not allocated! Unable to create response.");
    return;
  }

  // Initialize onvifBuffer with the first part
  snprintf((char*)onvifBuffer, ONVIF_BUFFER_SIZE, "%s", parts[0]);

  for (size_t i = 1; i < parts.size(); ++i) {
    if (parts[i] != NULL) {
      strncat((char*)onvifBuffer, parts[i], ONVIF_BUFFER_SIZE - strlen((char*)onvifBuffer) - 1);
    }
  }

  strncat((char*)onvifBuffer, footer, ONVIF_BUFFER_SIZE - strlen((char*)onvifBuffer) - 1);

  // Free all allocated buffers
  for (char* buffer : allocatedBuffers) {
    free(buffer);
  }
  allocatedBuffers.clear(); // Clear the list
}

template<typename... section>
void ONVIFServer::buildOnvifResponse(const char* mainHeader, section... args) {
  std::vector<const char*> parts = { mainHeader, args... };
  buildOnvifResponse(parts);
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
  buildOnvifResponse(discoverNS, populateSection(onvifMatch, messageID, relatesToID, fullAction,
                        (strcmp(action, "probe") == 0 ? "ProbeMatches" : "ResolveMatches"),
                        (strcmp(action, "probe") == 0 ? "ProbeMatch" : "ResolveMatch"),
                        deviceUUID, location, name, hardware, ipAddress,
                        (strcmp(action, "probe") == 0 ? "ProbeMatch" : "ResolveMatch"),
                        (strcmp(action, "probe") == 0 ? "ProbeMatches" : "ResolveMatches"))
                        );
}

void ONVIFServer::sendMessage(const char* messageType) {
  char messageID[37];
  generateUUID(messageID, sizeof(messageID));
  buildOnvifResponse(discoverNS, populateSection(messageType, messageID, deviceUUID, location, name, hardware, ipAddress));
  //populateOnvifResponse(discoverNS, messageType, messageID, deviceUUID, location, name, hardware, ipAddress);
  sendto(sock, (char*)onvifBuffer, strlen((char*)onvifBuffer), 0, (const struct sockaddr*)&addr, sizeof(addr));
}

bool extractValue(const char* xml, const char* tag, int& value) {
  char openTag[128];
  char closeTag[128];

  // Construct the full opening and closing tags
  snprintf(openTag, sizeof(openTag), "<%s", tag);
  snprintf(closeTag, sizeof(closeTag), "</%s>", tag);

  const char* start = strstr(xml, openTag);
  if (start) {
    // Move the pointer past the opening tag and any attributes
    start = strchr(start, '>') + 1;
    const char* end = strstr(start, closeTag);
    if (end) {
      char buffer[16];
      strncpy(buffer, start, end - start);
      buffer[end - start] = '\0';
      value = atoi(buffer);
      return true;
    }
  }
  return false;
}


bool extractStringValue(const char* xml, const char* tag, char* value, size_t valueSize) {
  const char* start = strstr(xml, tag);
  if (start) {
    start = strchr(start, '>') + 1;
    const char* end = strchr(start, '<');
    if (end) {
      size_t len = end - start;
      if (len >= valueSize) {
        LOG_ERR("Value is too long: %.*s", (int)len, start);
        return false;
      }
      strncpy(value, start, len);
      value[len] = '\0';
      return true;
    }
  }
  return false;
}

void ONVIFServer::parseSystemDateAndTime(const char* requestBody) {
  char dateTimeType[16], daylightSavings[6], timeZone[64];
  int hour, minute, second, year, month, day;

  // Extract and log DateTimeType
  if (!extractStringValue(requestBody, "DateTimeType", dateTimeType, sizeof(dateTimeType))) {
    LOG_ERR("Failed to extract DateTimeType");
    return;
  }
  LOG_INF("Extracted DateTimeType: %s", dateTimeType);

  // Extract and log DaylightSavings
  if (!extractStringValue(requestBody, "DaylightSavings", daylightSavings, sizeof(daylightSavings))) {
    LOG_ERR("Failed to extract DaylightSavings");
    return;
  }
  LOG_INF("Extracted DaylightSavings: %s", daylightSavings);

  // Extract and log TimeZone
  if (!extractStringValue(requestBody, "TZ", timeZone, sizeof(timeZone))) {
    LOG_ERR("Failed to extract TimeZone");
    return;
  }
  LOG_INF("Extracted TimeZone: %s", timeZone);

  // Extract and log time values
  if (!extractValue(requestBody, "Hour", hour) ||
      !extractValue(requestBody, "Minute", minute) ||
      !extractValue(requestBody, "Second", second) ||
      !extractValue(requestBody, "Year", year) ||
      !extractValue(requestBody, "Month", month) ||
      !extractValue(requestBody, "Day", day)) {
    LOG_ERR("Failed to extract date/time values");
    return;
  }
  LOG_INF("Extracted Time: %04d-%02d-%02d %02d:%02d:%02d", year, month, day, hour, minute, second);

  // Apply the extracted settings
  struct tm timeinfo;
  timeinfo.tm_year = year - 1900;
  timeinfo.tm_mon = month - 1;
  timeinfo.tm_mday = day;
  timeinfo.tm_hour = hour;
  timeinfo.tm_min = minute;
  timeinfo.tm_sec = second;

  // Log timeinfo struct values
  LOG_INF("tm structure - Year: %d, Month: %d, Day: %d, Hour: %d, Minute: %d, Second: %d",
          timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday, timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);

  time_t t = mktime(&timeinfo);
  struct timeval now = { .tv_sec = t };
  settimeofday(&now, NULL);

  // Set the timezone
  setenv("TZ", timeZone, 1);
  tzset();

  LOG_INF("System date and time set to: %04d-%02d-%02d %02d:%02d:%02d, TimeZone: %s, DaylightSavings: %s",
          year, month, day, hour, minute, second, timeZone, daylightSavings);
}

void ONVIFServer::parseNetworkProtocols(const char* requestBody) {
  char protocolName[16], enabledStr[6];
  int port;

  const char* protocols[] = {"HTTP", "HTTPS", "RTSP"};

  for (const char* protocol : protocols) {
    // Extract and log Protocol Name
    if (!extractStringValue(requestBody, protocol, protocolName, sizeof(protocolName))) {
      LOG_ERR("Failed to extract Protocol Name for %s", protocol);
      return;
    }
    LOG_INF("Extracted Protocol Name: %s", protocolName);

    // Extract and log Enabled status
    if (!extractStringValue(requestBody, "Enabled", enabledStr, sizeof(enabledStr))) {
      LOG_ERR("Failed to extract Enabled status for %s", protocol);
      return;
    }
    LOG_INF("Extracted Enabled status for %s: %s", protocol, enabledStr);

    // Extract and log Port
    if (!extractValue(requestBody, "Port", port)) {
      LOG_ERR("Failed to extract Port for %s", protocol);
      return;
    }
    LOG_INF("Extracted Port for %s: %d", protocol, port);

    // Apply the extracted settings
    bool enabled = strcmp(enabledStr, "true") == 0;
    setNetworkProtocol(protocolName, enabled, port);
  }
}

void ONVIFServer::setNetworkProtocol(const char* protocolName, bool enabled, int port) {
  // Set the protocol settings on the ESP32
  LOG_INF("Setting Protocol: %s, Enabled: %s, Port: %d", protocolName, enabled ? "true" : "false", port);
  
  // Implement protocol-specific logic here
  // For example, setting the HTTP server port, enabling/disabling HTTPS, etc.
}

void ONVIFServer::parseAndApplySettings(const char* requestBody) {
  LOG_INF("Received request body: %s", requestBody);

  sensor_t* s = esp_camera_sensor_get();
  if (!s) {
    LOG_ERR("Failed to get camera sensor");
    return;
  }

  int value;
  if (extractValue(requestBody, "Brightness", value)) s->set_brightness(s, value);
  if (extractValue(requestBody, "Contrast", value)) s->set_contrast(s, value);
  if (extractValue(requestBody, "ColorSaturation", value)) s->set_saturation(s, value);
  if (extractValue(requestBody, "Sharpness", value)) s->set_sharpness(s, value);

  // Exposure settings
  if (strstr(requestBody, "Exposure")) {
    if (strstr(requestBody, "Mode>AUTO")) s->set_exposure_ctrl(s, 1);
    else if (strstr(requestBody, "Mode>MANUAL")) s->set_exposure_ctrl(s, 0);

    if (extractValue(requestBody, "Gain", value)) s->set_agc_gain(s, value);
    if (extractValue(requestBody, "ExposureTime", value)) s->set_aec_value(s, value);
  }

  // White Balance settings
  if (strstr(requestBody, "WhiteBalance")) {
    if (strstr(requestBody, "Mode>AUTO")) s->set_whitebal(s, 1);
    else if (strstr(requestBody, "Mode>MANUAL")) s->set_whitebal(s, 0);
  }
}

void ONVIFServer::parseAndSetVideoEncoderConfiguration(const char* requestBody) {
  int width, height, quality, frameRateLimit;

  // Extract and log Resolution
  if (!extractValue(requestBody, "Width", width) || !extractValue(requestBody, "Height", height)) {
    LOG_ERR("Failed to extract Resolution");
  }
  LOG_INF("Extracted Resolution: %dx%d", width, height);

  // Extract and log Quality
  if (!extractValue(requestBody, "Quality", quality)) {
    LOG_ERR("Failed to extract Quality");
  }
  LOG_INF("Extracted Quality: %d", quality);

  // Extract and log FrameRateLimit
  if (!extractValue(requestBody, "FrameRateLimit", frameRateLimit)) {
    LOG_ERR("Failed to extract FrameRateLimit");
  }
  LOG_INF("Extracted FrameRateLimit: %d", frameRateLimit);

  // Apply the extracted settings
  // Implement the logic to set the video encoder configuration on the device
}

void ONVIFServer::handleDeviceService(const char* action, const char* requestBody) {
  if (strstr(action, "GetDeviceInformation")) {
    buildOnvifResponse(maxHeader, populateSection(getDeviceInfo, manufacturer, model, version, serial, hardware));
  } else if (strstr(action, "GetCapabilities")) {
    buildOnvifResponse(maxHeader, getCapabilitiesStart,
      populateSection(getCapabilitiesDevice, ipAddress),
      enableEvents ? populateSection(getCapabilitiesEvents, ipAddress) : NULL,
      populateSection(getCapabilitiesImaging, ipAddress),
      populateSection(getCapabilitiesMedia, ipAddress),
      populateSection(getCapabilitiesExension, ipAddress, enableAudio ? "1" : "0", enableTwoWayAudio ? "1" : "0"),
      enablePTZ ? populateSection(getCapabilitiesPTZ, ipAddress) : NULL,
      getCapabilitiesEnd
    );
  } else if (strstr(action, "GetServices")) {
    buildOnvifResponse(maxHeader, getServicesStart,
      populateSection(getServicesDevice, ipAddress),
      enableEvents ? populateSection(getServicesEvent, ipAddress) : NULL,
      enableTwoWayAudio ? populateSection(getServicesMedia2, ipAddress) : NULL,
      populateSection(getServicesMedia, ipAddress),
      enablePTZ ? populateSection(getServicesPTZ, ipAddress) : NULL,
      populateSection(getServicesImage, ipAddress),
      getServicesEnd
    );
  } else if (strstr(action, "GetScopes")) {
    buildOnvifResponse(maxHeader, getScopesStart,
      enableAudio ? getScopesAudio : NULL,
      getScopesVideo,
      enablePTZ ? getScopesPTZ : NULL,
      getScopesProS,
      enableTwoWayAudio ? getScopesProT : NULL,
      populateSection(getScopesInfo, location, name, hardware),
      getScopesEnd
    );
  } else if (strstr(action, "GetZeroConfiguration")) {
    buildOnvifResponse(maxHeader, populateSection(getZeroConfig, ipAddress));
  } else if (strstr(action, "GetNetworkInterfaces")) {
    char macAddress[18];
    snprintf(macAddress, sizeof(macAddress), "%s", WiFi.macAddress().c_str());
    buildOnvifResponse(maxHeader, populateSection(getNetworkInterfaces, macAddress, ipAddress));
  } else if (strstr(action, "GetNTP")) {
    buildOnvifResponse(maxHeader, populateSection(getNTP));
  } else if (strstr(action, "GetHostname")) {
    buildOnvifResponse(maxHeader, populateSection(getHostname));
  } else if (strstr(action, "GetNetworkDefaultGateway")) {
    buildOnvifResponse(maxHeader, populateSection(getNetworkDefaultGateway));
  } else if (strstr(action, "GetNetworkProtocols")) {
    buildOnvifResponse(maxHeader, populateSection(getNetworkProtocols));
  } else if (strstr(action, "GetDiscoveryMode")) {
    buildOnvifResponse(maxHeader, populateSection(getDiscoveryMode));
  } else if (strstr(action, "GetDNS")) {
    char gatewayIP[16];
    snprintf(gatewayIP, sizeof(gatewayIP), "%u.%u.%u.%u", 
             WiFi.gatewayIP()[0], WiFi.gatewayIP()[1], 
             WiFi.gatewayIP()[2], WiFi.gatewayIP()[3]);
    buildOnvifResponse(maxHeader, populateSection(getDNS, gatewayIP));
  } else if (strstr(action, "GetSystemDateAndTime")) {
    time_t now;
    struct tm timeinfo;
    time(&now);
    gmtime_r(&now, &timeinfo); // Use gmtime_r for UTC time

    const char* dateTimeType = (sntp_get_sync_status() == SNTP_SYNC_STATUS_COMPLETED) ? "NTP" : "Manual";
    const char* daylightSavings = timeinfo.tm_isdst > 0 ? "true" : "false";

    buildOnvifResponse(maxHeader, populateSection(getSystemDateAndTime,
                          dateTimeType, daylightSavings, timezone,
                          timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec,
                          timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday));
  } else if (strstr(action, "SetSystemDateAndTime")) {
    parseSystemDateAndTime(requestBody);
    buildOnvifResponse(maxHeader, setSystemDateAndTime);
  } else if (strstr(action, "SetNetworkProtocols")) {
    parseNetworkProtocols(requestBody);
    buildOnvifResponse(maxHeader, setNetworkProtocols);
  } else if (strstr(action, "GetEndpointReference")) {
    buildOnvifResponse(maxHeader, populateSection(getEndpointReference, deviceUUID));
  } else {
    snprintf((char*)onvifBuffer, ONVIF_BUFFER_SIZE, "UNKNOWN");
  }
}

void ONVIFServer::handleMediaService(const char* action, const char* requestBody) {
  if (strcmp(action, "GetProfile") == 0) {
    populateOnvifResponse(maxHeader, getProfiles, "Profile", "Profile", "Profile", "Profile");
  } else if (strcmp(action, "GetProfiles") == 0) {
    populateOnvifResponse(maxHeader, getProfiles, "Profiles", "Profiles", "Profiles", "Profiles");
  } else if (strstr(action, "GetStreamUri")) {
    populateOnvifResponse(maxHeader, getStreamUri, ipAddress);
  } else if (strstr(action, "GetSnapshotUri")) {
    populateOnvifResponse(maxHeader, getSnapshotUri, ipAddress);
  } else if (strstr(action, "GetVideoSources")) {
    populateOnvifResponse(maxHeader, getVideoSources);
  } else if (strstr(action, "GetVideoSourceConfiguration")) {
    populateOnvifResponse(maxHeader, getVideoSourceConfiguration);
  } else if (strstr(action, "GetVideoEncoderConfigurationOptions")) {
    buildOnvifResponse(maxHeader, populateSection(getVideoEncoderConfigurationOptions, getResolutions()));
  } else if (strstr(action, "GetAudioDecoderConfigurations")) {
    populateOnvifResponse(maxHeader, getAudioDecoderConfigurations);
  } else if (strstr(action, "GetAudioSources")) {
    populateOnvifResponse(maxHeader, getAudioSources);
  } else if (strstr(action, "GetAudioOutputConfiguration")) {
    populateOnvifResponse(maxHeader, getAudioOutputConfiguration);
  } else if (strstr(action, "GetAudioOutputConfigurationOptions")) {
    populateOnvifResponse(maxHeader, getAudioOutputConfigurationOptions);
  } else if (strstr(action, "GetAudioOutputConfigurations")) {
    populateOnvifResponse(maxHeader, getAudioOutputConfigurations);
  } else if (strstr(action, "SetVideoEncoderConfiguration")) {
    parseAndSetVideoEncoderConfiguration(requestBody);
    populateOnvifResponse(maxHeader, setVideoEncoderConfiguration);
  } else {
    snprintf((char*)onvifBuffer, ONVIF_BUFFER_SIZE, "UNKNOWN");
  }
}

void ONVIFServer::handleMedia2Service(const char* action, const char* requestBody) {
  if (strcmp(action, "GetProfiles") == 0) {
    populateOnvifResponse(maxHeader, getProfilesMedia2);
  } else if (strstr(action, "GetStreamUri")) {
    populateOnvifResponse(maxHeader, getStreamUri2, ipAddress);
  } else if (strstr(action, "GetSnapshotUri")) {
    populateOnvifResponse(maxHeader, getSnapshotUri2, ipAddress);
  } else if (strstr(action, "GetVideoSourceConfigurations")) {
    populateOnvifResponse(maxHeader, getVideoSourceConfigurations2);
  } else if (strstr(action, "GetVideoEncoderConfigurations")) {
    populateOnvifResponse(maxHeader, getVideoEncoderConfigurations2);
  } else if (strstr(action, "GetAudioDecoderConfigurations")) {
    populateOnvifResponse(maxHeader, getAudioDecoderConfigurations2);
  } else if (strstr(action, "GetAudioEncoderConfigurations")) {
    populateOnvifResponse(maxHeader, getAudioEncoderConfigurations2);
  } else if (strstr(action, "GetAudioDecoderConfigurationOptions")) {
    populateOnvifResponse(maxHeader, getAudioDecoderConfigurationOptions2);
  } else if (strstr(action, "GetAudioEncoderConfigurationOptions")) {
    populateOnvifResponse(maxHeader, getAudioEncoderConfigurationOptions2);
  } else if (strstr(action, "GetAudioOutputConfigurations")) {
    populateOnvifResponse(maxHeader, getAudioOutputConfigurations2);
  } else if (strstr(action, "GetAudioOutputConfigurationOptions")) {
    populateOnvifResponse(maxHeader, getAudioOutputConfigurationOptions2);
  } else if (strstr(action, "GetAudioInputConfigurations")) {
    populateOnvifResponse(maxHeader, getAudioInputConfigurations2);
  } else if (strstr(action, "SetAudioInputConfiguration")) {
    // Implement logic to set audio input configuration based on requestBody
    populateOnvifResponse(maxHeader, setAudioInputConfiguration2);
  } else if (strstr(action, "GetAudioSourceConfigurations")) {
    populateOnvifResponse(maxHeader, getAudioSourceConfigurations2);
  } else {
    snprintf((char*)onvifBuffer, ONVIF_BUFFER_SIZE, "UNKNOWN");
  }
}

void ONVIFServer::handleImagingService(const char* action, const char* requestBody) {
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
}

void ONVIFServer::handlePTZService(const char* action, const char* requestBody) {
  if (strstr(action, "GetNodes")) {
    populateOnvifResponse(ptzHeader, getNodes);
  } else if (strstr(action, "GetConfigurations")) {
    populateOnvifResponse(ptzHeader, getConfigurations);
  } else {
    snprintf((char*)onvifBuffer, ONVIF_BUFFER_SIZE, "UNKNOWN");
  }
}

void ONVIFServer::handleEventService(const char* action, const char* requestBody) {
  char currentTime[32], terminationTime[32], utcTime[32];
  time_t now = time(NULL);
  struct tm timeinfo;
  gmtime_r(&now, &timeinfo);
  strftime(currentTime, sizeof(currentTime), "%Y-%m-%dT%H:%M:%SZ", &timeinfo);
  timeinfo.tm_min += 1;
  mktime(&timeinfo);
  strftime(terminationTime, sizeof(terminationTime), "%Y-%m-%dT%H:%M:%SZ", &timeinfo);
  strftime(utcTime, sizeof(utcTime), "%Y-%m-%dT%H:%M:%SZ", &timeinfo);

  char messageID[42];
  generateUUID(messageID, sizeof(messageID));

  if (strstr(action, "GetEventProperties")) {
    populateOnvifResponse(maxHeader, getEventProperties, messageID);
  } else if (strstr(action, "CreatePullPointSubscription")) {
    populateOnvifResponse(maxHeader, createPullPointSubscription, messageID, ipAddress, currentTime, terminationTime);
  } else if (strstr(action, "PullMessages")) {
    populateOnvifResponse(maxHeader, pullMessages, currentTime, terminationTime, utcTime, utcTime);
  } else if (strstr(action, "Unsubscribe")) {
    populateOnvifResponse(maxHeader, unsubscribe, messageID);
  } else if (strstr(action, "Renew")) {
    populateOnvifResponse(maxHeader, renewSubscription, messageID, terminationTime, currentTime);
  } else {
    snprintf((char*)onvifBuffer, ONVIF_BUFFER_SIZE, "UNKNOWN");
  }
}

void ONVIFServer::onvifServiceResponse(const char* uri, const char* requestBody) {
  // Extract action from requestBody
  char* action = getAction(requestBody);

  // Safely log the action and URI
  if (action && uri) {
    LOG_INF("Onvif request: %s at URI: %s", action, uri);
  } else if (uri) {
    LOG_INF("Onvif request: [No Action] at URI: %s", uri);
  } else {
    LOG_INF("Onvif request: [No Action or URI]");
  }

  if (action) {
    if (strstr(uri, "/device_service")) {
      handleDeviceService(action, requestBody);
    } else if (strstr(uri, "/media_service")) {
      handleMediaService(action, requestBody);
    } else if (strstr(uri, "/media2_service")) {
      handleMedia2Service(action, requestBody);
    } else if (strstr(uri, "/image_service")) {
      handleImagingService(action, requestBody);
    } else if (strstr(uri, "/ptz_service")) {
      handlePTZService(action, requestBody);
    } else if (strstr(uri, "/event_service")) {
      handleEventService(action, requestBody);
    } else {
      snprintf((char*)onvifBuffer, ONVIF_BUFFER_SIZE, "UNKNOWN");
    }
    free(action); // Free the allocated memory for action
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
  sendMessage(onvifHello);
}
