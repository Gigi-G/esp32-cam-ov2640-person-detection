#include <camera.h>

void Camera::init() {
    Serial.printf("Init camera...");
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
    config.pin_sscb_sda = SIOD_GPIO_NUM;
    config.pin_sscb_scl = SIOC_GPIO_NUM;
    config.pin_pwdn = PWDN_GPIO_NUM;
    config.pin_reset = RESET_GPIO_NUM;
    config.xclk_freq_hz = 20000000;
    config.pixel_format = PIXFORMAT_GRAYSCALE;
    
    if(psramFound()){
        config.frame_size = FRAMESIZE_96X96;
        config.jpeg_quality = 10;
        config.fb_count = 1;
    } else {
        config.frame_size = FRAMESIZE_SVGA;
        config.jpeg_quality = 12;
        config.fb_count = 1;
    }
    
    // Camera init
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
        Serial.printf("Camera init failed with error 0x%x", err);
        exit(-1);
    }

    count = 0;
    queueImage = Queue<uint8_t*>::getInstance();

    Serial.printf("OK\n");
}

esp_err_t Camera::getImage() {
    esp_err_t res = ESP_OK;
    _jpg_buf_len = 0;
    _jpg_buf = NULL;

    fb = esp_camera_fb_get();
    if (!fb) {
        Serial.println("Camera capture failed");
        res = ESP_FAIL;
    }
    else if(fb->width > 32) {
        if(++count % 20000000 >= 0) {
            uint8_t* frame = new uint8_t[fb->len];
            memcpy(frame, fb->buf, fb->len);
            queueImage->push(frame);
            count = 0;
        }
        if(fb->format != PIXFORMAT_JPEG){
            bool jpeg_converted = frame2jpg(fb, 80, &_jpg_buf, &_jpg_buf_len);
            esp_camera_fb_return(fb);
            fb = NULL;
            if(!jpeg_converted){
                Serial.println("JPEG compression failed");
                res = ESP_FAIL;
            }
        }
        else {
            _jpg_buf_len = fb->len;
            _jpg_buf = fb->buf;
        }
    }
    return res;
}

void Camera::restore() {
    if(fb){
        esp_camera_fb_return(fb);
        fb = NULL;
        _jpg_buf = NULL;
    } else if(_jpg_buf){
        free(_jpg_buf);
        _jpg_buf = NULL;
    }
}

size_t Camera::getJPGBufLen() {
    return this->_jpg_buf_len;
}

uint8_t* Camera::getJPGBuf() {
    return this->_jpg_buf;
}