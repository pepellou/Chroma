#include "camera.h"

Camera *Camera::_theDefaultCamera = NULL;

Camera::Camera(
	CvCapture *cvCapture
) {
	this->_cvCapture = cvCapture;
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

