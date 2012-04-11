#ifndef CHROMA_H
#define CHROMA_H

#include <string>
#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "camera.h"
#include "image.h"
#include "window.h"

class Chroma {

	public:
		Chroma();

		std::string name();
		Camera *input();

		void setName(std::string name);
		void setInput(Camera *input);

		int thisMethodShouldDie();

		void release();

		void grabStaticScene();

	private:
		std::string _name;
		Camera *_input;
		Window *wInput;
		Window *wModel;
		Window *wMask;
		Window *wCleanMask;
		Window *wOutput;
		Image *staticScene;
		Image *background;

};

#endif
