#include "camera.hpp"


/******************************************************************************/
const camera_config_t Camera::_config = {
	.pin_pwdn = PWDN_GPIO_NUM,
	.pin_reset = RESET_GPIO_NUM,
	.pin_xclk = XCLK_GPIO_NUM,
	.pin_sscb_sda = SIOD_GPIO_NUM,
	.pin_sscb_scl = SIOC_GPIO_NUM,

	.pin_d7 = Y9_GPIO_NUM,
	.pin_d6 = Y8_GPIO_NUM,
	.pin_d5 = Y7_GPIO_NUM,
	.pin_d4 = Y6_GPIO_NUM,
	.pin_d3 = Y5_GPIO_NUM,
	.pin_d2 = Y4_GPIO_NUM,
	.pin_d1 = Y3_GPIO_NUM,
	.pin_d0 = Y2_GPIO_NUM,
	.pin_vsync = VSYNC_GPIO_NUM,
	.pin_href = HREF_GPIO_NUM,
	.pin_pclk = PCLK_GPIO_NUM,

	// Clock configuration
	.xclk_freq_hz = 20000000, // 20MHz XCLK
	.ledc_timer = LEDC_TIMER_0,
	.ledc_channel = LEDC_CHANNEL_0,

	// Image settings
	.pixel_format = PIXFORMAT_JPEG, // Use PIXFORMAT_RGB565 for face detection
	.frame_size = FRAMESIZE_UXGA,   // Use UXGA (1600x1200), limit if no PSRAM

	// Buffer and memory settings
	.jpeg_quality = 12, // Lower number = better quality (0-63)
	.fb_count = 2, // Set to 2 if using PSRAM
	.fb_location = CAMERA_FB_IN_PSRAM, // Use PSRAM if available
	.grab_mode = CAMERA_GRAB_LATEST
};


/******************************************************************************/
bool Camera::begin()
{
	esp_err_t err = esp_camera_init(&Camera::_config);
  if (err != ESP_OK) {
    return false;
  }

  return true;
}


/******************************************************************************/
camera_fb_t* Camera::capture()
{
	return esp_camera_fb_get();
}


/******************************************************************************/
void Camera::release(camera_fb_t *fb)
{
	esp_camera_fb_return(fb);
}