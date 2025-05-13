#ifndef IMG_IMAGE_HPP
#define IMG_IMAGE_HPP


#include "JPEGDEC.h"

#include "image_utils.hpp"


namespace img
{


class Image
{
public:
	bool load(const uint8_t *jpeg_data, size_t size);
	const uint16_t *getBuffer() const { return _buffer; }
	int getWidth() const { return _width; }
	int getHeight() const { return _height; }
 
private:
	static Image *_active_instance;
	JPEGDEC _jpeg;
	uint16_t *_buffer = nullptr;
	int _width = 0;
	int _height = 0;

private:
	static int JPEGDraw(JPEGDRAW *pDraw);
};


}	// namespace img


#endif	// IMG_IMAGE_HPP