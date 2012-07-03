#include "video.h"

Video *Video::_theDefaultCamera = NULL;

Video::Video(
) {
	this->_input = NULL;
	this->_currentFrame = NULL;
}

void Video::setInput(
	CvCapture *cvCapture
) {
	this->_input = cvCapture;
}

CvCapture *Video::getInput(
) {
	return this->_input;
}

Video *Video::theDefaultCamera(
) {
	if (Video::_theDefaultCamera == NULL) {
		CvCapture *cvCapture = cvCreateCameraCapture(1);
		if (cvCapture == NULL)
			cvCapture = cvCreateCameraCapture(0);
		Video::_theDefaultCamera = new Video();
		Video::_theDefaultCamera->setInput(
			cvCapture
		);
	}
	return Video::_theDefaultCamera;
}

Video::~Video(
) {
	cvReleaseCapture(&_input);
}

Image *Video::getCurrentFrame(
) {
	IplImage *currentCvFrame = cvQueryFrame(_input);
	if (this->_currentFrame == NULL) {
		this->_currentFrame = new Image(
			currentCvFrame
		);
	} else {
		this->_currentFrame->setInput(currentCvFrame);
	}
	return this->_currentFrame;
}

int Video::getFps(
) {
	return ( int ) cvGetCaptureProperty( this->_input, CV_CAP_PROP_FPS );
}
