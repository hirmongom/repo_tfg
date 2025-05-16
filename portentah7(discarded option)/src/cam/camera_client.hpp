#ifndef CAM_CAMERA_CLIENT_HPP
#define CAM_CAMERA_CLIENT_HPP


#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClient.h>

#include <NetworkInterface.h>
#include <utility/http_request.h>
#include <utility/http_response.h>

#include "../util/http_resp.hpp"


namespace cam
{


class CameraClient
{
public:
	CameraClient(): _srv_addr(192, 168, 1, 32) {}
	bool begin();
	bool capture(util::http_response_t &out_http_resp);

private:
	NetworkInterface *_interface;
	WiFiClient _client;
	const char *_ssid = "PortentaHost";
	const char *_password = "default123";
	const IPAddress _srv_addr;
	const uint16_t _srv_port = 80;

private:
	bool beginWiFi();
	bool serverConnect();
};


}	// namespace cam


#endif	// #ifndef CAM_CAMERA_CLIENT_HPP