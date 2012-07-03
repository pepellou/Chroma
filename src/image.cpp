#include "image.h"
#include "messages.h"

Image::Image(
) {
	this->_input = NULL;
}

Image::Image(
	IplImage *input
) {
	this->setInput(input);
}

Image::Image(
	const char *filePath
) {
	this->setInput(filePath);
}

IplImage *Image::getInput(
) {
	return this->_input;
}

void Image::setInput(
	IplImage *input
) {
	this->_input = input;
}

void Image::setInput(
	const char *filePath
) {
	this->_input = cvLoadImage(filePath);
}

void Image::release(
) {
	try {
		cvReleaseImage(&_input);
	} catch (cv::Exception& e) {
		Messages::error("Warning: can't release image:");
	}
}

bool Image::isValid(
) {
	return (_input != NULL);
}

Image *Image::clone(
) {
	Image *imageClone = new Image();
	imageClone->setInput(cvCloneImage(_input));
	imageClone->setOriginPosition(TOP_LEFT); 
	return imageClone;
}

Image *Image::cloneJustDimensions(
	int num_channels,
	int depth
) {
	if (depth == -1)
		depth = _input->depth;
	if (num_channels == -1)
		num_channels = _input->nChannels;
	Image *theClone = new Image();
	theClone->setInput(
		cvCreateImage(
			cvGetSize(_input), 
			depth, 
			num_channels
		)
	);
	return theClone;
}

void Image::cloneTo(
	Image *anotherImage,
	Image *mask
) {
	if (mask == NULL) {
		cvCopy(
			_input, 
			anotherImage->_input
		);
	} else {
		cvCopy(
			_input, 
			anotherImage->_input, 
			mask->_input
		);
	}
}

void Image::flip(
) {
	cvFlip(_input);
}

void Image::resizeLike(
	Image *anotherImage
) {
	Image *tmp = anotherImage->clone();
	cvResize(_input, tmp->getInput()); 
	release();
	setInput(tmp->getInput());
}

int Image::getOriginPosition(
) {
	if (_input == NULL)
		return -1;
	return _input->origin;
}

void Image::setOriginPosition(
	int position
) {
	_input->origin = position;
}

Image *Image::distorsionWith(
	Image *anotherImage
) {
	Image *distorsion = anotherImage->clone();
	cvDiv(
		_input, 
		anotherImage->_input, 
		distorsion->_input
	);
	return distorsion;
}

void Image::storeDistorsionWith(
	Image *anotherImage,
	Image *toStore
) {
	cvDiv(
		_input, 
		anotherImage->_input, 
		toStore->_input
	);
}

Image *Image::differenceWith(
	Image *anotherImage
) {
	Image *difference = anotherImage->clone();
	cvAbsDiff(
		_input, 
		anotherImage->_input, 
		difference->_input
	);
	return difference;
}

void Image::storeDifferenceWith(
	Image *anotherImage,
	Image *toStore
) {
	cvAbsDiff(
		_input, 
		anotherImage->_input, 
		toStore->_input
	);
}

void Image::splitTo(
	Image *channel1,
	Image *channel2,
	Image *channel3
) {
	cvSplit(
		_input, 
		channel1->_input, 
		channel2->_input, 
		channel3->_input, 
		NULL
	);
}

void Image::mergeToMaximumWithAndStore(
	Image *anotherImage,
	Image *toStore
) {
	cvMax(_input, anotherImage->_input, toStore->_input);
}

Image *Image::mergeToMaximumWith(
	Image *anotherImage
) {
	Image *maximum = anotherImage->clone();
	cvMax(_input, anotherImage->_input, maximum->_input);
	return maximum;
}

Image *Image::mergeChannelsToMaximum(
) {
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

void Image::mergeChannelsToMaximumAndStore(
	Image *toStore
) {
	Image *channel1 = this->cloneJustDimensions(1, IPL_DEPTH_8U);
	Image *channel2 = this->cloneJustDimensions(1, IPL_DEPTH_8U);
	Image *channel3 = this->cloneJustDimensions(1, IPL_DEPTH_8U);

	this->splitTo(channel1, channel2, channel3);

	channel1->mergeToMaximumWithAndStore(channel2, toStore);
	toStore->mergeToMaximumWithAndStore(channel3, toStore);

	channel1->release();
	channel2->release();
	channel3->release();
}

void Image::mergeChannels(
	Image *toStore,
	float weight_channel1,
	float weight_channel2,
	float weight_channel3
) {
	Image *channel1 = this->cloneJustDimensions(1, IPL_DEPTH_8U);
	Image *channel2 = this->cloneJustDimensions(1, IPL_DEPTH_8U);
	Image *channel3 = this->cloneJustDimensions(1, IPL_DEPTH_8U);

	this->splitTo(channel1, channel2, channel3);

	cvAddWeighted(
		channel1->_input,
		weight_channel1,
		channel2->_input,
		weight_channel2,
		0,
		toStore->_input
	);
	cvAddWeighted(
		toStore->_input,
		weight_channel1 + weight_channel2,
		channel3->_input,
		weight_channel3,
		0,
		toStore->_input
	);

	channel1->release();
	channel2->release();
	channel3->release();
}

void Image::binarize(
	int threshold
) {
	cvThreshold(
		_input, 
		_input, 
		 threshold, 
		255, 
		CV_THRESH_BINARY
	);
}

void Image::negativize(
) {
	cvNot(_input, _input);
}

Image *Image::cleanIsolatedDots(
) {
	Image *clean = this->clone();
return clean;
	uchar *dataIn  = (uchar *) this->_input->imageData;
	uchar *dataOut = (uchar *) clean->_input->imageData;
	int y, x, c;
	int width = _input->width;
	int height = _input->height;
	int channels = _input->nChannels;
	int step = _input->widthStep;

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

Image *Image::duplicate(
) {
	IplImage *in = _input;
	IplImage* out = cvCreateImage(
		cvSize(2*in->width, 2*in->height),
		IPL_DEPTH_8U,
		in->nChannels
	);
	cvPyrUp(in, out);
	Image *theDuplicate = new Image();
	theDuplicate->setInput(out);
	return theDuplicate;
}
