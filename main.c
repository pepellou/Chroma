#include <opencv/cv.h>
#include <opencv/highgui.h>

CvCapture* g_capture = NULL;

IplImage* doPyrDown(
	IplImage* in,
	int filter = IPL_GAUSSIAN_5x5
) {
	assert( in->width%2 == 0 && in->height%2 == 0 );
	IplImage* out = cvCreateImage(
		cvSize( in->width/2, in->height/2 ),
		in->depth,
		in->nChannels
	);
	cvPyrDown( in, out );
	return( out );
};

int main( int argc, char** argv ) {
	cvNamedWindow( "KarapaKroma", CV_WINDOW_AUTOSIZE );
	g_capture = cvCreateCameraCapture( 0 );
	IplImage* frame;
	while(1) {
		frame = cvQueryFrame( g_capture );
		if( !frame ) break;
		cvShowImage( "KarapaKroma", doPyrDown(frame) );
		char c = cvWaitKey(33);
		if( c == 27 ) break;
	}
	cvReleaseCapture( &g_capture );
	cvDestroyWindow( "KarapaKroma" );
}
