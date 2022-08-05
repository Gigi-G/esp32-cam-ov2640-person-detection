#ifndef STREAM
#define STREAM
#define PART_BOUNDARY "123456789000000000000987654321"

#include <camera.h>
#include <esp_http_server.h>
#include <machine_learning.h>

static const char* _STREAM_CONTENT_TYPE = "multipart/x-mixed-replace;boundary=" PART_BOUNDARY;
static const char* _STREAM_BOUNDARY = "\r\n--" PART_BOUNDARY "\r\n";
static const char* _STREAM_PART = "Content-Type: image/jpeg\r\nContent-Length: %u\r\n\r\n";

class StreamCamera {
    private:
        static Camera* camera;
        static PersonDetection* detector;
        static QueueImage<uint8_t*>* queueImage;
        httpd_handle_t stream_httpd;

    public:
        StreamCamera();
        void startCameraServer();
        Camera* getCamera();
        static esp_err_t stream_handler(httpd_req_t *req);
        static void run(void* parameter);
};

#endif