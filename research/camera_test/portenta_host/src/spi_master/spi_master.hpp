#ifndef SRC_SPI_MASTER_SPI_MASTER_HPP
#define SRC_SPI_MASTER_SPI_MASTER_HPP


class SPI_Master
{
public:
	SPI_Master() = default;
	bool begin();
	void ping(byte *response);
private:
};


#endif