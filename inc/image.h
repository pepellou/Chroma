#ifndef IMAGE_H
#define IMAGE_H

#include <opencv/cv.h>
#include <opencv/highgui.h>

#define TOP_LEFT 0
#define BOTTOM_LEFT 1

class Image {

	public:
		Image(IplImage *cvImage);
		Image(const char *filePath);
		IplImage *cvImage();
		void setCvImage(IplImage *cvImage);
		void release();
		bool isValid();
		Image *clone();
		Image *cloneJustDimensions(int num_channels);
		void cloneTo(Image *anotherImage);
		void flip();
		void resizeLike(Image *otherImage);
		int originPosition();
		void setOriginPosition(int position);

	private:
		IplImage *_cvImage;

};

#endif
