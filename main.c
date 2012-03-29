#include <opencv/highgui.h>

CvCapture* g_capture = NULL;

int main( int argc, char** argv ) {
	cvNamedWindow( "KarapaKroma", CV_WINDOW_AUTOSIZE );
	g_capture = cvCreateCameraCapture( 0 );
	IplImage* frame;
	while(1) {
		frame = cvQueryFrame( g_capture );
		if( !frame ) break;
		cvShowImage( "KarapaKroma", frame );
		char c = cvWaitKey(33);
		if( c == 27 ) break;
	}
	cvReleaseCapture( &g_capture );
	cvDestroyWindow( "KarapaKroma" );
}
