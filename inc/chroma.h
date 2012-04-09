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

	private:
		std::string _name;
		CvCapture *_input;

};

#endif
