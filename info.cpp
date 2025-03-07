#include "esp_camera.h"
#include <WiFi.h>
#include "sensor.h" // Include the sensor.h header

//
// WARNING!!! PSRAM IC required for UXGA resolution and high JPEG quality
//            Ensure ESP32 Wrover Module or other board with PSRAM is selected
//            Partial images will be transmitted if image exceeds buffer size
//
//            You must select partition scheme from the board menu that has at least 3MB APP space.
//            Face Recognition is DISABLED for ESP32 and ESP32-S2, because it takes up from 15
//            seconds to process single frame. Face Detection is ENABLED if PSRAM is enabled as well

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
const char *ssid = "**********";
const char *password = "**********";

void startCameraServer();
void setupLedFlash(int pin);

#include <vector>
#include "esp_camera.h"
#include "sensor.h" // Include the sensor.h header

struct CameraSettings {
    framesize_t framesize;
    bool scale;
    bool binning;
    uint8_t quality;
    int8_t brightness;
    int8_t contrast;
    int8_t saturation;
    int8_t sharpness;
    uint8_t denoise;
    uint8_t special_effect;
    uint8_t wb_mode;
    uint8_t awb;
    uint8_t awb_gain;
    uint8_t aec;
    uint8_t aec2;
    int8_t ae_level;
    uint16_t aec_value;
    uint8_t agc;
    uint8_t agc_gain;
    uint8_t gainceiling;
    uint8_t bpc;
    uint8_t wpc;
    uint8_t raw_gma;
    uint8_t lenc;
    bool hmirror;
    bool vflip;
    uint8_t dcw;
    uint8_t colorbar;
};

CameraSettings getCameraSettings() {
    CameraSettings settings = {};
    sensor_t* sensor = esp_camera_sensor_get();
    if (sensor) {
        settings.framesize = sensor->status.framesize;
        settings.scale = sensor->status.scale != 0;
        settings.binning = sensor->status.binning != 0;
        settings.quality = sensor->status.quality;
        settings.brightness = sensor->status.brightness;
        settings.contrast = sensor->status.contrast;
        settings.saturation = sensor->status.saturation;
        settings.sharpness = sensor->status.sharpness;
        settings.denoise = sensor->status.denoise;
        settings.special_effect = sensor->status.special_effect;
        settings.wb_mode = sensor->status.wb_mode;
        settings.awb = sensor->status.awb;
        settings.awb_gain = sensor->status.awb_gain;
        settings.aec = sensor->status.aec;
        settings.aec2 = sensor->status.aec2;
        settings.ae_level = sensor->status.ae_level;
        settings.aec_value = sensor->status.aec_value;
        settings.agc = sensor->status.agc;
        settings.agc_gain = sensor->status.agc_gain;
        settings.gainceiling = sensor->status.gainceiling;
        settings.bpc = sensor->status.bpc;
        settings.wpc = sensor->status.wpc;
        settings.raw_gma = sensor->status.raw_gma;
        settings.lenc = sensor->status.lenc;
        settings.hmirror = sensor->status.hmirror != 0;
        settings.vflip = sensor->status.vflip != 0;
        settings.dcw = sensor->status.dcw;
        settings.colorbar = sensor->status.colorbar;
    }
    return settings;
}

std::vector<resolution_info_t> getAvailableResolutions() {
    std::vector<resolution_info_t> resolutions;
    sensor_t* sensor = esp_camera_sensor_get();
    camera_sensor_info_t* info = getSensorInfo();
    if (sensor && info) {
        framesize_t max_size = info->max_size;
        for (int fs = 0; fs <= max_size; ++fs) {
            resolutions.push_back(resolution[fs]);
        }
    }
    return resolutions;
}



camera_sensor_info_t* getSensorInfo() {
    sensor_t* sensor = esp_camera_sensor_get();
    if (sensor) {
        return esp_camera_sensor_get_info(&sensor->id);
    }
    return nullptr;
}

