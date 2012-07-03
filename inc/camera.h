#ifndef CAMERA_H
#define CAMERA_H

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "image.h"

class Camera {

	public:
		Camera();
		~Camera();
		static Camera *theDefaultCamera();

		CvCapture *getInput();
		int getFps();
		Image *getCurrentFrame();

		void setInput(CvCapture *cvCapture);

	private:
		static Camera *_theDefaultCamera;
		CvCapture *_input;
		Image *_currentFrame;

};

#endif
