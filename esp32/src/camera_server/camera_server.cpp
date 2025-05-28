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

	pinMode(_flash_led, OUTPUT);
	digitalWrite(_flash_led, HIGH);

	Serial.println("Camera Server Started at:");
	Serial.print("\thttp://");
  Serial.print(WiFi.softAPIP());
	Serial.println("/frame");
	return true;
}


/******************************************************************************/
bool CameraServer::beginWiFi()
{
	if (WiFi.status() == WL_NO_SHIELD) {
		Serial.println("Failed to communicate with the WiFi module.");
		return false;
	}
	
	Serial.print("Starting WiFi AP.");

	WiFi.softAPConfig(_ip, _gateway, _subnet);
	bool res = WiFi.softAP(_ssid, _password);
	WiFi.setSleep(false);
	if (res) {
		Serial.println("WiFi AP started.");
		return true;
	}
	else {
		Serial.println("Failed to start softAP");
		return false;
	}
}


/******************************************************************************/
bool CameraServer::beginCam()
{
	Serial.println("Initializing Camera");
	esp_err_t err = esp_camera_init(&camera_config);
	if (err != ESP_OK) {
		Serial.printf("Camera init failed with error 0x%x", err);
		return false;
	}
	configureSensor();

	Serial.println("Camera Initialized");
	return true;
}


/******************************************************************************/
bool CameraServer::beginCamServer()
{
	httpd_config_t config = HTTPD_DEFAULT_CONFIG();
	config.server_port = _port;

	httpd_uri_t capture_uri = {
		.uri       = "/frame",
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
void CameraServer::configureSensor()
{
	sensor_t *s = esp_camera_sensor_get();

	/*
	 * Configure a cropped windowed resolution with 21:9 aspect ratio.
	 * 
	 * The cropping region { offsetX = 2, offsetY = 258, totalX = 1596, totalY = 684 }
	 * corresponds to the 21:9 preset found in the ov2640_ratio_settings_t table:
	 * https://github.com/espressif/esp32-camera/blob/master/sensors/private_include/ov2640_settings.h
	 * 
	 * The output resolution is set to 1008×432, which:
	 * - Maintains 21:9 aspect ratio.
	 * - Is fully divisible by 8 and 16 (important for JPEG encoder MCU blocks).
	 * - Fits entirely within the cropped window.
	 * - Avoids artefacts or encoder errors caused by misaligned buffer sizes.
	 * 
	 * Scaling is enabled to resize from the crop window to the output size,
	 * and binning is disabled to preserve maximum image detail.
	 */
	s->set_res_raw(s,
		0, 0,					// startX, startY: sensor raw region start
		1600, 1200,		// endX, endY: full sensor dimensions
		2, 258,				// offsetX, offsetY: crop offsets into the sensor area
		1596, 684,		// totalX, totalY: cropped window size (21:9 aspect)
		1008, 432,		// outputX, outputY: final output resolution
		true,					// scale: enabled (crop → output resize)
		false					// binning: off
	);

	s->set_exposure_ctrl(s, 1);		// enable automatic exposure control
	s->set_aec2(s, 0);						// disable second exposure algorithm (AEC2)
	s->set_ae_level(s, -2);				// Bias automatic exposure towards underexposure
	s->set_aec_value(s, 60);			// Limit max exposure time
	s->set_gain_ctrl(s, 1);				// enable gain control to compensate brightness
	s->set_agc_gain(s, 20);				// moderate gain to boost brightness
	s->set_whitebal(s, 0);				// disable auto white balance
	s->set_wb_mode(s, 3);					// set fixed WB mode (3 = Office)
	s->set_special_effect(s, 0);	// Disable special effects
	s->set_bpc(s, 0);							// Disable black pixel correction
	s->set_wpc(s, 0);							// Disable white pixel correciton 
	s->set_raw_gma(s, 1);					// Enable gamma correction, improve tonal range
	s->set_lenc(s, 0);						// Disable lens correction
	s->set_brightness(s, 1);			// Increase brightness (Range -2 to 2)
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
	Serial.print("fb len = ");
	Serial.println(fb->len);
	err = httpd_resp_send(req, (const char *)fb->buf, fb->len);

	esp_camera_fb_return(fb);
	return err;
}