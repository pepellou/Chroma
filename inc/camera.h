#ifndef CAMERA_H
#define CAMERA_H

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "image.h"

class Camera {

	public:
		Camera(CvCapture *cvCapture);
		CvCapture *cvCapture();
		static Camera *theDefaultCamera();
		void release();
		Image *grabCurrentFrame();
		Image *grabStaticScene();
		long processedFrames();

	private:
		static Camera *_theDefaultCamera;
		CvCapture *_cvCapture;
		Image *_currentFrame;
		long _processed_frames;

};

#endif
