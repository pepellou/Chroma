#ifndef CHROMA_H
#define CHROMA_H

#include <string>
#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "camera.h"

class Chroma {

	public:
		Chroma();

		std::string name();
		CvCapture *input();

		void setName(std::string name);
		void setInput(CvCapture *input);

		int thisMethodShouldDie();

	private:
		std::string _name;
		CvCapture *_input;

		IplImage *smooth(IplImage *in);
		IplImage *duplicate(IplImage *in);
		void putStar(IplImage* frame);
		void putStarPeak(
			IplImage* frame,
			CvPoint pt1,
			CvPoint pt2,
			CvPoint pt3,
			CvPoint pt4,
			CvScalar color
		);
		void fill_croma_from_border(
			IplImage* frame,
			CvPoint border_point,
			CvScalar color,
			CvScalar threshold = cvScalar(4,4,4)
		);
		void invert(IplImage* src, IplImage* dst);
		void sum_rgb(IplImage* src, IplImage* dst);
		void MaxCanales(IplImage *color, IplImage *gris);

};

#endif
