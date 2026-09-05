#include <WiFi.h>
#include "esp_camera.h"
#include <HTTPClient.h>

// --- TEAM CONFIGURATION ---
const char* ssid = "YOUR_WIFI_NAME";         
const char* password = "YOUR_WIFI_PASSWORD"; 
String serverName = "https://YOUR-RENDER-URL.onrender.com/api/upload"; 
String deviceID = "POLE-001";
String deviceLocation = "Central Station, Platform 1";

// --- PINS ---
const int buttonPin = 14; 
const int buzzerPin = 12;
const int ledProcessing = 13; 
const int ledSuccess = 15;    
const int ledWarning = 2;     

unsigned long lastPressTime = 0;
const unsigned long cooldownPeriod = 10000; // 10-second spam protection

// --- CAMERA PINS (AI Thinker) ---
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

void setup() {
  Serial.begin(115200);
  delay(2000);

  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledProcessing, OUTPUT);
  pinMode(ledSuccess, OUTPUT);
  pinMode(ledWarning, OUTPUT);

  digitalWrite(buzzerPin, LOW);
  digitalWrite(ledProcessing, LOW);
  digitalWrite(ledSuccess, LOW);
  digitalWrite(ledWarning, LOW);

  Serial.print("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { delay(500); }

  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM; config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM; config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM; config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM; config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM; config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM; config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM; config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM; config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  
  // Privacy Settings (Low Res, High Compression)
  config.frame_size = FRAMESIZE_HQVGA; 
  config.jpeg_quality = 20;            
  config.fb_count = 1;

  esp_camera_init(&config);
}

void loop() {
  if (digitalRead(buttonPin) == LOW) {
    if (millis() - lastPressTime < cooldownPeriod && lastPressTime != 0) {
      for(int i=0; i<3; i++){
        digitalWrite(ledWarning, HIGH); delay(100);
        digitalWrite(ledWarning, LOW); delay(100);
      }
      while(digitalRead(buttonPin) == LOW) { delay(10); }
      return; 
    }

    lastPressTime = millis();
    digitalWrite(ledProcessing, HIGH);
    digitalWrite(buzzerPin, HIGH); delay(200); digitalWrite(buzzerPin, LOW);

    camera_fb_t * fb = esp_camera_fb_get();
    if (!fb) { digitalWrite(ledProcessing, LOW); return; }

    if(WiFi.status() == WL_CONNECTED){
      HTTPClient http;
      http.begin(serverName);
      http.addHeader("Content-Type", "image/jpeg");
      http.addHeader("X-Device-ID", deviceID);
      http.addHeader("X-Location", deviceLocation);
      
      int httpResponseCode = http.POST(fb->buf, fb->len);
      
      if(httpResponseCode > 0){
        digitalWrite(ledProcessing, LOW);
        digitalWrite(ledSuccess, HIGH); delay(3000); digitalWrite(ledSuccess, LOW);
      } else {
        digitalWrite(ledProcessing, LOW);
      }
      http.end();
    }
    esp_camera_fb_return(fb); 
    while(digitalRead(buttonPin) == LOW) { delay(10); }
  }
}




















Detailed code:

#include <WiFi.h>
#include "esp_camera.h"
#include <HTTPClient.h>

// ===========================
// 1. YOUR CREDENTIALS & INFO
// ===========================
const char* ssid = "YOUR_WIFI_NAME";         // CHANGE THIS
const char* password = "YOUR_WIFI_PASSWORD"; // CHANGE THIS

// Paste your Render URL here (Make sure it ends in /api/upload)
String serverName = "https://your-render-app-name.onrender.com/api/upload"; // CHANGE THIS

String deviceID = "POLE-001";
String deviceLocation = "Central Station, Platform 1";

// ===========================
// 2. HARDWARE PINS
// ===========================
const int buttonPin = 14; 
const int buzzerPin = 12;
const int ledProcessing = 13; // LED 1: Blue/Yellow (Working)
const int ledSuccess = 15;    // LED 2: Green (Sent successfully)
const int ledWarning = 2;     // LED 3: Red (Spam warning)

// ===========================
// 3. TIMING & SPAM LOGIC
// ===========================
unsigned long lastPressTime = 0;
const unsigned long cooldownPeriod = 10000; // 10 seconds

// ===========================
// 4. CAMERA PINS (AI Thinker)
// ===========================
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27
#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

