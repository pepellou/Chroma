#include "chroma.h"

int main( int argc, char** argv ) {
	Chroma *chroma = new Chroma(
		Camera::theDefaultCamera()
	);
	int error = chroma->mainLoop();
	delete chroma;
	return error;
}
