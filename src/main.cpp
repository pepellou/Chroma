#include "chroma.h"

int main( int argc, char** argv ) {
	Chroma *chroma = new Chroma(
		Video::theDefaultCamera()
	);
	int error = chroma->mainLoop();
	delete chroma;
	return error;
}
