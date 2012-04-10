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


	while(1) {
		Image *inputSignal = camera->grabCurrentFrame();

		if (inputSignal->originPosition() == BOTTOM_LEFT)
			inputSignal->flip();

		wEntrada->renderImage(inputSignal);

		if (camera->processedFrames() == 50) {
			inputSignal->cloneTo(staticScene);
		}
		wModelo->renderImage(staticScene);

		Image *difference = staticScene->differenceWith(inputSignal);
		Image *mask = difference->mergeChannelsToMaximum();

		difference->release();

		wDiferencia->renderImage(mask);

		mask->binarize();
		mask->negativize();
		background->cloneTo(inputSignal, mask);
		mask->release();

		wSalida->renderImage(inputSignal);

		char c = cvWaitKey(33);
		if( c == 27 ) break;
	}
	camera->release();
	cvDestroyWindow( "KarapaKroma" );
	return 0;
}
