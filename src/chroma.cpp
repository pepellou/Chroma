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
	Camera* camera = input();
	Image *currentFrame = camera->grabCurrentFrame();

	if (!currentFrame->isValid()) { 
		camera->release();
		return -1;
	} 

	Image *cop = currentFrame->clone();
	cop->setOriginPosition(TOP_LEFT); 

	Image *staticScene = cop->clone();

	if (currentFrame->originPosition() == BOTTOM_LEFT) 
		staticScene->flip();

	staticScene->setOriginPosition(TOP_LEFT); 

	Image *background = new Image("./tests/data/fondo.jpg");
	background->resizeLike(cop);

	Window *wEntrada = new Window((char *) "Entrada", 1, 1);
	Window *wModelo = new Window((char *) "Modelo", 400, 1);
	Window *wDiferencia = new Window((char *) "Diferencia", 800, 1);
	Window *wSalida = new Window((char *) "Salida", 400, 350);

	while(1) {
		camera->grabCurrentFrame();

		currentFrame->cloneTo(cop);
		if (currentFrame->originPosition() == BOTTOM_LEFT)
			cop->flip();

		wEntrada->renderImage(cop);

		if (camera->processedFrames() == 50) {
			cop->cloneTo(staticScene);
		}
		wModelo->renderImage(staticScene);

		Image *difference = staticScene->differenceWith(cop);
		Image *mask = difference->mergeChannelsToMaximum();

		difference->release();

		wDiferencia->renderImage(mask);

		cvThreshold(mask->cvImage(), mask->cvImage(), 40, 255, CV_THRESH_BINARY);
		cvNot(mask->cvImage(), mask->cvImage());
		cvCopy(background->cvImage(), cop->cvImage(), mask->cvImage());
		mask->release();

		wSalida->renderImage(cop);

		char c = cvWaitKey(33);
		if( c == 27 ) break;
	}
	camera->release();
	cvDestroyWindow( "KarapaKroma" );
	return 0;
}
