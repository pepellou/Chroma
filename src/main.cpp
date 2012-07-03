#include "chroma.h"

int main( int argc, char** argv ) {
	Chroma *chroma = new Chroma(
		Video::theDefaultCamera()
	);

	Video *fentos = new Video();
	fentos->setInput(
		cvCaptureFromAVI("./tests/data/fentos_base.mov")
	);
	chroma->addVideoLayer(fentos);
	chroma->setFps(fentos->getFps());

	int error = chroma->mainLoop();
	delete chroma;
	return error;
}
