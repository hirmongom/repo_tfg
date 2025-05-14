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


class CameraServer
{
public:
	CameraServer(): 
		_ip(192, 168, 1, 32),
		_gateway(192, 168, 1, 1),
		_subnet(255, 255, 255, 0) {}
	bool begin();

private:
	const char *_ssid = "ssid";
	const char *_password = "password";
	const IPAddress _ip;
	const IPAddress _gateway;
	const IPAddress _subnet;
	const uint16_t _port = 80;
	httpd_handle_t _capture_httpd = NULL;

private:
	bool beginWiFi();
	bool beginCam();
	bool beginCamServer();
	esp_err_t captureHandler(httpd_req_t *req);
	static esp_err_t _s_captureHandler(httpd_req_t *req) {
		auto *self = static_cast<CameraServer*>(req->user_ctx);
		return self->captureHandler(req);
	}
};


#endif