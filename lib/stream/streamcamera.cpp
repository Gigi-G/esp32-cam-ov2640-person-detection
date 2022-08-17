#include <streamcamera.h>

Camera* StreamCamera::camera = new Camera();

StreamCamera::StreamCamera() {
    stream_httpd = NULL;
    camera->init();
}

esp_err_t StreamCamera::stream_handler(httpd_req_t *req) {
    char * part_buf[64];
    esp_err_t res = ESP_OK;

    res = httpd_resp_set_type(req, _STREAM_CONTENT_TYPE);
    if(res != ESP_OK){
        return res;
    }

    Serial.print("Run streaming on core ");
    Serial.println(xPortGetCoreID());

    while(true) {
        delay(1);
        res = camera->getImage();

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
        delay(1);
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
    
    if (httpd_start(&stream_httpd, &config) == ESP_OK) {
        httpd_register_uri_handler(stream_httpd, &index_uri);
        Serial.printf("Starting web server on port: '%d'\n", config.server_port);
    }
}