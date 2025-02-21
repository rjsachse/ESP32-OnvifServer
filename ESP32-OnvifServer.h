#ifndef ESP32_ONVIF_SERVER_H
#define ESP32_ONVIF_SERVER_H

#include <Arduino.h>
#include <WiFi.h>
#include <lwip/sockets.h>
#include <esp_camera.h>
#include <esp_http_server.h>

#define ONVIF_PORT 3702
#define ONVIF_IP "239.255.255.250"
#define APP_NAME "Onvif Server"
#define APP_VER "1.0.0"
#define CAM_BOARD "ESP32"
#define ONVIF_HELLO_INTERVAL 30
#define ONVIF_BUFFER_SIZE (1024 * 8)
#define ONVIF_STACK_SIZE (1024 * 8)
#define ONVIF_PRI 1

// Definitions for logging
#define LOG_ERR(format, ...) ESP_LOGE("ONVIF", format, ##__VA_ARGS__)
#define LOG_INF(format, ...) ESP_LOGI("ONVIF", format, ##__VA_ARGS__)
#define LOG_DBG(format, ...) ESP_LOGD("ONVIF", format, ##__VA_ARGS__)

class ONVIFServer {
public:
  ONVIFServer();
  ~ONVIFServer();

  // Public member variables
  char onvifManufacturer[16];
  char onvifModel[16];
  char apMac[18];
  char staMac[18];
  char blockedIPs[2][16];
  size_t blockedIPCount;

  esp_err_t onvifHandler(httpd_req_t *req);
  void setup_onvif_server(httpd_handle_t server);
  void startOnvif();
  void stopOnvif();

private:
  void extractMessageID(const char* packetData, char* messageID, size_t messageIDSize);
  void generateUUID(char* uuid, size_t uuidSize);
  void generateDeviceUUID(char* uuid, size_t uuidSize);
  void populateOnvifResponse(const char* mainHeader, const char* templateStr, ...);
  void sendMatch(const char* messageID, const char* relatesToID, const char* action);
  void sendMessage(const char* messageType);
  void parseAndApplySettings(const char* requestBody);
  void onvifServiceResponse(const char* action, const char* uri, const char* requestBody = nullptr);
  bool isBlocked(const char* ip);
  void process_packet(const char *packet_data, size_t len, const struct sockaddr_in *sender_addr, int sock);
  bool setNonBlocking(int sock);
  void onvifTask(void *pvParameters);
  void sendHello();
  static void sendHelloWrapper(void* arg);

  TaskHandle_t onvifHandle;
  uint8_t* onvifBuffer;
  esp_timer_handle_t helloTimer;
  char deviceUUID[37];
  char ipAddress[16];
  int sock;
  struct sockaddr_in addr;
};

#endif // ESP32_ONVIF_SERVER_H
