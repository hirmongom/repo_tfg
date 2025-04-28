#include "src/wifi_ap_connection/wifi_ap_connection.hpp"
#include "src/camera_server/camera_server.hpp"


WiFiAPConnection connection;
CameraServer server;


/******************************************************************************/
void setup()
{
	Serial.begin(112500);
	while (!Serial);

	connection.begin(); 
	if (!server.begin()) {
		while (1);
	}
}


/******************************************************************************/
void loop()
{
	server.handleClient();
}