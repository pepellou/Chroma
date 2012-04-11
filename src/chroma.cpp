#include "chroma.h"
#include "window.h"

using namespace std;

Chroma::Chroma(
) {
	this->_name = "ChromaPrototype";
	this->_input = Camera::theDefaultCamera();

	this->wInput = new Window((char *) "Entrada", 1, 1);
	this->wModel = new Window((char *) "Modelo", 400, 1);
	this->wMask = new Window((char *) "Diferencia", 800, 1);
	this->wCleanMask = new Window(
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
}

void Chroma::release(
) {
	this->wInput->release();
	this->wModel->release();
	this->wMask->release();
	this->wCleanMask->release();
	this->wOutput->release();
	this->_input->release();
	this->staticScene->release();
	this->background->release();
}

void Chroma::grabStaticScene(
) {
	Camera* camera = input();
	this->staticScene = camera->grabStaticScene();
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

// --- UNTESTED CODE FROM THIS POINT DOWN ------------



// --- LITTER FROM THIS POINT DOWN -------------------

int Chroma::thisMethodShouldDie(
) {
	Camera* camera = input();

	while (true) {
		Image *inputSignal = camera->grabCurrentFrame();

		if (inputSignal->originPosition() == BOTTOM_LEFT)
			inputSignal->flip();


		if (camera->processedFrames() == 50) {
			inputSignal->cloneTo(this->staticScene);
		}


		Image *outputSignal = inputSignal->clone();

		Image *difference = this->staticScene->differenceWith(inputSignal);
		Image *mask = difference->mergeChannelsToMaximum();

		mask->binarize();
		mask->negativize();

		Image *cleanMask = mask->cleanIsolatedDots();

		this->background->cloneTo(outputSignal, cleanMask);

		this->wInput->renderImage(inputSignal);
		this->wModel->renderImage(this->staticScene);
		this->wMask->renderImage(mask);
		this->wCleanMask->renderImage(cleanMask);
		this->wOutput->renderImage(outputSignal);

		mask->release();
		cleanMask->release();
		difference->release();
		outputSignal->release();

		char c = cvWaitKey(33);
		if( c == 27 ) break;
	}
	this->release();
	return 0;
}
