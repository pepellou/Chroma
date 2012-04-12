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
		void copyStaticToModel();
		void copyInputToOutput();
		void grabInputSignal();
		void computeDifference();
		void computeDistorsion();
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
		void cropOutput();
		void increaseWeightRed();
		void decreaseWeightRed();
		void increaseWeightGreen();
		void decreaseWeightGreen();
		void increaseWeightBlue();
		void decreaseWeightBlue();

		void operateOnModel();
		void operateOnOutput();
		void operateOnDifference();

	private:
		std::string _name;
		Camera *_input;
		Window *wInput;
		Window *wModel;
		Window *wDifference;
		Window *wDistorsion;
		Window *wMask;
		Window *wOutput;
		Image *staticScene;
		Image *model;
		Image *background;
		Image *inputSignal;
		Image *outputSignal;
		Image *difference;
		Image *distorsion;
		Image *mask;

		Image *currentOperationImage;

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
		void outputRGBWeights();
		void adjustCropPosition();

		void adjustOutput();
		void adjustDifference();
		void adjustModel();
		void adjustImage(Image *image, float r, float g, float b);

		float weight_output_r;
		float weight_output_g;
		float weight_output_b;
		float weight_model_r;
		float weight_model_g;
		float weight_model_b;
		float weight_difference_r;
		float weight_difference_g;
		float weight_difference_b;

		Camera *video_fentos;
		void applyFentosToOutput();

		int fps;
};

#endif
