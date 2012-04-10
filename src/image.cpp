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
	int num_channels,
	int depth
) {
	if (depth == -1)
		depth = _cvImage->depth;
	if (num_channels == -1)
		num_channels = _cvImage->nChannels;
	return new Image(
		cvCreateImage(
			cvGetSize(_cvImage), 
			depth, 
			num_channels
		)
	);
}

void Image::cloneTo(
	Image *anotherImage,
	Image *mask
) {
	if (mask == NULL) {
		cvCopy(
			_cvImage, 
			anotherImage->_cvImage
		);
	} else {
		cvCopy(
			_cvImage, 
			anotherImage->_cvImage, 
			mask->_cvImage
		);
	}
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

Image *Image::differenceWith(
	Image *anotherImage
) {
	Image *difference = anotherImage->clone();
	cvAbsDiff(
		_cvImage, 
		anotherImage->_cvImage, 
		difference->_cvImage
	);
	return difference;
}

void Image::splitTo(
	Image *channel1,
	Image *channel2,
	Image *channel3
) {
	cvSplit(
		_cvImage, 
		channel1->_cvImage, 
		channel2->_cvImage, 
		channel3->_cvImage, 
		NULL
	);
}

Image *Image::mergeToMaximumWith(
	Image *anotherImage
) {
	Image *maximum = anotherImage->clone();
	cvMax(_cvImage, anotherImage->_cvImage, maximum->_cvImage);
	return maximum;
}

Image *Image::mergeChannelsToMaximum() {
	Image *channel1 = this->cloneJustDimensions(1, IPL_DEPTH_8U);
	Image *channel2 = this->cloneJustDimensions(1, IPL_DEPTH_8U);
	Image *channel3 = this->cloneJustDimensions(1, IPL_DEPTH_8U);

	this->splitTo(channel1, channel2, channel3);

	Image *max1and2 = channel1->mergeToMaximumWith(channel2);
	Image *maxAll = max1and2->mergeToMaximumWith(channel3);

	max1and2->release();
	channel1->release();
	channel2->release();
	channel3->release();

	return maxAll;
}
