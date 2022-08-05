#include <Arduino.h>
#include <password.h>
#include <WiFi.h>
#include <esp_timer.h>
#include <img_converters.h>
#include <fb_gfx.h>
#include <soc/soc.h> //disable brownout problems
#include <soc/rtc_cntl_reg.h>  //disable brownout problems
#include <streamcamera.h>

Camera* StreamCamera::camera = new Camera();
PersonDetection* StreamCamera::detector = new PersonDetection();
QueueImage<uint8_t*>* StreamCamera::queueImage = new QueueImage<uint8_t*>();

void setup() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector
 
  Serial.begin(115200);
  Serial.setDebugOutput(false);

  // Wi-Fi connection
  WiFi.begin(STA_SSID, STA_PSK);
  while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  Serial.print("Camera Stream Ready! Go to: http://");
  Serial.print(WiFi.localIP());
  
  // Start streaming web server
  StreamCamera stream = StreamCamera();
  stream.startCameraServer();
}

void loop() {
}