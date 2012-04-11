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

		void increaseBinarizationThreshold();
		void decreaseBinarizationThreshold();
		void increaseCropWidth();
		void decreaseCropWidth();
		void increaseCropHeight();
		void decreaseCropHeight();
		void resetCropDimensions();
		void moveCropLeft();
		void moveCropRight();
		void moveCropUp();
		void moveCropDown();

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
		int crop_width;
		int crop_height;
		int maxCropHeight;
		int maxCropWidth;
		int crop_x;
		int crop_y;

		int binarize_threshold;
		void outputBinarizationThreshold();
		void outputHelp();
		void outputCropDimensions();
		void adjustCropPosition();

};

#endif
