#include <streamcamera.h>

StreamCamera::StreamCamera() {
    stream_httpd = NULL;
    camera->init();
    if(kTfLiteOk != detector->init()) {
        Serial.println("ERROR");
        exit(-1);
    }
}

Camera* StreamCamera::getCamera() {
    return this->camera;
}

void StreamCamera::run(void* parameter) {
    Serial.print("run inference on core ");
    Serial.println(xPortGetCoreID());
    int count = 0;
    while(true) {
        if(++count % 20000000 == 0 && !queueImage->isEmpty()) {
            count = 1;
            Node<uint8_t*>* n = queueImage->pop();
            if(n->val) {
                detector->getInput()->data.uint8 = n->val;
                detector->run();
                delete[] n;
            }
        }
        //detector->run();
    }
}

esp_err_t StreamCamera::stream_handler(httpd_req_t *req) {
    char * part_buf[64];
    esp_err_t res = ESP_OK;

    res = httpd_resp_set_type(req, _STREAM_CONTENT_TYPE);
    if(res != ESP_OK){
        return res;
    }

    TaskHandle_t Task1;

    xTaskCreatePinnedToCore(
        run, /* Function to implement the task */
        "Inference", /* Name of the task */
        10000,  /* Stack size in words */
        NULL,  /* Task input parameter */
        0,  /* Priority of the task */
        &Task1,  /* Task handle. */
        1 /* Core where the task should run */ 
    );

    while(true) {
        res = camera->getImage(queueImage);

        if(res == ESP_OK){
            size_t hlen = snprintf((char *)part_buf, 64, _STREAM_PART, camera->getJPGBufLen());
            res = httpd_resp_send_chunk(req, (const char *)part_buf, hlen);
        }
        if(res == ESP_OK){
            res = httpd_resp_send_chunk(req, (const char *)camera->getJPGBuf(), camera->getJPGBufLen());
        }
        if(res == ESP_OK){
            res = httpd_resp_send_chunk(req, _STREAM_BOUNDARY, strlen(_STREAM_BOUNDARY));
        }

        camera->restore();
        if(res != ESP_OK){
            break;
        }
    }
  return res;
}

void StreamCamera::startCameraServer() {
    httpd_config_t config = HTTPD_DEFAULT_CONFIG();
    config.server_port = 80;

    httpd_uri_t index_uri = {
        .uri       = "/",
        .method    = HTTP_GET,
        .handler   = stream_handler,
        .user_ctx  = NULL
    };
    
    //Serial.printf("Starting web server on port: '%d'\n", config.server_port);
    if (httpd_start(&stream_httpd, &config) == ESP_OK) {
        httpd_register_uri_handler(stream_httpd, &index_uri);
    }
}