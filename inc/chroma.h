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

		int mainLoop();

		void release();

		void grabStaticScene();
		void copyInputToOutput();
		void grabInputSignal();
		void computeDifference();
		void applyBackgroundToOutput();
		void renderWindows();
		bool processKeys();

	private:
		std::string _name;
		Camera *_input;
		Window *wInput;
		Window *wModel;
		Window *wDifference;
		Window *wCleanDifference;
		Window *wOutput;
		Image *staticScene;
		Image *background;
		Image *inputSignal;
		Image *outputSignal;
		Image *difference;
		Image *cleanDifference;

};

#endif
