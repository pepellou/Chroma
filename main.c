#include <opencv/cv.h>
#include <opencv/highgui.h>

CvCapture* g_capture = NULL;

IplImage* smooth(
	IplImage* in
) {
	IplImage* out = cvCreateImage(
		cvGetSize(in),
		IPL_DEPTH_8U,
		3
	);
	cvSmooth( in, out, CV_GAUSSIAN, 3, 3 );

	return( out );
};

IplImage* duplicate(
	IplImage* in
) {
	IplImage* out = cvCreateImage(
		cvSize(2*in->width, 2*in->height),
		IPL_DEPTH_8U,
		3
	);
	cvPyrUp(in, out);
	return( out );
};

void putStarPeak(
	IplImage* frame,
	CvPoint pt1,
	CvPoint pt2,
	CvPoint pt3,
	CvPoint pt4,
	CvScalar color
) {
	cvLine(frame, pt1, pt2, color, 1);
	cvLine(frame, pt2, pt3, color, 1);
	cvLine(frame, pt3, pt4, color, 1);
	cvLine(frame, pt4, pt1, color, 1);
	CvPoint *points = new CvPoint[4];
	points[0] = pt1;
	points[1] = pt2;
	points[2] = pt3;
	points[3] = pt4;
	cvFillConvexPoly(frame, points, 4, color); 
}

void putStar(
	IplImage* frame
) {
	CvScalar red = cvScalar(0,0,255,255);
	putStarPeak(frame, cvPoint(230, 92), cvPoint(254,159), cvPoint(225,160), cvPoint(207,147), red);
	putStarPeak(frame, cvPoint(268,156), cvPoint(320,162), cvPoint(279,201), cvPoint(260,186), red);
	putStarPeak(frame, cvPoint(244,224), cvPoint(274,224), cvPoint(306,278), cvPoint(240,246), red);
	putStarPeak(frame, cvPoint(184,214), cvPoint(204,219), cvPoint(214,249), cvPoint(158,280), red);
	putStarPeak(frame, cvPoint(132,176), cvPoint(188,162), cvPoint(194,182), cvPoint(160,196), red);
	CvFont *font = new CvFont();
	cvInitFont(font, CV_FONT_HERSHEY_SIMPLEX, 1.0, 1.0);
	cvPutText(frame, "Kroma", cvPoint(175,320), font, red);
}

void fill_croma_from_border(
	IplImage* frame,
	CvPoint border_point,
	CvScalar color,
	CvScalar threshold = cvScalar(4,4,4)
) {
	cvFloodFill(frame, border_point, color, threshold, threshold);
}

void sum_rgb( IplImage* src, IplImage* dst ) {
	IplImage* r = cvCreateImage( cvGetSize(src), IPL_DEPTH_8U, 1 );
	IplImage* g = cvCreateImage( cvGetSize(src), IPL_DEPTH_8U, 1 );
	IplImage* b = cvCreateImage( cvGetSize(src), IPL_DEPTH_8U, 1 );

	// Split image onto the color planes.
	cvSplit( src, r, g, b, NULL );

	// Temporary storage.
	IplImage* s = cvCreateImage( cvGetSize(src), IPL_DEPTH_8U, 1 );

	// Add equally weighted rgb values.
	cvAddWeighted( r, 1./3., g, 1./3., 0.0, s );
	cvAddWeighted( s, 2./3., b, 1./3., 0.0, s );

	// Truncate values above 100.
	int thres = 100;
	cvThreshold( s, dst, thres, thres, CV_THRESH_TOZERO );
	cvThreshold( s, dst, thres, thres, CV_THRESH_BINARY );
	cvReleaseImage(&r);
	cvReleaseImage(&g);
	cvReleaseImage(&b);
	cvReleaseImage(&s);
}

int main( int argc, char** argv ) {
	cvNamedWindow( "KarapaKroma", CV_WINDOW_AUTOSIZE );
	g_capture = cvCreateCameraCapture( 0 );
	IplImage* frame;
	IplImage* tempFrame;
	IplConvKernel *kernel = cvCreateStructuringElementEx(5, 5, 2, 2, CV_SHAPE_RECT);
	while(1) {
		frame = cvQueryFrame( g_capture );
		if( !frame ) break;
		putStar(frame);
		//cvMorphologyEx(frame, frame, tempFrame, kernel, CV_MOP_GRADIENT);
		//fill_croma_from_border(frame, cvPoint(639,0), cvScalar(0,255,0));
		//fill_croma_from_border(frame, cvPoint(0,0), cvScalar(0,255,0));
		IplImage* out = cvCreateImage( cvGetSize(frame), IPL_DEPTH_8U, 1 );
		sum_rgb(frame, out);
		cvShowImage( "KarapaKroma", out );
		char c = cvWaitKey(33);
		if( c == 27 ) break;
	}
	cvReleaseStructuringElement(&kernel);
	cvReleaseCapture( &g_capture );
	cvDestroyWindow( "KarapaKroma" );
}
