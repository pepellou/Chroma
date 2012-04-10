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
	// CAN REMOVE??
	Image* frame;
	while(1) {
		frame = new Image(cvQueryFrame( camera->cvCapture() ) );
		if( !(frame->cvImage()) ) break;

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
		cvShowImage("Salida", cop->duplicate()->cvImage());


		char c = cvWaitKey(33);
		if( c == 27 ) break;
	}
	camera->release();
	cvDestroyWindow( "KarapaKroma" );
	return 0;
}

void Chroma::putStarPeak(
	Image *frame,
	CvPoint pt1,
	CvPoint pt2,
	CvPoint pt3,
	CvPoint pt4,
	CvScalar color
) {
	cvLine(frame->cvImage(), pt1, pt2, color, 1);
	cvLine(frame->cvImage(), pt2, pt3, color, 1);
	cvLine(frame->cvImage(), pt3, pt4, color, 1);
	cvLine(frame->cvImage(), pt4, pt1, color, 1);
	CvPoint *points = new CvPoint[4];
	points[0] = pt1;
	points[1] = pt2;
	points[2] = pt3;
	points[3] = pt4;
	cvFillConvexPoly(frame->cvImage(), points, 4, color); 
}

void Chroma::putStar(
	Image *frame
) {
	CvScalar red = cvScalar(0,0,255,255);
	putStarPeak(frame, cvPoint(230, 92), cvPoint(254,159), cvPoint(225,160), cvPoint(207,147), red);
	putStarPeak(frame, cvPoint(268,156), cvPoint(320,162), cvPoint(279,201), cvPoint(260,186), red);
	putStarPeak(frame, cvPoint(244,224), cvPoint(274,224), cvPoint(306,278), cvPoint(240,246), red);
	putStarPeak(frame, cvPoint(184,214), cvPoint(204,219), cvPoint(214,249), cvPoint(158,280), red);
	putStarPeak(frame, cvPoint(132,176), cvPoint(188,162), cvPoint(194,182), cvPoint(160,196), red);
	CvFont *font = new CvFont();
	cvInitFont(font, CV_FONT_HERSHEY_SIMPLEX, 1.0, 1.0);
	cvPutText(frame->cvImage(), "Kroma", cvPoint(175,320), font, red);
}

void Chroma::sum_rgb(
	Image *src, 
	Image *dst 
) {
	Image* r = new Image(cvCreateImage( cvGetSize(src->cvImage()), IPL_DEPTH_8U, 1 ));
	Image* g = new Image(cvCreateImage( cvGetSize(src->cvImage()), IPL_DEPTH_8U, 1 ));
	Image* b = new Image(cvCreateImage( cvGetSize(src->cvImage()), IPL_DEPTH_8U, 1 ));
	Image* s_inv = new Image( cvCreateImage( cvGetSize(src->cvImage()), IPL_DEPTH_8U, 1 ));

	double w_r = 0.0;
	double w_g = 100.0;
	double w_b = 0.0;

	double w_sum = w_r + w_g + w_b;
	w_r /= w_sum;
	w_g /= w_sum;
	w_b /= w_sum;

	cvSplit( src->cvImage(), r, g, b, NULL );

	Image* s = new Image(cvCreateImage( cvGetSize(src->cvImage()), IPL_DEPTH_8U, 1 ));

	cvAddWeighted( r->cvImage(), w_r, g->cvImage(), w_g, 0.0, s->cvImage() );
	cvAddWeighted( s->cvImage(), w_r + w_g, b->cvImage(), w_b, 0.0, s->cvImage() );

	cvAdaptiveThreshold( 
		s->cvImage(), 
		s->cvImage(), 
		255, 
		CV_ADAPTIVE_THRESH_MEAN_C,
		CV_THRESH_BINARY,
		71,
		15		
	);

	s->invert(s, s_inv);

	cvAnd( r->cvImage(), s->cvImage(), r->cvImage() );
	cvAnd( g->cvImage(), s->cvImage(), g->cvImage() );
	cvAnd( b->cvImage(), s->cvImage(), b->cvImage() );

	cvMerge( r->cvImage(), g->cvImage(), b->cvImage(), NULL, dst->cvImage() );

	s_inv->release();
	r->release();
	g->release();
	b->release();
	s->release();
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

