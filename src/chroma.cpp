#include <iostream>

#include "chroma.h"
#include "window.h"

using namespace std;

Chroma::Chroma(
) {
	this->binarize_threshold = 40;

	this->_name = "ChromaPrototype";
	this->_input = Camera::theDefaultCamera();

	this->wInput = new Window((char *) "Entrada", 1, 1);
	this->wModel = new Window((char *) "Modelo", 400, 1);
	this->wDifference = new Window((char *) "Diferencia", 800, 1);
	this->wCleanDifference = new Window(
		(char *) "Diferencia limpia", 
		1, 
		350
	);
	this->wOutput = new Window(
		(char *) "Salida", 
		400, 
		350, 
		CV_WINDOW_AUTOSIZE
	);

	grabStaticScene();
	this->background = new Image("./tests/data/fondo.jpg");
	this->background->resizeLike(this->staticScene);
	this->maxCropWidth = this->background->cvImage()->width;
	this->maxCropHeight = this->background->cvImage()->height;
	this->resetCropDimensions();
	this->inputSignal = NULL;
	this->outputSignal = NULL;
	this->difference = NULL;
	this->cleanDifference = NULL;
}

void Chroma::increaseBinarizationThreshold(
) {
	if (this->binarize_threshold < 255)
		this->binarize_threshold++;
	this->outputBinarizationThreshold();
}

void Chroma::decreaseBinarizationThreshold(
) {
	if (this->binarize_threshold > 0)
		this->binarize_threshold--;
	this->outputBinarizationThreshold();
}

void Chroma::increaseCropWidth(
) {
	if (this->crop_width < this->maxCropWidth)
		this->crop_width++;
	this->adjustCropPosition();
	this->outputCropDimensions();
}

void Chroma::decreaseCropWidth(
) {
	if (this->crop_width > 0)
		this->crop_width--;
	this->adjustCropPosition();
	this->outputCropDimensions();
}

void Chroma::increaseCropHeight(
) {
	if (this->crop_height < this->maxCropHeight)
		this->crop_height++;
	this->adjustCropPosition();
	this->outputCropDimensions();
}

void Chroma::decreaseCropHeight(
) {
	if (this->crop_height > 0)
		this->crop_height--;
	this->adjustCropPosition();
	this->outputCropDimensions();
}

void Chroma::moveCropLeft(
) {
	this->crop_x--;
	this->adjustCropPosition();
	this->outputCropDimensions();
}

void Chroma::moveCropRight(
) {
	this->crop_x++;
	this->adjustCropPosition();
	this->outputCropDimensions();
}

void Chroma::moveCropUp(
) {
	this->crop_y--;
	this->adjustCropPosition();
	this->outputCropDimensions();
}

void Chroma::moveCropDown(
) {
	this->crop_y++;
	this->adjustCropPosition();
	this->outputCropDimensions();
}

void Chroma::adjustCropPosition(
) {
	if (this->crop_x < 0)
		this->crop_x = 0;
	if (this->crop_y < 0)
		this->crop_y = 0;
	if (this->crop_x > this->maxCropWidth - this->crop_width)
		this->crop_x = this->maxCropWidth - this->crop_width;
	if (this->crop_y > this->maxCropHeight - this->crop_height)
		this->crop_y = this->maxCropHeight - this->crop_height;
}

void Chroma::resetCropDimensions(
) {
	this->crop_width = this->maxCropWidth;
	this->crop_height = this->maxCropHeight;
	this->crop_x = 0;
	this->crop_y = 0;
	this->outputCropDimensions();
}

void Chroma::outputCropDimensions(
) {
	cout 
		<< "Crop dimensions = " 
		<< this->crop_width << " x " << this->crop_height
		<< " placed at (" << this->crop_x << ", " << this->crop_y << ")"
		<< endl;
	flush(cout);
}

void Chroma::outputBinarizationThreshold(
) {
	cout 
		<< "Binarization threshold = " 
		<< this->binarize_threshold 
		<< endl;
	flush(cout);
}

