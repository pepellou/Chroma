#include "image.h"

Image::Image(
	IplImage *cvImage
) {
	this->_cvImage = cvImage;
}

IplImage *Image::cvImage(
) {
	return this->_cvImage;
}