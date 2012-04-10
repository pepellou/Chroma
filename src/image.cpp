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

void Image::setCvImage(
	IplImage *cvImage
) {
	this->_cvImage = cvImage;
}

void Image::release(
) {
	cvReleaseImage(&_cvImage);
}

Image *Image::smooth(
) {
	Image *out = new Image(cvCreateImage(
		cvGetSize(_cvImage),
		IPL_DEPTH_8U,
		3
	));
	cvSmooth( _cvImage, out->cvImage(), CV_GAUSSIAN, 3, 3 );

	return out;
}

Image* Image::duplicate(
) {
	Image* out = new Image(cvCreateImage(
		cvSize(2*_cvImage->width, 2*_cvImage->height),
		IPL_DEPTH_8U,
		_cvImage->nChannels
	));
	cvPyrUp(_cvImage, out->cvImage());
	return out;
}

