#include "chroma.h"

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

	int numf= 0; 
	while(1) {
		camera->grabCurrentFrame();

		currentFrame->cloneTo(cop);
		if (currentFrame->originPosition() == BOTTOM_LEFT)
			cop->flip();

		cvNamedWindow("Entrada", 0);
		cvMoveWindow("Entrada", 1, 1);
		cvShowImage("Entrada", cop->cvImage());
		numf++;
		if (numf==50) {
			cvCopy(cop->cvImage(), modelo->cvImage());
		}
		cvNamedWindow("Modelo", 0);
		cvMoveWindow("Modelo", 400, 1);
		cvShowImage("Modelo", modelo->cvImage());
		cvAbsDiff(modelo->cvImage(), cop->cvImage(), difference->cvImage());
		MaxCanales(difference, mask);
		cvNamedWindow("Diferencia", 0);
		cvMoveWindow("Diferencia", 800, 1);
		cvShowImage("Diferencia", mask->cvImage());
		cvThreshold(mask->cvImage(), mask->cvImage(), 40, 255, CV_THRESH_BINARY);
		cvNot(mask->cvImage(), mask->cvImage());
		cvCopy(background->cvImage(), cop->cvImage(), mask->cvImage());
		cvNamedWindow("Salida", 0);
		cvMoveWindow("Salida", 400, 350);
		cvShowImage("Salida", cop->cvImage());


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

