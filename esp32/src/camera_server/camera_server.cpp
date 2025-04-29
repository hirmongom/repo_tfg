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
	
	Serial.printf("Starting Camera Web Server on port: '%d'\n", config.server_port);
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
	camera_fb_t * fb = NULL;
	esp_err_t res = ESP_OK;
	size_t _jpg_buf_len = 0;
	uint8_t * _jpg_buf = NULL;
	char * part_buf[64];

	res = httpd_resp_set_type(req, _STREAM_CONTENT_TYPE);
	if(res != ESP_OK){
		return res;
	}

	while(true){
		fb = esp_camera_fb_get();
		if (!fb) {
			Serial.println("Camera capture failed");
			res = ESP_FAIL;
		} else {
			if(fb->width > 400){
				if(fb->format != PIXFORMAT_JPEG){
					bool jpeg_converted = frame2jpg(fb, 80, &_jpg_buf, &_jpg_buf_len);
					esp_camera_fb_return(fb);
					fb = NULL;
					if(!jpeg_converted){
						Serial.println("JPEG compression failed");
						res = ESP_FAIL;
					}
				} else {
					_jpg_buf_len = fb->len;
					_jpg_buf = fb->buf;
				}
			}
		}
		if(res == ESP_OK){
			size_t hlen = snprintf((char *)part_buf, 64, _STREAM_PART, _jpg_buf_len);
			res = httpd_resp_send_chunk(req, (const char *)part_buf, hlen);
		}
		if(res == ESP_OK){
			res = httpd_resp_send_chunk(req, (const char *)_jpg_buf, _jpg_buf_len);
		}
		if(res == ESP_OK){
			res = httpd_resp_send_chunk(req, _STREAM_BOUNDARY, strlen(_STREAM_BOUNDARY));
		}
		if(fb){
			esp_camera_fb_return(fb);
			fb = NULL;
			_jpg_buf = NULL;
		} else if(_jpg_buf){
			free(_jpg_buf);
			_jpg_buf = NULL;
		}
		if(res != ESP_OK){
			break;
		}
		//Serial.printf("MJPG: %uB\n",(uint32_t)(_jpg_buf_len));
	}
	return res;
}