void Chroma::outputHelp(
) {
	cout 
		<< "COMMANDS " << endl
		<< "           <: Decrease crop width" << endl 
		<< "           >: Increase crop width" << endl 
		<< "           =: Set default crop dimensions" << endl 
		<< "           -: Decrease crop height" << endl 
		<< "           +: Increase crop height" << endl 
		<< "           ?: This help" << endl
		<< "           G: Grab static scene" << endl 
		<< "     <RePag>: Increase binarization threshold" << endl 
		<< "     <AvPag>: Decrease binarization threshold" << endl 
		<< endl << "   <CURSORS>: Move crop" << endl
		<< endl << "       <ESC>: Exit program" << endl 
	;
	flush(cout);
}

void Chroma::release(
) {
	this->wInput->release();
	this->wModel->release();
	this->wDifference->release();
	this->wCleanDifference->release();
	this->wOutput->release();
	this->_input->release();
	this->staticScene->release();
	this->background->release();
	if (this->outputSignal != NULL)
		this->outputSignal->release();
	if (this->difference != NULL)
		this->difference->release();
	if (this->cleanDifference != NULL)
		this->cleanDifference->release();
}

void Chroma::grabStaticScene(
) {
	Camera* camera = input();
	this->staticScene = camera->grabStaticScene();
}

void Chroma::copyInputToOutput(
) {
	if (this->outputSignal != NULL)
		this->inputSignal->cloneTo(this->outputSignal);
	else
		this->outputSignal = this->inputSignal->clone();
}

void Chroma::computeDifference(
) {
	if (this->difference != NULL) {
		this->staticScene->storeDifferenceWith(
			this->inputSignal, 
			this->difference
		);
	} else {
		this->difference = 
			this->staticScene->differenceWith(
				this->inputSignal
			);
		this->cleanDifference = 
			this->difference->cloneJustDimensions(1);
	}

	this->difference->mergeChannelsToMaximumAndStore(
		this->cleanDifference
	);
	this->cleanDifference->binarize(this->binarize_threshold);
	this->cleanDifference->negativize();
	//this->cleanDifference->cleanIsolatedDots();
}

void Chroma::applyBackgroundToOutput(
) {
	this->background->cloneTo(
		this->outputSignal, 
		this->cleanDifference
	);
}

void Chroma::renderWindows(
) {
	this->wInput->renderImage(this->inputSignal);
	this->wModel->renderImage(this->staticScene);
	this->wDifference->renderImage(this->difference);
	this->wCleanDifference->renderImage(this->cleanDifference);
	this->wOutput->renderImage(this->outputSignal);
}

bool Chroma::processKeys(
) {
	char key = cvWaitKey(33);

	switch (key) {
		case 45:
			this->decreaseCropHeight();
			break;
		case '?':
			this->outputHelp();
			break;
		case 60:
			this->decreaseCropWidth();
			break;
		case 43:
			this->increaseCropHeight();
			break;
		case 'g':
		case 'G':
			this->grabStaticScene();
			break;
		case 62:
			this->increaseCropWidth();
			break;
		case 61:
			this->resetCropDimensions();
			break;
		case 'U':
			this->increaseBinarizationThreshold();
			break;
		case 'V':
			this->decreaseBinarizationThreshold();
			break;
		case 'Q':
			this->moveCropLeft();
			break;
		case 'R':
			this->moveCropUp();
			break;
		case 'T':
			this->moveCropDown();
			break;
		case 'S':
			this->moveCropRight();
			break;
		case 27: return false;
	}

	return true;
}

void Chroma::grabInputSignal(
) {
	Camera* camera = input();
	this->inputSignal = camera->grabCurrentFrame();
	if (this->inputSignal->originPosition() == BOTTOM_LEFT)
		this->inputSignal->flip();
}

void Chroma::setName(
	string name
) {
	this->_name = name;
}

string Chroma::name(
) {
	return this->_name;
}

void Chroma::setInput(
	Camera *input
) {
	this->_input = input;
}

Camera *Chroma::input(
) {
	return this->_input;
}

int Chroma::mainLoop(
) {
	bool running = true;

	while (running) {

		this->grabInputSignal();

		this->copyInputToOutput();

		this->computeDifference();

		this->applyBackgroundToOutput();

		this->renderWindows();

		running = this->processKeys();

	}
	this->release();
	return 0;
}
