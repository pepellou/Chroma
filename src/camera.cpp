#include "camera.h"

Camera *Camera::_theDefaultCamera = NULL;

Camera::Camera(
	CvCapture *cvCapture
) {
	this->_cvCapture = cvCapture;
	this->_currentFrame = NULL;
}

CvCapture *Camera::cvCapture(
) {
	return this->_cvCapture;
}

Camera *Camera::theDefaultCamera(
) {
	if (Camera::_theDefaultCamera == NULL) {
		Camera::_theDefaultCamera = 
			new Camera(cvCreateCameraCapture(0));
	}
	return Camera::_theDefaultCamera;
}

void Camera::release(
) {
	cvReleaseCapture(&_cvCapture);
}

Image *Camera::grabCurrentFrame(
) {
	if (this->_currentFrame == NULL) {
		this->_currentFrame = new Image(
			cvQueryFrame(_cvCapture)
		);
	} else {
		cvQueryFrame(_cvCapture);
	}
	return this->_currentFrame;
}
