#include <WiFi.h>
#include <ESP32-RTSPServer.h>
#include "ESP32-OnvifServer.h"
#include "esp_camera.h"

// ===================
// Select camera model
// ===================
//#define CAMERA_MODEL_WROVER_KIT // Has PSRAM
//#define CAMERA_MODEL_ESP_EYE  // Has PSRAM
//#define CAMERA_MODEL_ESP32S3_EYE // Has PSRAM
//#define CAMERA_MODEL_M5STACK_PSRAM // Has PSRAM
//#define CAMERA_MODEL_M5STACK_V2_PSRAM // M5Camera version B Has PSRAM
//#define CAMERA_MODEL_M5STACK_WIDE // Has PSRAM
//#define CAMERA_MODEL_M5STACK_ESP32CAM // No PSRAM
//#define CAMERA_MODEL_M5STACK_UNITCAM // No PSRAM
//#define CAMERA_MODEL_M5STACK_CAMS3_UNIT  // Has PSRAM
#define CAMERA_MODEL_AI_THINKER // Has PSRAM
//#define CAMERA_MODEL_TTGO_T_JOURNAL // No PSRAM
//#define CAMERA_MODEL_XIAO_ESP32S3 // Has PSRAM
//#define CAMERA_MODEL_XENOIONEX // Has PSRAM Custom Board
// ** Espressif Internal Boards **
//#define CAMERA_MODEL_ESP32_CAM_BOARD
//#define CAMERA_MODEL_ESP32S2_CAM_BOARD
//#define CAMERA_MODEL_ESP32S3_CAM_LCD
//#define CAMERA_MODEL_DFRobot_FireBeetle2_ESP32S3 // Has PSRAM
//#define CAMERA_MODEL_DFRobot_Romeo_ESP32S3 // Has PSRAM
#include "camera_pins.h"

// ===========================
// Enter your WiFi credentials
// ===========================
const char *ssid = "WiFi-E0C99C";
const char *password = "11444305";
  // const char *ssid = "PrettyFlyForWifi";
  // const char *password = "Mynetwork13!";

// RTSPServer instance
RTSPServer rtspServer;

// Create an instance of ONVIFServer
ONVIFServer onvifServer;

// Can set a username and password for RTSP authentication or leave blank for no authentication
const char *rtspUser = "";
const char *rtspPassword = "";

// Define HAVE_AUDIO to include audio-related code
//#define HAVE_AUDIO // Comment out if don't have audio

#ifdef HAVE_AUDIO
#include <ESP_I2S.h>
// I2SClass object for I2S communication
I2SClass I2S;

#ifndef I2S_SCK
#define I2S_SCK 4  // Serial Clock (SCK) or Bit Clock (BCLK)
#endif

#ifndef I2S_WS
#define I2S_WS 5  // Word Select (WS) or Left Right Clock (LRCLK)
#endif

#ifndef I2S_SDI
#define I2S_SDI 6  // Serial Data In (Mic)
#endif

#ifndef I2S_SDO
#define I2S_SDO 7  // Serial Data Out (Amp)
#endif


// Audio variables
//int sampleRate = 16000;      // Sample rate in Hz
// int sampleRate = 8000;      // Sample rate in Hz
// const size_t sampleBytes = 1024; // Sample buffer size (in bytes)
// int16_t* sampleBuffer = NULL;  // Pointer to the sample buffer

// Audio parameters
static const int sampleRate = 8000;          // 8 kHz for G.711
static const size_t sampleCount = 160;       // 20 ms at 8 kHz
static const size_t sampleBytes = sampleCount * sizeof(int16_t);  // 320 bytes
static const size_t g711Bytes = sampleCount; // 160 bytes
static int16_t* sampleBuffer = NULL;         // PCM buffer for I2S
static uint8_t* g711Buffer = NULL;           // G.711 buffer for RTSP
#endif


// Variable to hold quality for RTSP frame
int quality;
// Task handles
TaskHandle_t videoTaskHandle = NULL; 
TaskHandle_t audioTaskHandle = NULL; 
TaskHandle_t subtitlesTaskHandle = NULL;


