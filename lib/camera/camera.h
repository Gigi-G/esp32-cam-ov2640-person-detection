#ifndef CAMERA
#define CAMERA

#include <Arduino.h>
#include <esp_camera.h>
#include <queue.h>

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

class Camera {
    private:
        camera_fb_t* fb;
        size_t _jpg_buf_len;
        uint8_t* _jpg_buf;
        Queue<uint8_t*>* queueImage;
        int count;

    public:
        void init();
        esp_err_t getImage();
        void restore();
        size_t getJPGBufLen();
        uint8_t* getJPGBuf();
};

#endif