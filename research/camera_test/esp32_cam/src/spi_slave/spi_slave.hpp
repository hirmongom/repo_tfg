#ifndef SPI_SLAVE_SPI_SLAVE_HPP
#define SPI_SLAVE_SPI_SLAVE_HPP

extern "C"
{
	#include "driver/spi_slave.h"
}

#define MISO_PIN	12
#define MOSI_PIN	13
#define SCK_PIN		14
#define CS_PIN		2 

class SPI_Slave
{
public:
	SPI_Slave() = default;
	bool begin();
	void handleTransaction();

private:
	void sendResponse(uint8_t *data, size_t length);
};

#endif