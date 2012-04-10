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

	Image *modelo = cop->clone();

	if (currentFrame->originPosition() == BOTTOM_LEFT) 
		modelo->flip();

	modelo->setOriginPosition(TOP_LEFT); 

	Image *background = new Image("./tests/data/fondo.jpg");
	background->resizeLike(cop);

	Image *difference = cop->clone();
	Image *mask = cop->cloneJustDimensions(1);

	Window *wEntrada = new Window((char *) "Entrada", 1, 1);
	Window *wModelo = new Window((char *) "Modelo", 400, 1);
	Window *wDiferencia = new Window((char *) "Diferencia", 800, 1);
	Window *wSalida = new Window((char *) "Salida", 400, 350);

	int numf= 0; 
	while(1) {
		camera->grabCurrentFrame();

		currentFrame->cloneTo(cop);
		if (currentFrame->originPosition() == BOTTOM_LEFT)
			cop->flip();

		wEntrada->renderImage(cop);

		numf++;
		if (numf==50) {
			cvCopy(cop->cvImage(), modelo->cvImage());
		}
		wModelo->renderImage(modelo);

		cvAbsDiff(modelo->cvImage(), cop->cvImage(), difference->cvImage());
		MaxCanales(difference, mask);
		wDiferencia->renderImage(mask);

		cvThreshold(mask->cvImage(), mask->cvImage(), 40, 255, CV_THRESH_BINARY);
		cvNot(mask->cvImage(), mask->cvImage());
		cvCopy(background->cvImage(), cop->cvImage(), mask->cvImage());
		wSalida->renderImage(cop);


		char c = cvWaitKey(33);
		if( c == 27 ) break;
	}
	camera->release();
	cvDestroyWindow( "KarapaKroma" );
	return 0;
}

void Chroma::MaxCanales(Image *color, Image *gris)
{
	Image *can1 = new Image(cvCreateImage(cvGetSize(gris->cvImage()), IPL_DEPTH_8U, 1));
	Image *can2 = new Image(cvCreateImage(cvGetSize(gris->cvImage()), IPL_DEPTH_8U, 1));
	cvSplit(color->cvImage(), gris->cvImage(), can1->cvImage(), can2->cvImage(), NULL);
	cvMax(can1->cvImage(), can2->cvImage(), can1->cvImage());
	cvMax(can1->cvImage(), gris->cvImage(), gris->cvImage());
	can1->release();
	can2->release();
}

