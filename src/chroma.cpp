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
	Image *img = new Image(cvQueryFrame(camera->cvCapture()));
	if (!img->cvImage()) { 
		camera->release();
		return -1;
	} 
	Image *cop = new Image(cvCloneImage(img->cvImage()));
	cop->cvImage()->origin= 0; 
	Image *modelo = new Image(cvCloneImage(cop->cvImage()));
	if (img->cvImage()->origin) 
		cvFlip(modelo->cvImage()); 
	modelo->cvImage()->origin= 0; 
	Image *tmp = new Image(cvCloneImage(cop->cvImage()));
	Image *nuevofondo = new Image(cvLoadImage("./tests/data/fondo.jpg"));
	cvResize(nuevofondo->cvImage(), tmp->cvImage()); 
	nuevofondo->release();
	nuevofondo -> setCvImage(tmp->cvImage());
	Image *dif = new Image(cvCloneImage(cop->cvImage()));
	Image *masc = new Image(cvCreateImage(cvGetSize(cop->cvImage()), cop->cvImage()->depth, 1));
	int numf= 0; 
	while(1) {
		cvQueryFrame( camera->cvCapture() );

		if (img->cvImage()->origin==0)
			cvCopy(img->cvImage(), cop->cvImage());
		else
			cvFlip(img->cvImage(), cop->cvImage());
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
		cvAbsDiff(modelo->cvImage(), cop->cvImage(), dif->cvImage());
		MaxCanales(dif, masc);
		cvNamedWindow("Diferencia", 0);
		cvMoveWindow("Diferencia", 800, 1);
		cvShowImage("Diferencia", masc->cvImage());
		cvThreshold(masc->cvImage(), masc->cvImage(), 40, 255, CV_THRESH_BINARY);
		cvNot(masc->cvImage(), masc->cvImage());
		cvCopy(nuevofondo->cvImage(), cop->cvImage(), masc->cvImage());
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

