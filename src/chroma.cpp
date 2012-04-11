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
		1200, 
		1
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
	this->inputSignal = NULL;
	this->outputSignal = NULL;
	this->difference = NULL;
	this->cleanDifference = NULL;
}

void Chroma::incrementBinarizationThreshold(
) {
	if (this->binarize_threshold < 255)
		this->binarize_threshold++;
	this->outputBinarizationThreshold();
}

void Chroma::decrementBinarizationThreshold(
) {
	if (this->binarize_threshold > 0)
		this->binarize_threshold--;
	this->outputBinarizationThreshold();
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
		<< "     S: Grab static scene" << endl 
		<< "     X: Increment binarization threshold" << endl 
		<< "     Z: Decrement binarization threshold" << endl 
		<< "     H: This help" << endl << endl
		<< " <ESC>: Exit program" << endl 
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
		case 27: 
			return false;
		case 's':
		case 'S':
			this->grabStaticScene();
			break;
		case 'x':
		case 'X':
			this->incrementBinarizationThreshold();
			break;
		case 'z':
		case 'Z':
			this->decrementBinarizationThreshold();
			break;
		case 'h':
		case 'H':
			this->outputHelp();
			break;
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
