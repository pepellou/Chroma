#ifndef CHROMA_H
#define CHROMA_H

#include <string>
#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "camera.h"
#include "image.h"

class Chroma {

	public:
		Chroma();

		std::string name();
		Camera *input();

		void setName(std::string name);
		void setInput(Camera *input);

		int thisMethodShouldDie();

	private:
		std::string _name;
		Camera *_input;

		void putStar(Image* frame);
		void putStarPeak(
			Image* frame,
			CvPoint pt1,
			CvPoint pt2,
			CvPoint pt3,
			CvPoint pt4,
			CvScalar color
		);
		void fill_croma_from_border(
			Image* frame,
			CvPoint border_point,
			CvScalar color,
			CvScalar threshold = cvScalar(4,4,4)
		);
		void invert(Image* src, Image* dst);
		void sum_rgb(Image* src, Image* dst);
		void MaxCanales(Image *color, Image *gris);

};

#endif
