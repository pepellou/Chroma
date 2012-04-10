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

void Image::fillColor(
	CvPoint startPoint,
	CvScalar color,
	CvScalar threshold
) {
	cvFloodFill(_cvImage, startPoint, color, threshold, threshold);
}

void Image::invert(
	Image *src,
	Image *dst
) {
	cvCopy(src->cvImage(), dst->cvImage());
	uchar *data = (uchar *)dst->cvImage()->imageData;
	int i, j, k;
	int width = src->cvImage()->width;
	int height = src->cvImage()->height;
	int channels = src->cvImage()->nChannels;
	int step = src->cvImage()->widthStep;
	for(i=0;i<height;i++)
		for(j=0;j<width;j++)
			for(k=0;k<channels;k++)
				data[i*step+j*channels+k]=255-data[i*step+j*channels+k];
}