// Function to print camera settings to console
void printCameraSettings(const CameraSettings& settings) {
    Serial.println("=== Camera Settings ===");
    Serial.printf("Frame Size: %d (Width: %d, Height: %d)\n", settings.framesize, 
                  resolution[settings.framesize].width, resolution[settings.framesize].height);
    Serial.printf("Scale: %d, Binning: %d\n", settings.scale, settings.binning);
    Serial.printf("Quality: %d\n", settings.quality);
    Serial.printf("Brightness: %d, Contrast: %d, Saturation: %d, Sharpness: %d\n", 
                  settings.brightness, settings.contrast, settings.saturation, settings.sharpness);
    Serial.printf("Denoise: %d, Special Effect: %d, White Balance Mode: %d\n", 
                  settings.denoise, settings.special_effect, settings.wb_mode);
    Serial.printf("AWB: %d, AWB Gain: %d\n", settings.awb, settings.awb_gain);
    Serial.printf("AEC: %d, AEC2: %d, AE Level: %d, AEC Value: %d\n", 
                  settings.aec, settings.aec2, settings.ae_level, settings.aec_value);
    Serial.printf("AGC: %d, AGC Gain: %d, Gain Ceiling: %d\n", 
                  settings.agc, settings.agc_gain, settings.gainceiling);
    Serial.printf("BPC: %d, WPC: %d, Raw GMA: %d, Lens Correction: %d\n", 
                  settings.bpc, settings.wpc, settings.raw_gma, settings.lenc);
    Serial.printf("Horizontal Mirror: %d, Vertical Flip: %d\n", settings.hmirror, settings.vflip);
    Serial.printf("DCW: %d, Colorbar: %d\n", settings.dcw, settings.colorbar);
    Serial.println("======================");
}

// Function to print available resolutions to console
void printAvailableResolutions(const std::vector<resolution_info_t>& resolutions) {
    Serial.println("=== Available Resolutions ===");
    for (size_t i = 0; i < resolutions.size(); i++) {
        Serial.printf("%d: %dx%d\n", i, resolutions[i].width, resolutions[i].height);
    }
    Serial.println("=============================");
}

// Function to print sensor information to console
void printSensorInfo(camera_sensor_info_t* info) {
    if (info != nullptr) {
        Serial.println("=== Sensor Information ===");
        Serial.printf("Model: %d\n", info->model);
        Serial.printf("Name: %s\n", info->name);
        Serial.printf("SCCB Address: 0x%02X\n", info->sccb_addr);
        Serial.printf("PID: 0x%04X\n", info->pid);
        Serial.printf("Max Size: %d\n", info->max_size);
        Serial.printf("Support JPEG: %s\n", info->support_jpeg ? "Yes" : "No");
        Serial.println("==========================");
    } else {
        Serial.println("Failed to get sensor information.");
    }
}

void setup() {
  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();

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
  //config.pixel_format = PIXFORMAT_RGB565; // for face detection/recognition
  config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
  config.fb_location = CAMERA_FB_IN_PSRAM;
  config.jpeg_quality = 12;
  config.fb_count = 1;

  // if PSRAM IC present, init with UXGA resolution and higher JPEG quality
  //                      for larger pre-allocated frame buffer.
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

  // camera init
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

// Setup LED FLash if LED pin is defined in camera_pins.h
#if defined(LED_GPIO_NUM)
  setupLedFlash(LED_GPIO_NUM);
#endif

//  WiFi.begin(ssid, password);
//  WiFi.setSleep(false);
//
//  Serial.print("WiFi connecting");
//  while (WiFi.status() != WL_CONNECTED) {
//    delay(500);
//    Serial.print(".");
//  }
//  Serial.println("");
//  Serial.println("WiFi connected");
//
//  startCameraServer();
//
//  Serial.print("Camera Ready! Use 'http://");
//  Serial.print(WiFi.localIP());
//  Serial.println("' to connect");
  
  // Get and print camera settings
  CameraSettings settings = getCameraSettings();
  printCameraSettings(settings);
//
//  // Get and print available resolutions
//  std::vector<Resolution> resolutions = getAvailableResolutions();
//  printAvailableResolutions(resolutions);

  
    // Print available resolutions
    auto resolutions = getAvailableResolutions();
    printAvailableResolutions(resolutions);

    // Print sensor information
    auto sensorInfo = getSensorInfo();
    printSensorInfo(sensorInfo);
}

void loop() {
  // Do nothing. Everything is done in another task by the web server
  delay(10000);
}
