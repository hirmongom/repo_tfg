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
	WiFi.begin(_ssid, _password);
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
	config.server_port = 80;

	httpd_uri_t index_uri = {
		.uri       = "/",
		.method    = HTTP_GET,
		.handler  = CameraServer::_s_streamHandler,
		.user_ctx = this 
	};
	
	Serial.println("Starting Camera Web Server on port: 80");
	esp_err_t err = httpd_start(&_stream_httpd, &config);
	if (err == ESP_OK) {
		httpd_register_uri_handler(_stream_httpd, &index_uri);
	}
	else {
		Serial.printf("Camera Server init failed with error 0x%x", err);
		return false;	
	}

	Serial.println("Camera Web Server Initialized.");
	return true;
}


/******************************************************************************/
esp_err_t CameraServer::streamHandler(httpd_req_t *req)
{
	camera_fb_t *fb = NULL;
	esp_err_t err = ESP_OK;
	size_t jpg_buf_len = 0;
	uint8_t *jpg_buf = NULL;
	char *part_buf[64];

	err = httpd_resp_set_type(req, _STREAM_CONTENT_TYPE);
	if(err != ESP_OK) {
		return err;
	}

	while(true) {
		fb = esp_camera_fb_get();
		if (!fb) {
			Serial.println("Camera capture failed");
			err = ESP_FAIL;
		} 
		else {
			bool jpeg_converted = frame2jpg(fb, 80, &jpg_buf, &jpg_buf_len);
			esp_camera_fb_return(fb);
			fb = NULL;
			if (!jpeg_converted) {
				Serial.println("JPEG compression failed");
				err = ESP_FAIL;
			}
		}

		if (err == ESP_OK) {
			size_t hlen = snprintf((char *)part_buf, 64, _STREAM_PART, jpg_buf_len);
			err = httpd_resp_send_chunk(req, (const char *)part_buf, hlen);
		}

		if (err == ESP_OK) {
			err = httpd_resp_send_chunk(req, (const char *)jpg_buf, jpg_buf_len);
		}

		if (err == ESP_OK) {
			err = httpd_resp_send_chunk(
				req, _STREAM_BOUNDARY, 
				strlen(_STREAM_BOUNDARY));
		}

		if (fb) {
			esp_camera_fb_return(fb);
			fb = NULL;
			_jpg_buf = NULL;
		} 
		else if (jpg_buf) {
			free(jpg_buf);
			jpg_buf = NULL;
		}

		if(err != ESP_OK) {
			break;
		}
	}

	return err;
}