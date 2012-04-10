#include "image.h"

Image::Image(
	IplImage *cvImage
) {
	this->_cvImage = cvImage;
}

Image::Image(
	const char *filePath
) {
	this->_cvImage = cvLoadImage(filePath);
}

IplImage *Image::cvImage(
) {
	return this->_cvImage;
}

void Image::setCvImage(
	IplImage *cvImage
) {
	this->_cvImage = cvImage;
}

void Image::release(
) {
	cvReleaseImage(&_cvImage);
}

bool Image::isValid(
) {
	return (_cvImage != NULL);
}

Image *Image::clone(
) {
	return new Image(cvCloneImage(_cvImage));
}

Image *Image::cloneJustDimensions(
	int num_channels = -1
) {
	if (num_channels == -1)
		num_channels = _cvImage->nChannels;
	return new Image(
		cvCreateImage(
			cvGetSize(_cvImage), 
			_cvImage->depth, 
			num_channels
		)
	);
}

void Image::cloneTo(
	Image *anotherImage
) {
	cvCopy(_cvImage, anotherImage->_cvImage);
}

void Image::flip(
) {
	cvFlip(_cvImage);
}

void Image::resizeLike(
	Image *anotherImage
) {
	Image *tmp = anotherImage->clone();
	cvResize(_cvImage, tmp->cvImage()); 
	release();
	setCvImage(tmp->cvImage());
}

int Image::originPosition(
) {
	return _cvImage->origin;
}

void Image::setOriginPosition(
	int position
) {
	_cvImage->origin = position;
}
