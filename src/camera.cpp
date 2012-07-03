#include "camera.h"

Camera *Camera::_theDefaultCamera = NULL;

Camera::Camera(
) {
	this->_input = NULL;
	this->_currentFrame = NULL;
}

void Camera::setInput(
	CvCapture *cvCapture
) {
	this->_input = cvCapture;
}

CvCapture *Camera::getInput(
) {
	return this->_input;
}

Camera *Camera::theDefaultCamera(
) {
	if (Camera::_theDefaultCamera == NULL) {
		CvCapture *cvCapture = cvCreateCameraCapture(1);
		if (cvCapture == NULL)
			cvCapture = cvCreateCameraCapture(0);
		Camera::_theDefaultCamera = new Camera();
		Camera::_theDefaultCamera->setInput(
			cvCapture
		);
	}
	return Camera::_theDefaultCamera;
}

void Camera::release(
) {
	cvReleaseCapture(&_input);
}

Image *Camera::getCurrentFrame(
) {
	IplImage *currentCvFrame = cvQueryFrame(_input);
	if (this->_currentFrame == NULL) {
		this->_currentFrame = new Image(
			currentCvFrame
		);
	} else {
		this->_currentFrame->setCvImage(currentCvFrame);
	}
	return this->_currentFrame;
}

int Camera::getFps(
) {
	return ( int ) cvGetCaptureProperty( this->_input, CV_CAP_PROP_FPS );
}
