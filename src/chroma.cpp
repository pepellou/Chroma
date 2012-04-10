#include "chroma.h"
#include "window.h"

using namespace std;

Chroma::Chroma(
) {
	this->_name = "ChromaPrototype";
	this->_input = Camera::theDefaultCamera();
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
	Window *wEntrada = new Window((char *) "Entrada", 1, 1);
	Window *wModelo = new Window((char *) "Modelo", 400, 1);
	Window *wDiferencia = new Window((char *) "Diferencia", 800, 1);
	Window *wSalida = new Window((char *) "Salida", 400, 350);

	Camera* camera = input();

	Image *staticScene = camera->grabStaticScene();

	Image *background = new Image("./tests/data/fondo.jpg");
	background->resizeLike(staticScene);

	while (true) {
		Image *inputSignal = camera->grabCurrentFrame();

		if (inputSignal->originPosition() == BOTTOM_LEFT)
			inputSignal->flip();


		if (camera->processedFrames() == 50) {
			inputSignal->cloneTo(staticScene);
		}


		Image *outputSignal = inputSignal->clone();

		Image *difference = staticScene->differenceWith(inputSignal);
		Image *mask = difference->mergeChannelsToMaximum();

		difference->release();

		mask->binarize();
		mask->negativize();
		background->cloneTo(outputSignal, mask);

		wEntrada->renderImage(inputSignal);
		wModelo->renderImage(staticScene);
		wDiferencia->renderImage(mask);
		wSalida->renderImage(outputSignal);

		mask->release();
		difference->release();
		outputSignal->release();

		char c = cvWaitKey(33);
		if( c == 27 ) break;
	}
	staticScene->release();
	background->release();
	camera->release();
	cvDestroyWindow( "KarapaKroma" );
	return 0;
}
