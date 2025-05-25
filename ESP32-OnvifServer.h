#ifndef ESP32_ONVIF_SERVER_H
#define ESP32_ONVIF_SERVER_H

#include <Arduino.h>
#include <WiFi.h>
#include <lwip/sockets.h>
#include <esp_camera.h>
#include <esp_http_server.h>

#define ONVIF_PORT 3702
#define ONVIF_IP "239.255.255.250"
#define ONVIF_HELLO_INTERVAL 30
#define ONVIF_BUFFER_SIZE (1024 * 8)
#define ONVIF_PARTS_BUFFER_SIZE (1024 * 3)
#define ONVIF_STACK_SIZE (1024 * 8)
#define ONVIF_PRI 1
#define MAX_ALLOWED_CONTENT_SIZE 4096 // 4 KB
//#define MAX_ALLOWED_CONTENT_SIZE 8192 // 8 KB


// Definitions for logging
#define LOG_ERR(format, ...) ESP_LOGE("ONVIF", format, ##__VA_ARGS__)
#define LOG_INF(format, ...) ESP_LOGI("ONVIF", format, ##__VA_ARGS__)
#define LOG_DBG(format, ...) ESP_LOGD("ONVIF", format, ##__VA_ARGS__)

class ONVIFServer {
public:
  ONVIFServer();
  ~ONVIFServer();

  // Public member variables
  char name[32];
  char manufacturer[16];
  char model[16];
  char version[6];
  char serial[32];
  char hardware[32];
  char location[32];
  char timezone[32];
  bool enableEvents;
  bool enablePTZ;
  bool enableAudio;
  bool enableTwoWayAudio;
  
  char blockedIPs[10][16]; // Increase size to allow more IPs

  esp_err_t onvifHandler(httpd_req_t *req);
  void setup_onvif_server(httpd_handle_t server);
  void startOnvif();
  void stopOnvif();

  template<typename... Args>
  void setBlockedIPs(Args... ips); // Renamed method
  // Resolution struct
  struct Resolution {
    int width;
    int height;
  };

private:
  size_t blockedIPCount;
  void extractMessageID(const char* packetData, char* messageID, size_t messageIDSize);
  void generateUUID(char* uuid, size_t uuidSize);
  void generateDeviceUUID(char* uuid, size_t uuidSize);
  char* getAction(const char* requestBody);
  void populateOnvifResponse(const char* mainHeader, const char* templateStr, ...);
  std::vector<char*> allocatedBuffers;
  char* populateSection(const char* format, ...);
  void buildOnvifResponse(const std::vector<const char*>& parts);
  template<typename... section>
  void buildOnvifResponse(const char* mainHeader, section... args);
  void sendMatch(const char* messageID, const char* relatesToID, const char* action);
  void sendMessage(const char* messageType);
  void parseSystemDateAndTime(const char* requestBody);
  void parseNetworkProtocols(const char* requestBody);
  void parseAndApplySettings(const char* requestBody);
  void parseAndSetVideoEncoderConfiguration(const char* requestBody);
  void onvifServiceResponse(const char* uri, const char* requestBody = nullptr);
  bool isBlocked(const char* ip);
  void process_packet(const char *packet_data, size_t len, const struct sockaddr_in *sender_addr, int sock);
  bool setNonBlocking(int sock);
  void onvifTask(void *pvParameters);
  void sendHello();
  static void sendHelloWrapper(void* arg);

  std::vector<Resolution> getCameraResolutions(camera_pid_t pid);
  const char* getResolutions();

  void setNetworkProtocol(const char* protocolName, bool enabled, int port);

  void handleDeviceService(const char* action, const char* requestBody);
  void handleMediaService(const char* action, const char* requestBody);
  void handleMedia2Service(const char* action, const char* requestBody);
  void handleImagingService(const char* action, const char* requestBody);
  void handlePTZService(const char* action, const char* requestBody);
  void handleEventService(const char* action, const char* requestBody);

  TaskHandle_t onvifHandle;
  uint8_t* onvifBuffer;
  uint8_t* onvifPartBuffer;
  esp_timer_handle_t helloTimer;
  char deviceUUID[37];
  char ipAddress[16];
  int sock;
  struct sockaddr_in addr;

  template<typename T>
  void setBlockedIPsHelper(T ip) {
    if (blockedIPCount < 10) {
      strncpy(blockedIPs[blockedIPCount], ip, sizeof(blockedIPs[blockedIPCount]));
      blockedIPCount++;
    }
  }

  template<typename T, typename... Args>
  void setBlockedIPsHelper(T ip, Args... ips) {
    setBlockedIPsHelper(ip);
    setBlockedIPsHelper(ips...);
  }
};

#endif // ESP32_ONVIF_SERVER_H
