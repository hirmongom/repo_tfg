#include "wifi_ap_connection.hpp"


/******************************************************************************/
void WiFiAPConnection::begin()
{
	WiFi.config(
		IPAddress(192, 168, 0, 32), 
		IPAddress(255, 255, 255, 0), 
		IPAddress(192, 168, 0, 1)
	);
	WiFi.begin(_ssid, _password);
  WiFi.setSleep(false);

  Serial.print("WiFi connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected.");
	Serial.print("Local IP: ");
	Serial.println(WiFi.localIP());
}