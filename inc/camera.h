#ifndef CAMERA_H
#define CAMERA_H

#include <opencv/cv.h>
#include <opencv/highgui.h>

class Camera {

	public:
		Camera(CvCapture *cvCapture);
		CvCapture *cvCapture();
		static Camera *theDefaultCamera();
		void release();

	private:
		static Camera *_theDefaultCamera;
		CvCapture *_cvCapture;

};

#endif
