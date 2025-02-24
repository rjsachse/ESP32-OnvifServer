#include "ESP32-OnvifServer.h"
//#include "lwip/apps/sntp.h" // Include SNTP header
#include "esp_sntp.h"
#include "service_headers.h"
#include "discovery_service.h"
#include "device_service.h"
#include "media_service.h"
#include "events_service.h"
#include "imaging_service.h"
#include "ptz_service.h"


//#include <vector>

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

char* ONVIFServer::populateSection(const char* format, ...) {
  va_list args;
  va_start(args, format);
  vsnprintf((char*)onvifPartBuffer, ONVIF_PARTS_BUFFER_SIZE, format, args);
  va_end(args);
  return (char*)onvifPartBuffer;
}


void ONVIFServer::buildOnvifResponse(const std::vector<const char*>& parts) {
  if (onvifBuffer == NULL) {
    LOG_ERR("ONVIF Buffer not allocated! Unable to create response.");
    return;
  }

  // Initialize onvifBuffer with the first part
  snprintf((char*)onvifBuffer, ONVIF_BUFFER_SIZE, "%s", parts[0]);

  for (size_t i = 1; i < parts.size(); ++i) {
    strncat((char*)onvifBuffer, parts[i], ONVIF_BUFFER_SIZE - strlen((char*)onvifBuffer) - 1);
  }

  strncat((char*)onvifBuffer, footer, ONVIF_BUFFER_SIZE - strlen((char*)onvifBuffer) - 1);

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
//  populateOnvifResponse(discoverNS, onvifMatch, messageID, relatesToID, fullAction,
//                        (strcmp(action, "probe") == 0 ? "ProbeMatches" : "ResolveMatches"),
//                        (strcmp(action, "probe") == 0 ? "ProbeMatch" : "ResolveMatch"),
//                        deviceUUID, location, name, hardware, ipAddress,
//                        (strcmp(action, "probe") == 0 ? "ProbeMatch" : "ResolveMatch"),
//                        (strcmp(action, "probe") == 0 ? "ProbeMatches" : "ResolveMatches"));
}

void ONVIFServer::sendMessage(const char* messageType) {
  char messageID[37];
  generateUUID(messageID, sizeof(messageID));
  buildOnvifResponse(discoverNS, populateSection(messageType, messageID, deviceUUID, location, name, hardware, ipAddress));
  //populateOnvifResponse(discoverNS, messageType, messageID, deviceUUID, location, name, hardware, ipAddress);
  sendto(sock, (char*)onvifBuffer, strlen((char*)onvifBuffer), 0, (const struct sockaddr*)&addr, sizeof(addr));
}

