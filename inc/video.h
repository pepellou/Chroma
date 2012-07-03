#ifndef CAMERA_H
#define CAMERA_H

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "image.h"

class Video {

	public:
		Video();
		~Video();
		static Video *theDefaultCamera();

		CvCapture *getInput();
		int getFps();
		Image *getCurrentFrame();

		void setInput(CvCapture *cvCapture);

	private:
		static Video *_theDefaultCamera;
		CvCapture *_input;
		Image *_currentFrame;

};

#endif
