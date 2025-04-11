#include "src/wifi_ap_connection/wifi_ap_connection.hpp"
#include "src/wifi_web_server/wifi_web_server.hpp"

WiFiAPConnection connection;
WiFiWebServer server;

void setup()
{
	Serial.begin(112500);
	while (!Serial);

	connection.begin(); 
	if (!server.begin()) {
		while (1);
	}
}


void loop()
{
	server.handleClient();
}