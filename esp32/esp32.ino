#include "src/camera_server/camera_server.hpp"


CameraServer server;


/******************************************************************************/
void setup()
{
	Serial.begin(112500);
	while (!Serial);

	if (!server.begin()) {
		while (1);
	}
}


/******************************************************************************/
void loop()
{
}