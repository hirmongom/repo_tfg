#include "src/camera_client/camera_client.hpp"


CameraClient camera;


/******************************************************************************/
void setup()
{
	Serial.begin(112500);
	while (!Serial);

	if (!camera.begin()) {
		Serial.println("Failed to Initialize Camera Client");
		while (1);
	}

	while (!camera.capture());
}


/******************************************************************************/
void loop()
{
	delay(1);
}