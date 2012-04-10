#include "camera.h"

Camera *Camera::_theDefaultCamera = NULL;

Camera::Camera(
	CvCapture *cvCapture
) {
	this->_cvCapture = cvCapture;
	this->_currentFrame = NULL;
	this->_processed_frames = 0;
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
	IplImage *currentCvFrame = cvQueryFrame(_cvCapture);
	if (this->_currentFrame == NULL) {
		this->_currentFrame = new Image(
			currentCvFrame
		);
	}
	this->_processed_frames++;
	return this->_currentFrame;
}

long Camera::processedFrames(
) {
	return this->_processed_frames;
}

