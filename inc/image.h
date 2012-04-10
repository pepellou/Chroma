#ifndef IMAGE_H
#define IMAGE_H

#include <opencv/cv.h>
#include <opencv/highgui.h>

class Image {

	public:
		Image(IplImage *cvImage);
		IplImage *cvImage();
		void setCvImage(IplImage *cvImage);

	private:
		IplImage *_cvImage;

};

#endif
