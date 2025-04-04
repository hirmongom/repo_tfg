#include "spi_slave.hpp"


bool SPI_Slave::begin()
{
	// SPI Bus Configuration
	spi_bus_config_t buscfg = {
		.mosi_io_num = MOSI_PIN,
		.miso_io_num = MISO_PIN,
		.sclk_io_num = SCK_PIN,
		.quadwp_io_num = -1,
		.quadhd_io_num = -1,
		.max_transfer_sz = 32
	};

	// SPI Slave Configuration
	spi_slave_interface_config_t slvcfg = {
		.spics_io_num = CS_PIN,
		.flags = 0,
		.queue_size = 3,
		.mode = 0,
		.post_setup_cb = nullptr,
		.post_trans_cb = nullptr
	};

	// Initialize SPI as Slave
	esp_err_t ret = spi_slave_initialize(VSPI_HOST, &buscfg, &slvcfg, SPI_DMA_CH_AUTO);
	return (ret == ESP_OK);
}


void SPI_Slave::handleTransaction()
{
	uint8_t recv_buf[1] = {0};
	uint8_t response[] = {0xAA, 0x55, 'E', 'S', 'P', '3', '2'};

	spi_slave_transaction_t t = {
		.length = 8,
		.rx_buffer = recv_buf
	};

	esp_err_t ret = spi_slave_transmit(VSPI_HOST, &t, portMAX_DELAY);
	if (ret == ESP_OK) {
		if (recv_buf[0] == 0x01) {
			sendResponse(response, sizeof(response));
		}
	}
}


void SPI_Slave::sendResponse(uint8_t *data, size_t length)
{
	spi_slave_transaction_t t = {
		.length = length * 8,
		.tx_buffer = data
	};

	spi_slave_transmit(VSPI_HOST, &t, portMAX_DELAY);
}