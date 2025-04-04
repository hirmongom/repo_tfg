#include "esp_camera.h"
#include "src/camera/camera.hpp" 
#include "src/spi_slave/spi_slave.hpp"


Camera camera;
SPI_Slave spiSlave;


void setup() 
{
	// Serial Debug
	Serial.begin(115200);
	Serial.setDebugOutput(true);

	// Camera setup
	if (camera.begin()) {
		Serial.println("Camera initialized successfully.");
	}
	else {
		Serial.println("Camera initialization failed.");
	}

	// SPI Slave setup
	if (spiSlave.begin()) {
		Serial.println("SPI Slave initialized successfully.");
	}
	else {
		Serial.println("SPI Slave initialization failed.");
	}
}


void loop()
{
	spiSlave.handleTransaction();
}
