#include "camera.h"

CvCapture *Camera::_theDefaultCamera = NULL;

CvCapture *Camera::theDefaultCamera(
) {
	if (Camera::_theDefaultCamera == NULL) {
		Camera::_theDefaultCamera = 
			cvCreateCameraCapture(0);
	}
	return Camera::_theDefaultCamera;
}

