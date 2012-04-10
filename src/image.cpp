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

void Image::binarize(
) {
	cvThreshold(
		_cvImage, 
		_cvImage, 
		40, 
		255, 
		CV_THRESH_BINARY
	);
}

void Image::negativize(
) {
	cvNot(_cvImage, _cvImage);
}

Image *Image::cleanIsolatedDots(
) {
	Image *clean = this->clone();
	uchar *dataIn  = (uchar *) this->_cvImage->imageData;
	uchar *dataOut = (uchar *) clean->_cvImage->imageData;
	int y, x, c;
	int width = _cvImage->width;
	int height = _cvImage->height;
	int channels = _cvImage->nChannels;
	int step = _cvImage->widthStep;

	int around_points[][2] = { {-1, -1}, {1, -1}, {1, 1},   {-1, 1} };
	int total_around = sizeof(around_points) / sizeof(int);
	int point;
	float maximum_not_same = 0.1 * total_around;

	for (y = 0; y < height; y++) {
		for (x = 0; x < width; x++) {
			for (c = 0; c < channels; c++) {
				int index = y * step
					+ x * channels + c;
				int new_color = dataIn[index];
				dataOut[index] = new_color;

				if (x < width - 1 && x > 0 && y < height - 1 && y > 0) {
					int not_same_around = 0;
					for (point = 0; not_same_around < maximum_not_same && point < total_around; point++) {
						int nx = x + around_points[point][0];
						int ny = y + around_points[point][1];
						if (nx < width && nx >= 0 && ny < height && ny >= 0) {
							int new_index = ny * step + nx * channels + c;
							if (dataIn[new_index] != new_color)
								not_same_around++;
						}
					}
					if (not_same_around == maximum_not_same)
						dataOut[index] = 255 - new_color;
				}
			}
		}
	}
	return clean;
}