bool extractValue(const char* xml, const char* tag, int& value) {
  const char* start = strstr(xml, tag);
  if (start) {
    start = strchr(start, '>') + 1;
    const char* end = strchr(start, '<');
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


void ONVIFServer::onvifServiceResponse(const char* action, const char* uri, const char* requestBody) {
  LOG_INF("Onvif request: %s at URI: %s", action, uri);

  char messageID[37];
  generateUUID(messageID, sizeof(messageID));

  if (strstr(uri, "/device_service")) {
    // Device services
    if (strstr(action, "GetDeviceInformation")) {
      //populateOnvifResponse(maxHeader, getDeviceInfo, manufacturer, model, version, serial, hardware);
      buildOnvifResponse(maxHeader, populateSection(getDeviceInfo, manufacturer, model, version, serial, hardware));
    } else if (strstr(action, "GetCapabilities")) {
      buildOnvifResponse(maxHeader, getCapabilitiesStart,
        populateSection(getCapabilitiesDevice, ipAddress),
        enableEvents ? populateSection(getCapabilitiesEvents, ipAddress) : "",
        populateSection(getCapabilitiesImaging, ipAddress),
        populateSection(getCapabilitiesMedia, ipAddress),
        populateSection(getCapabilitiesExension, ipAddress, enableAudio ? "1" : "0", enableTwoWayAudio ? "1" : "0"),
        enablePTZ ? populateSection(getCapabilitiesPTZ, ipAddress) : "",
        getCapabilitiesEnd
      );

    } else if (strstr(action, "GetServices")) {
      buildOnvifResponse(maxHeader, getServicesStart,
        populateSection(getServicesDevice, ipAddress),
        enableEvents ? populateSection(getServicesEvent, ipAddress) : "",
        enableTwoWayAudio ? populateSection(getServicesMedia2, ipAddress) : "",
        populateSection(getServicesMedia, ipAddress),
        enablePTZ ? populateSection(getServicesPTZ, ipAddress) : "",
        populateSection(getServicesImage, ipAddress),
        getServicesEnd
      );
      //populateOnvifResponse(maxHeader, getServices, ipAddress, ipAddress, ipAddress, ipAddress, ipAddress, ipAddress, ipAddress, ipAddress, ipAddress, ipAddress, ipAddress, ipAddress, ipAddress, ipAddress, ipAddress, ipAddress, ipAddress, ipAddress, ipAddress, ipAddress);
    } else if (strstr(action, "GetScopes")) {
      populateOnvifResponse(maxHeader, getScopes, location, name, hardware);
    } else if (strstr(action, "GetZeroConfiguration")) {
      populateOnvifResponse(maxHeader, getZeroConfig, ipAddress);
    } else if (strstr(action, "GetNetworkInterfaces")) {
      char macAddress[18];
      snprintf(macAddress, sizeof(macAddress), "%s", WiFi.macAddress().c_str());
      populateOnvifResponse(maxHeader, getNetworkInterfaces, macAddress, ipAddress);
    } else if (strstr(action, "GetDNS")) {
      char gatewayIP[16];
      snprintf(gatewayIP, sizeof(gatewayIP), "%u.%u.%u.%u", 
               WiFi.gatewayIP()[0], WiFi.gatewayIP()[1], 
               WiFi.gatewayIP()[2], WiFi.gatewayIP()[3]);
      populateOnvifResponse(deviceHeader, getDNS, gatewayIP);
    } else if (strstr(action, "GetSystemDateAndTime")) {
      time_t now;
      struct tm timeinfo;
      time(&now);
      localtime_r(&now, &timeinfo);

      const char* dateTimeType = (sntp_get_sync_status() == SNTP_SYNC_STATUS_COMPLETED) ? "NTP" : "Manual";
      const char* daylightSavings = timeinfo.tm_isdst > 0 ? "true" : "false";

      populateOnvifResponse(maxHeader, getSystemDateAndTime,
                            dateTimeType, daylightSavings, timezone,
                            timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec,
                            timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday);
    } else if (strstr(action, "GetEndpointReference")) {
      populateOnvifResponse(maxHeader, getEndpointReference, deviceUUID);
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
    } else if (strstr(action, "GetAudioSources")) {
      populateOnvifResponse(mediaHeader, getAudioSources);
    } else if (strstr(action, "GetAudioOutputConfiguration")) {
      populateOnvifResponse(mediaHeader, getAudioOutputConfiguration);
    } else if (strstr(action, "GetAudioOutputConfigurationOptions")) {
      populateOnvifResponse(mediaHeader, getAudioOutputConfigurationOptions);
    } else if (strstr(action, "GetAudioOutputConfigurations")) {
      populateOnvifResponse(mediaHeader, getAudioOutputConfigurations);
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
    
  } else if (strstr(uri, "/ptz_service")) {
    // PTZ services
    if (strstr(action, "GetNodes")) {
      populateOnvifResponse(ptzHeader, getNodes);
    } else if (strstr(action, "GetConfigurations")) {
      populateOnvifResponse(ptzHeader, getConfigurations);
    } else {
      snprintf((char*)onvifBuffer, ONVIF_BUFFER_SIZE, "UNKNOWN");
    }

  } else if (strstr(uri, "/event_service")) {
    // Event services
    char currentTime[32], terminationTime[32], utcTime[32];
    // Generate current, termination, and UTC times
    time_t now = time(NULL);
    struct tm timeinfo;
    gmtime_r(&now, &timeinfo);
    strftime(currentTime, sizeof(currentTime), "%Y-%m-%dT%H:%M:%SZ", &timeinfo);
    timeinfo.tm_min += 1; // Add 1 minute for termination time
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
      populateOnvifResponse(maxHeader, pullMessages, messageID, currentTime, terminationTime, utcTime, utcTime);
    } else if (strstr(action, "Unsubscribe")) {
      populateOnvifResponse(maxHeader, unsubscribe, messageID);
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
  sendMessage(onvifHello);
}
