#include "esp_camera.h"
#include <WiFi.h>
#include <WebSocketsClient.h>

#define CAMERA_MODEL_AI_THINKER
#include "camera_pins.h"

const char* ssid = "RoboNetwork";
const char* password = "1234567890";

#define TRIGGER_PIN 14
#define LED_PIN 4

WebSocketsClient webSocket;
bool fotoTrimisa = false;
unsigned long lastTriggerTime = 0;
const unsigned long IGNORE_DURATION = 3000;  // 3 secunde
unsigned long lastHeartbeatTime = 0;
const unsigned long HEARTBEAT_INTERVAL = 10000;  // 10 secunde

void initCameraConfig() {
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
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_QVGA;
  config.jpeg_quality = 18;
  config.fb_count = 1;

  esp_camera_init(&config);
}

void connectToWiFi() {
  WiFi.setHostname("SortoBot-CAM");
  WiFi.begin(ssid, password);
  Serial.println("\n✅ Conectat la WiFi!");
  Serial.print("🔄 Conectare WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(300);
    Serial.print(".");
  }
  Serial.println("\n✅ Conectat la WiFi!");
  Serial.print("📶 IP: ");
  Serial.println(WiFi.localIP());
}

void webSocketEvent(WStype_t type, uint8_t * payload, size_t length) {
  switch(type) {
    case WStype_DISCONNECTED:
      Serial.println("🔌 WebSocket deconectat");
      break;
    case WStype_CONNECTED:
      Serial.println("🔗 WebSocket conectat la server");
      break;
    case WStype_ERROR:
      Serial.println("❌ Eroare WebSocket");
      break;
    default:
      break;
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(TRIGGER_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);


  connectToWiFi();
  initCameraConfig();

  webSocket.begin("192.168.137.1", 5000, "/");  // ← fără /ws
  webSocket.onEvent(webSocketEvent);
  webSocket.setReconnectInterval(3000);  // reconectare la 3s
}

void loop() {
  webSocket.loop();

  // Heartbeat: trimite mesaj text la fiecare 10s
  if (millis() - lastHeartbeatTime > HEARTBEAT_INTERVAL) {
    webSocket.sendTXT("heartbeat");
    lastHeartbeatTime = millis();
    Serial.println("💓 Trimis heartbeat către server");
  }

  int triggerState = digitalRead(TRIGGER_PIN);

  if (triggerState == HIGH && !fotoTrimisa && millis() - lastTriggerTime > IGNORE_DURATION) {
    //digitalWrite(LED_PIN, HIGH);
    delay(100);

    // Ignorăm prima captură
    camera_fb_t* fb1 = esp_camera_fb_get();
    if (fb1) esp_camera_fb_return(fb1);
    delay(100);

    // A doua captură – se trimite
    camera_fb_t* fb2 = esp_camera_fb_get();
    digitalWrite(LED_PIN, LOW);

    if (fb2) {
      Serial.printf("📤 Trimit imagine (%d bytes)...\n", fb2->len);
      webSocket.sendBIN(fb2->buf, fb2->len);
      esp_camera_fb_return(fb2);
    } else {
      Serial.println("❌ Captură eșuată");
    }

    fotoTrimisa = true;
    lastTriggerTime = millis();
  }

  if (triggerState == LOW) {
    fotoTrimisa = false;
  }
}
