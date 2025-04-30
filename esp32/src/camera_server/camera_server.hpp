#ifndef SRC_CAMERA_SERVER_CAMERA_SERVER_HPP
#define SRC_CAMERA_SERVER_CAMERA_SERVER_HPP


#include <Arduino.h>
#include <WiFi.h>
#include <WiFiType.h>

#include "esp_camera.h"
#include "esp_timer.h"
#include "img_converters.h"
#include "fb_gfx.h"
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"
#include "esp_http_server.h"

#include "camera_config.hpp"

#define PART_BOUNDARY "123456789000000000000987654321"


class CameraServer
{
public:
	CameraServer() = default; 
	bool begin();

private:
	const char *_ssid = "EXTENDED_MOVISTAR_BE3B";
	const char *_password = "a12fg45ca";
	static constexpr const char* _STREAM_CONTENT_TYPE = 
		"multipart/x-mixed-replace;boundary=" PART_BOUNDARY;
	static constexpr const char* _STREAM_BOUNDARY = 
		"\r\n--" PART_BOUNDARY "\r\n";
	static constexpr const char* _STREAM_PART = 
		"Content-Type: image/jpeg\r\n"
		"Content-Length: %u\r\n\r\n";
	httpd_handle_t _stream_httpd = NULL;
private:
	bool beginWiFi();
	bool beginCam();
	bool beginCamServer();
	esp_err_t streamHandler(httpd_req_t *req);
	static esp_err_t _s_streamHandler(httpd_req_t *req) {
		auto *self = static_cast<CameraServer*>(req->user_ctx);
		return self->streamHandler(req);
	}
};


#endif
