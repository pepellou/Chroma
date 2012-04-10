#ifndef IMAGE_H
#define IMAGE_H

#include <opencv/cv.h>
#include <opencv/highgui.h>

class Image {

	public:
		Image(IplImage *cvImage);
		IplImage *cvImage();
		void setCvImage(IplImage *cvImage);
		void release();
		Image *smooth();
		Image *duplicate();
		void fillColor(
			CvPoint startPoint,
			CvScalar color,
			CvScalar threshold
		);
		Image *invert();

	private:
		IplImage *_cvImage;

};

#endif
