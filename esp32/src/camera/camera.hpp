#ifndef SRC_CAMERA_CAMERA_HPP
#define SRC_CAMERA_CAMERA_HPP


#include "esp_camera.h"


#define CAMERA_MODEL_AI_THINKER
#include "camera_pins.h"


class Camera
{
private:
	static const camera_config_t _config;

public:
	Camera() = default;
	bool begin();
	camera_fb_t* capture();
	void release(camera_fb_t *fb);
};


#endif