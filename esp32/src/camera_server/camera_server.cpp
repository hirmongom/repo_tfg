#include "camera_server.hpp"


/******************************************************************************/
bool CameraServer::begin()
{
	WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

	if (!beginWiFi()) {
		return false;
	}
	if (!beginCam()) {
		return false;
	}
	if (!beginCamServer()) {
		return false;
	}

	Serial.println("Camera Server Started at:");
	Serial.print("\thttp://");
  Serial.println(WiFi.localIP());
	return true;
}


/******************************************************************************/
bool CameraServer::beginWiFi()
{
	if (WiFi.status() == WL_NO_SHIELD) {
		Serial.println("Failed to communicate with the WiFi module.");
		return false;
	}
	
	Serial.print("Connecting to WiFi.");
	WiFi.config(_ip, IPAddress(0, 0, 0, 0), _gateway, _subnet);
	WiFi.begin(_ssid, _password);
	WiFi.setSleep(false);
	while (WiFi.status() != WL_CONNECTED) {
		delay(500);
		Serial.print(".");
	}
	Serial.println();

	Serial.println("WiFi connected.");	
	return true;
}


/******************************************************************************/
bool CameraServer::beginCam()
{
	// @TODO configure sensor to use appropiate capture window
	Serial.println("Initializing Camera");
	esp_err_t err = esp_camera_init(&camera_config);
	if (err != ESP_OK) {
		Serial.printf("Camera init failed with error 0x%x", err);
		return false;
	}

	Serial.println("Camera Initialized");
	return true;
}


/******************************************************************************/
bool CameraServer::beginCamServer()
{
	httpd_config_t config = HTTPD_DEFAULT_CONFIG();
	config.server_port = _port;

	httpd_uri_t capture_uri = {
		.uri       = "/capture",
		.method    = HTTP_GET,
		.handler  = CameraServer::_s_captureHandler,
		.user_ctx = this 
	};
	
	esp_err_t err = httpd_start(&_capture_httpd, &config);
	if (err == ESP_OK) {
		httpd_register_uri_handler(_capture_httpd, &capture_uri);
	}
	else {
		Serial.printf("Camera Server init failed with error 0x%x", err);
		return false;	
	}

	Serial.println("Camera Web Server Initialized.");
	return true;
}


/******************************************************************************/
esp_err_t CameraServer::captureHandler(httpd_req_t *req)
{
	camera_fb_t *fb = NULL;
	esp_err_t err = ESP_OK;

	fb = esp_camera_fb_get();
	if (!fb) {
		Serial.println("Camera capture failed");
		return httpd_resp_send_500(req);
	}

	err = httpd_resp_set_type(req, "image/jpeg");
	if (err != ESP_OK) {
		return err;
	}

	err = httpd_resp_send(req, (const char *)fb->buf, fb->len);

	esp_camera_fb_return(fb);
	return err;
}