void setup() {
  Serial.begin(115200);
  delay(2000);

  // Setup Hardware Pins
  pinMode(buttonPin, INPUT_PULLUP);
  pinMode(buzzerPin, OUTPUT);
  pinMode(ledProcessing, OUTPUT);
  pinMode(ledSuccess, OUTPUT);
  pinMode(ledWarning, OUTPUT);

  // Turn everything off to start
  digitalWrite(buzzerPin, LOW);
  digitalWrite(ledProcessing, LOW);
  digitalWrite(ledSuccess, LOW);
  digitalWrite(ledWarning, LOW);

  // Connect to Wi-Fi
  Serial.print("Connecting to Wi-Fi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWi-Fi Connected!");

  // Setup Camera for Privacy (Very Low Res, High Compression)
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM; config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM; config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM; config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM; config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM; config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM; config.pin_href = HREF_GPIO_NUM;
  config.pin_sccb_sda = SIOD_GPIO_NUM; config.pin_sccb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM; config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  
  // PRIVACY SETTINGS
  config.frame_size = FRAMESIZE_HQVGA; // 240x176 pixels
  config.jpeg_quality = 20;            // Highly compressed
  config.fb_count = 1;

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed! Error 0x%x", err);
    return;
  }
  
  Serial.println("StreetConnect Prototype Ready!");
}

void loop() {
  if (digitalRead(buttonPin) == LOW) {
    
    // --- SPAM CHECK ---
    if (millis() - lastPressTime < cooldownPeriod && lastPressTime != 0) {
      Serial.println("SPAM WARNING: Cooldown active.");
      for(int i=0; i<3; i++){
        digitalWrite(ledWarning, HIGH); delay(100);
        digitalWrite(ledWarning, LOW); delay(100);
      }
      while(digitalRead(buttonPin) == LOW) { delay(10); }
      return; 
    }

    // --- NORMAL TRIGGER ---
    Serial.println("\nHelp Request Triggered!");
    lastPressTime = millis();
    
    // 1. Feedback
    digitalWrite(ledProcessing, HIGH);
    digitalWrite(buzzerPin, HIGH);
    delay(200);
    digitalWrite(buzzerPin, LOW);

    // 2. Capture Image
    camera_fb_t * fb = esp_camera_fb_get();
    if (!fb) {
      Serial.println("Camera capture failed!");
      digitalWrite(ledProcessing, LOW);
      return;
    }

    // 3. Send to Render Server
    if(WiFi.status() == WL_CONNECTED){
      Serial.println("Uploading to Render Server...");
      HTTPClient http;
      http.begin(serverName);
      
      http.addHeader("Content-Type", "image/jpeg");
      http.addHeader("X-Device-ID", deviceID);
      http.addHeader("X-Location", deviceLocation);
      
      int httpResponseCode = http.POST(fb->buf, fb->len);
      
      if(httpResponseCode > 0){
        Serial.println("Success! Render received it.");
        digitalWrite(ledProcessing, LOW);
        digitalWrite(ledSuccess, HIGH); 
        delay(3000);
        digitalWrite(ledSuccess, LOW);
      } else {
        Serial.printf("Failed to reach Render! Error: %s\n", http.errorToString(httpResponseCode).c_str());
        digitalWrite(ledProcessing, LOW);
      }
      http.end();
    } else {
      Serial.println("Wi-Fi disconnected. Cannot send.");
      digitalWrite(ledProcessing, LOW);
    }

    // 4. Cleanup
    esp_camera_fb_return(fb); 
    while(digitalRead(buttonPin) == LOW) { delay(10); }
  }
}












Connections:

1. Power Distribution
To ensure the camera module receives enough power to operate without crashing, the system is powered via the 5V line.
•	ESP32-CAM 5V Pin: Connected to the positive (+) power rail of the breadboard.
•	ESP32-CAM GND Pin: Connected to the negative (-) ground rail of the breadboard.
Component	ESP32-CAM Pin	Connection Details
Push Button (Trigger)	GPIO 14	One leg to Pin 14, the other leg to GND. (Uses internal pull-up resistor in code).
Active Buzzer	GPIO 12	Positive (longer) leg to Pin 12, negative (shorter) leg to GND.
Blue LED (Processing)	GPIO 13	Positive leg to Pin 13. Negative leg connects to a 220Ω resistor, which connects to GND.
Green LED (Success)	GPIO 15	Positive leg to Pin 15. Negative leg connects to a 220Ω resistor, which connects to GND.
Red LED (Spam Warning)	GPIO 2	Positive leg to Pin 2. Negative leg connects to a 220Ω resistor, which connects to GND.