/** 
 * @brief Sets up the camera with the specified configuration. 
*/
void setupCamera() {
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM;
  config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.frame_size = FRAMESIZE_UXGA;
  config.pixel_format = PIXFORMAT_JPEG;  // for streaming
  config.grab_mode = CAMERA_GRAB_LATEST;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 10;
  config.fb_count = 2;

  // if PSRAM IC present, init with UXGA resolution and higher JPEG quality
  // for larger pre-allocated frame buffer.
  if (config.pixel_format == PIXFORMAT_JPEG) {
    if (psramFound()) {
      config.jpeg_quality = 10;
      config.fb_count = 2;
      config.grab_mode = CAMERA_GRAB_LATEST;
    } else {
      // Limit the frame size when PSRAM is not available
      config.frame_size = FRAMESIZE_SVGA;
      config.fb_location = CAMERA_FB_IN_DRAM;
    }
  } else {
    // Best option for face detection/recognition
    config.frame_size = FRAMESIZE_240X240;
#if CONFIG_IDF_TARGET_ESP32S3
    config.fb_count = 2;
#endif
  }

#if defined(CAMERA_MODEL_ESP_EYE)
  pinMode(13, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
#endif

  // Initialize camera
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  sensor_t *s = esp_camera_sensor_get();
  // initial sensors are flipped vertically and colors are a bit saturated
  if (s->id.PID == OV3660_PID) {
    s->set_vflip(s, 1);        // flip it back
    s->set_brightness(s, 1);   // up the brightness just a bit
    s->set_saturation(s, -2);  // lower the saturation
  }
  // drop down frame size for higher initial frame rate
  if (config.pixel_format == PIXFORMAT_JPEG) {
    s->set_framesize(s, FRAMESIZE_QVGA);
  }

#if defined(CAMERA_MODEL_M5STACK_WIDE) || defined(CAMERA_MODEL_M5STACK_ESP32CAM)
  s->set_vflip(s, 1);
  s->set_hmirror(s, 1);
#endif

#if defined(CAMERA_MODEL_ESP32S3_EYE)
  s->set_vflip(s, 1);
#endif
  Serial.println("Camera Setup Complete");
}

/** 
 * @brief Retrieves the current frame quality from the camera. 
*/
void getFrameQuality() { 
  sensor_t * s = esp_camera_sensor_get(); 
  quality = s->status.quality; 
  Serial.printf("Camera Quality is: %d\n", quality);
}

#ifdef HAVE_AUDIO
/** 
 * @brief Sets up the I2S microphone. 
 * 
 * @return true if setup is successful, false otherwise. 
 */
// static bool setupMic() {
//   bool res;
//   // I2S mic and I2S amp can share same I2S channel
//   I2S.setPins(I2S_SCK, I2S_WS, I2S_SDO, I2S_SDI, -1); // BCLK/SCK, LRCLK/WS, SDOUT, SDIN, MCLK
//   res = I2S.begin(I2S_MODE_STD, sampleRate, I2S_DATA_BIT_WIDTH_16BIT, I2S_SLOT_MODE_MONO, I2S_STD_SLOT_LEFT);
//   if (sampleBuffer == NULL) sampleBuffer = (int16_t*)malloc(sampleBytes);
//   return res;
// }

static bool setupMic() {
  bool res;
  // Configure I2S pins
  I2S.setPins(I2S_SCK, I2S_WS, I2S_SDO, I2S_SDI, -1);  // No MCLK

  // Initialize I2S: 8 kHz, 16-bit, mono
  res = I2S.begin(I2S_MODE_STD, sampleRate, I2S_DATA_BIT_WIDTH_16BIT, 
                  I2S_SLOT_MODE_MONO, I2S_STD_SLOT_LEFT);
  if (!res) {
    Serial.println("I2S initialization failed");
    return false;
  }

  // Allocate buffers
#ifdef USE_PSRAM
  if (sampleBuffer == NULL) sampleBuffer = (int16_t*)ps_malloc(sampleBytes);
  if (g711Buffer == NULL) g711Buffer = (uint8_t*)ps_malloc(g711Bytes);
#else
  if (sampleBuffer == NULL) sampleBuffer = (int16_t*)malloc(sampleBytes);
  if (g711Buffer == NULL) g711Buffer = (uint8_t*)malloc(g711Bytes);
#endif

  if (sampleBuffer == NULL || g711Buffer == NULL) {
    Serial.println("Buffer allocation failed");
    return false;
  }

  Serial.printf("Free heap after setup: %u\n", ESP.getFreeHeap());
  return true;
}

/** 
 * @brief Reads audio data from the I2S microphone. 
 * 
 * @return The number of bytes read. 
 */
static size_t micInput() {
  // read esp mic
  size_t bytesRead = 0;
  bytesRead = I2S.readBytes((char*)sampleBuffer, sampleBytes);
  return bytesRead;
}
/**
 * @brief Task to send audio data via RTP. 
 */
void sendAudio(void* pvParameters) {
  Serial.printf("[Audio] Started on Core %d\n", xPortGetCoreID());
  while (true) {
    if (rtspServer.readyToSendAudio()) {
      size_t bytesRead = micInput();
      if (bytesRead) {
        rtspServer.sendRTSPAudio(sampleBuffer, bytesRead);
      }
    }
    vTaskDelay(pdMS_TO_TICKS(1));
  }
}

//Callback function to handle received audio
// void receivedAudio(const uint8_t* l16Data, size_t len) {
//   // Example: Play received audio (implement your playback logic here)
//   Serial.printf("Received audio data of length: %d\n", len);
//   size_t bytesWritten = I2S.write(l16Data, len);
// }
void receivedAudio(const uint8_t* l16Data, size_t len) {
  // size_t numSamples = len / sizeof(int16_t);
  // Serial.printf("Received audio data, Length: %d, Samples: %d\n", len, numSamples);

  // // Log first 4 samples using a for loop
  // const int16_t* pcmData = (const int16_t*)l16Data;
  // size_t samplesToLog = min(numSamples, (size_t)4); // Log up to 4 samples
  // if (samplesToLog > 0) {
  //   Serial.print("Samples: ");
  //   for (size_t i = 0; i < samplesToLog; i++) {
  //     Serial.printf("%d", pcmData[i]);
  //     if (i < samplesToLog - 1) {
  //       Serial.print(", ");
  //     }
  //   }
  //   Serial.println();
  // }

  // Write to I2S
  size_t bytesWritten = I2S.write(l16Data, len);
  if (bytesWritten != len) {
    Serial.printf("I2S write failed: Wrote %d of %d bytes\n", bytesWritten, len);
  }
}
#endif

/** 
 * @brief Task to send jpeg frames via RTP. 
*/
void sendVideo(void* pvParameters) { 
  Serial.printf("[Video] Started on Core %d\n", xPortGetCoreID());
  while (true) { 
    // Send frame via RTP
    if(rtspServer.readyToSendFrame()) {
      camera_fb_t* fb = esp_camera_fb_get();
      rtspServer.sendRTSPFrame(fb->buf, fb->len, quality, fb->width, fb->height);
      esp_camera_fb_return(fb);
    }
    vTaskDelay(pdMS_TO_TICKS(1)); 
  }
}

/**
 * @brief Task to send subtitles via RTP. 
 */
void sendSubtitles(void* pvParameters) {
  char data[100];
  while (true) {
    if(rtspServer.readyToSendSubtitles()) {
      size_t len = snprintf(data, sizeof(data), "FPS: %lu", rtspServer.rtpFps);
      rtspServer.sendRTSPSubtitles(data, len);
    }
  vTaskDelay(1000 / portTICK_PERIOD_MS); // Delay for 1 second has to be 1 second
  }
}

// Timer callback function 
void onSubtitles(void* arg) { 
  char data[100];
  if(rtspServer.readyToSendSubtitles()) {
    size_t len = snprintf(data, sizeof(data), "FPS: %lu", rtspServer.rtpFps);
    rtspServer.sendRTSPSubtitles(data, len);
  }
}

void setup() {
  // Initialize serial communication
  Serial.begin(115200);

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");

  // Setup camera
  setupCamera();
  getFrameQuality();

#ifdef HAVE_AUDIO
  // Setup microphone
  if (setupMic()) {
    Serial.println("Microphone Setup Complete");
    // Create tasks for sending audio
    //xTaskCreate(sendAudio, "Audio", 8192, NULL, 8, &audioTaskHandle);
    xTaskCreatePinnedToCore(sendAudio, "Audio", 8192, NULL, 8, &audioTaskHandle, 1);
  } else {
    Serial.println("Mic Setup Failed!");
  }
    // Pass the callback to the RTSP server for handling received audio
  rtspServer.setAudioReceiveCallback(receivedAudio);

#ifdef USE_SPEEXDSP
  // Configure audio processing settings via audioProcessor
  // Note: These settings can be adjusted dynamically in loop() or elsewhere

  // Acoustic Echo Cancellation (AEC)
  rtspServer.audioProcessor.enableAEC(true); // Enable to remove echo from mic input using speaker output

  // Microphone Preprocessing
  rtspServer.audioProcessor.enableMicNoiseSuppression(false);     // Enable to reduce background noise from mic
  rtspServer.audioProcessor.setMicNoiseSuppressionLevel(-25);    // Set noise reduction strength (-10 to -40 dB, lower = stronger)
  rtspServer.audioProcessor.enableMicAGC(true, 0.8f);           // Enable Automatic Gain Control to normalize mic volume (target 0.0-1.0)
  rtspServer.audioProcessor.enableMicVAD(false);                  // Enable Voice Activity Detection to send audio only when voice is detected
  rtspServer.audioProcessor.setMicVADThreshold(80);              // Set VAD sensitivity (0-100, higher = stricter)

  // Speaker Preprocessing
  rtspServer.audioProcessor.enableSpeakerNoiseSuppression(false); // Disable noise suppression for speaker output (optional cleanup)
  rtspServer.audioProcessor.setSpeakerNoiseSuppressionLevel(-15); // Set speaker NS strength if enabled
  rtspServer.audioProcessor.enableSpeakerAGC(true, 0.9f);        // Enable AGC to boost speaker volume (target 0.0-1.0)

  // Advanced Adjustments (require reinitialization, use sparingly)
  // rtspServer.audioProcessor.setSampleRate(8000, 128, 256);    // Change sample rate (e.g., 8 kHz), AEC frame, filter length
  // rtspServer.audioProcessor.setFrameSize(128);                // Adjust frame size for processing (e.g., 128 samples)
  // rtspServer.audioProcessor.setResamplerQuality(7);           // Set resampling quality (0-10, higher = better but slower)
  // rtspServer.audioProcessor.resizeBuffer(2048);               // Resize ring buffer for AEC reference (in samples)
#endif
#endif

  // Create tasks for sending video, and subtitles
  //xTaskCreate(sendVideo, "Video", 8192, NULL, 9, &videoTaskHandle);
  xTaskCreatePinnedToCore(sendVideo, "Video", 8192, NULL, 8, &videoTaskHandle, 0);

  
  // You can use a task to send subtitles every second
  //xTaskCreate(sendSubtitles, "Subtitles", 2560, NULL, 7, &subtitlesTaskHandle);

  // Or a callback to send the subtitles with the callback function 
  rtspServer.startSubtitlesTimer(onSubtitles); // 1-second period

  rtspServer.maxRTSPClients = 5; // Set the maximum number of RTSP Multicast clients else enable OVERRIDE_RTSP_SINGLE_CLIENT_MODE to allow multiple clients for all transports eg. TCP, UDP, Multicast

  rtspServer.setCredentials(rtspUser, rtspPassword); // Set RTSP authentication

  // Initialize the RTSP server
  /**
   * @brief Initializes the RTSP server with the specified configuration.
   * 
   * This method can be called with specific parameters, or the parameters
   * can be set directly in the RTSPServer instance before calling begin().
   * If any parameter is not explicitly set, the method uses default values.
   * 
   * @param transport The transport type. Default is VIDEO_AND_SUBTITLES. Options are (VIDEO_ONLY, AUDIO_ONLY, VIDEO_AND_AUDIO, VIDEO_AND_SUBTITLES, AUDIO_AND_SUBTITLES, VIDEO_AUDIO_SUBTITLES).
   * @param rtspPort The RTSP port to use. Default is 554.
   * @param sampleRate The sample rate for audio streaming. Default is 0 must pass or set if using audio.
   * @param port1 The first port (used for video, audio or subtitles depending on transport). Default is 5430.
   * @param port2 The second port (used for audio or subtitles depending on transport). Default is 5432.
   * @param port3 The third port (used for subtitles). Default is 5434.
   * @param rtpIp The IP address for RTP multicast streaming. Default is IPAddress(239, 255, 0, 1).
   * @param rtpTTL The TTL value for RTP multicast packets. Default is 64.
   * @return true if initialization is successful, false otherwise.
   *
   * Example usage:
   * // Option 1: Start RTSP server with default values
   * if (rtspServer.init()) { 
   *   Serial.println("RTSP server started successfully on port 554"); 
   * } else { 
   *   Serial.println("Failed to start RTSP server"); 
   * }
   * 
   * // Option 2: Set variables directly and then call init
   * rtspServer.transport = RTSPServer::VIDEO_AUDIO_SUBTITLES; 
   * rtspServer.sampleRate = 48000; 
   * rtspServer.rtspPort = 8554; 
   * rtspServer.rtpIp = IPAddress(239, 255, 0, 1); 
   * rtspServer.rtpTTL = 64; 
   * rtspServer.rtpVideoPort = 5004; 
   * rtspServer.rtpAudioPort = 5006; 
   * rtspServer.rtpSubtitlesPort = 5008;
   * if (rtspServer.init()) { 
   *   Serial.println("RTSP server started successfully"); 
   * } else { 
   *   Serial.println("Failed to start RTSP server"); 
   * }
   * 
   * // Option 3: Set variables in the init call
   * if (rtspServer.init(RTSPServer::VIDEO_AUDIO_SUBTITLES, 554, sampleRate)) { 
   *   Serial.println("RTSP server started successfully"); 
   * } else { 
   *   Serial.println("Failed to start RTSP server"); 
   * }
   *
   * Also have deinit() and reinit() for either deinitialise or reinitialise the rtsp server
   * use reinit() if changing settings
   */

#ifdef HAVE_AUDIO
  if (rtspServer.init(RTSPServer::VIDEO_AUDIO_SUBTITLES, 554, sampleRate)) {
    Serial.printf("RTSP server started successfully, Connect to rtsp://%s:554/\n", WiFi.localIP().toString().c_str());
  } else { 
    Serial.println("Failed to start RTSP server"); 
  }
#else
  if (rtspServer.init(RTSPServer::VIDEO_AND_SUBTITLES, 554)) { 
    Serial.printf("RTSP server started successfully using default values, Connect to rtsp://%s:554/\n", WiFi.localIP().toString().c_str());
  } else { 
    Serial.println("Failed to start RTSP server"); 
  }
#endif

  // // Start the HTTP server
  httpd_handle_t server = NULL;
  httpd_config_t config = HTTPD_DEFAULT_CONFIG();
  config.uri_match_fn = httpd_uri_match_wildcard;  // Enable wildcard matching
  config.stack_size = 8192; // Increase stack size to 8192 bytes
  
  if (httpd_start(&server, &config) == ESP_OK) {
    // Setup the ONVIF server using the library
    onvifServer.setup_onvif_server(server);
    onvifServer.setBlockedIPs("192.168.1.103", "192.168.1.112", "192.168.1.120"); // blockes other devices to this device
    onvifServer.startOnvif();
  }
}

// Function to get the number of tasks
UBaseType_t getTaskCount() {
  return uxTaskGetNumberOfTasks();
}

void checkTasks() {
  // Get task count
  UBaseType_t taskCount = getTaskCount();
  
  // Allocate taskDetails dynamically
  TaskStatus_t* taskDetails = (TaskStatus_t*)heap_caps_malloc(taskCount * sizeof(TaskStatus_t), MALLOC_CAP_8BIT);
  if (!taskDetails) {
    Serial.println("ERROR: Failed to allocate taskDetails");
    return;
  }

  // Get task statuses
  UBaseType_t numTasks = uxTaskGetSystemState(taskDetails, taskCount, NULL);

  Serial.println("=== Task Status ===");
  Serial.printf("Total Tasks: %u\n", numTasks);
  Serial.println("Legend: R=Running, X=Ready, B=Blocked, S=Suspended, D=Deleted");
  Serial.println("#\tName\t\tState\tPrio\tBasePrio\tStack (Words)\tStack (Bytes)\tCore\tNum");

  for (UBaseType_t i = 0; i < numTasks; i++) {
    const char* name = taskDetails[i].pcTaskName ? taskDetails[i].pcTaskName : "Unknown";
    const char* stateStr = (taskDetails[i].eCurrentState == eRunning) ? "R" :
                           (taskDetails[i].eCurrentState == eReady) ? "X" :
                           (taskDetails[i].eCurrentState == eBlocked) ? "B" :
                           (taskDetails[i].eCurrentState == eSuspended) ? "S" : "D";
    int priority = taskDetails[i].uxCurrentPriority;
    int basePriority = taskDetails[i].uxBasePriority;
    size_t stackFreeWords = taskDetails[i].xHandle ? uxTaskGetStackHighWaterMark(taskDetails[i].xHandle) : 0;
    size_t stackFreeBytes = stackFreeWords * 4; // 4 bytes per word on ESP32
    int pinnedCore = taskDetails[i].xHandle ? xTaskGetCoreID(taskDetails[i].xHandle) : -1;
    int num = taskDetails[i].xTaskNumber;

    // Core detection for unpinned tasks
    int currentCore = pinnedCore;
    if (pinnedCore == 0x7FFFFFFF && taskDetails[i].xHandle) {
      if (taskDetails[i].eCurrentState == eRunning) {
        if (taskDetails[i].xHandle == xTaskGetCurrentTaskHandleForCore(0)) {
          currentCore = 0;
        } else if (taskDetails[i].xHandle == xTaskGetCurrentTaskHandleForCore(1)) {
          currentCore = 1;
        }
      }
    }

    Serial.printf("%u\t%-12s\t%s\t%d\t%d\t\t%u\t\t%u\t\t%d\t%d", 
                  i + 1, name, stateStr, priority, basePriority, stackFreeWords, stackFreeBytes, currentCore, num);

    if (strcmp(name, "loopTask") == 0) Serial.print("\t[Arduino]");
    if (stackFreeBytes < 1000 && stackFreeBytes > 0) Serial.print(" WARNING: Low stack!");
    if (pinnedCore == 0x7FFFFFFF) {
      Serial.print(" NOTE: No core affinity");
      if (taskDetails[i].eCurrentState != eRunning) {
        Serial.print(" (not running)");
      }
    }
    Serial.println();
  }

  // Free taskDetails
  heap_caps_free(taskDetails);

  uint32_t totalHeap = ESP.getFreeHeap();
  uint32_t psramHeap = ESP.getFreePsram();
  uint32_t sramHeap = (totalHeap > psramHeap) ? totalHeap - psramHeap : totalHeap;

  Serial.printf("Total Free Heap: %lu bytes (%lu KB)\n", totalHeap, totalHeap / 1024);
  Serial.printf("SRAM Free Heap: %lu bytes (%lu KB)\n", sramHeap, sramHeap / 1024);
  Serial.printf("PSRAM Free Heap: %lu bytes (%lu KB)\n", psramHeap, psramHeap / 1024);
  Serial.printf("Max Alloc Heap: %lu bytes (%lu KB)\n", ESP.getMaxAllocHeap(), ESP.getMaxAllocHeap() / 1024);
  Serial.printf("Min Free Heap: %lu bytes (%lu KB)\n", ESP.getMinFreeHeap(), ESP.getMinFreeHeap() / 1024);
  Serial.println("==================");
}

void loop() {
  // static unsigned long lastCheck = 0;
  // if (millis() - lastCheck >= 10000) {  // Check every 5s
  //   checkTasks();
  //   lastCheck = millis();
  // }
  // delay(100);  // Light load on Core 1
}