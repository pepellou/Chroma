#ifndef CAMERA_H
#define CAMERA_H

#include <opencv/cv.h>
#include <opencv/highgui.h>

class Camera {

	public:
		static CvCapture *theDefaultCamera();

	private:
		static CvCapture *_theDefaultCamera;

};

#endif
