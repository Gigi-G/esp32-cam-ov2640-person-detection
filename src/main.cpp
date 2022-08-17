#include <Arduino.h>
#include <password.h>
#include <WiFi.h>
#include <esp_timer.h>
#include <img_converters.h>
#include <fb_gfx.h>
#include <soc/soc.h>            //disable brownout problems
#include <soc/rtc_cntl_reg.h>   //disable brownout problems
#include <streamcamera.h>

void _taskStreaming();
void _taskDetection(void* parameters);

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
  
  Serial.print("Camera Ready! IP: ");
  Serial.println(WiFi.localIP());

  TaskHandle_t taskDetection;

  xTaskCreatePinnedToCore(
    _taskDetection,   /* Task function. */
    "Detection",      /* name of task. */
    10000,            /* Stack size of task */
    NULL,             /* parameter of the task */
    1,                /* priority of the task */
    &taskDetection,   /* Task handle to keep track of created task */
    1                 /* pin task to core 1 */
  );
  delay(500);

  _taskStreaming();
}

void _taskStreaming() {
  // Start streaming web server
  StreamCamera stream = StreamCamera();
  stream.startCameraServer();
}

void _taskDetection(void* parameters) {
  // Start person detector
  Serial.print("Run detection on core ");
  Serial.println(xPortGetCoreID());

  PersonDetection* detector = new PersonDetection();
  static Queue<uint8_t*>* queueImage = Queue<uint8_t*>::getInstance();

  if(kTfLiteOk != detector->init()) {
      Serial.println("ERROR");
      exit(-1);
  }

  if(!queueImage) {
    Serial.println("Error in queue");
    exit(-1);
  }
  
  while(true) {
    delay(1);
    if(!queueImage->isEmpty()) {
      Node<uint8_t*>* n = queueImage->pop();
      if(n->val) {
          detector->getInput()->data.uint8 = n->val;
          detector->run();
          delete n->val;
          delete n;
          queueImage->clear();
      }
    }
    delay(1);
  }
}

void loop() {
}