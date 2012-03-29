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
	cvSmooth( in, out, CV_GAUSSIAN, 17, 17 );

	return( out );
};

int main( int argc, char** argv ) {
	cvNamedWindow( "KarapaKroma", CV_WINDOW_AUTOSIZE );
	g_capture = cvCreateCameraCapture( 0 );
	IplImage* frame;
	while(1) {
		frame = cvQueryFrame( g_capture );
		if( !frame ) break;
		cvShowImage( "KarapaKroma", smooth(frame) );
		char c = cvWaitKey(33);
		if( c == 27 ) break;
	}
	cvReleaseCapture( &g_capture );
	cvDestroyWindow( "KarapaKroma" );
}
