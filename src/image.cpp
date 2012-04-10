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
	Image *in
) {
	Image *out = new Image(cvCreateImage(
		cvGetSize(in),
		IPL_DEPTH_8U,
		3
	));
	cvSmooth( in->cvImage(), out->cvImage(), CV_GAUSSIAN, 3, 3 );

	return out;
}
