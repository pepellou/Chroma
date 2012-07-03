#include <igloo/igloo_alt.h>
#include "chroma.h"
#include "video.h"

using namespace igloo;
using namespace std;

Describe(a_chroma) {

	Chroma *chroma;

	Video *theInput;

	void SetUp() {
		theInput = new Video();
		theInput->setInput(cvCreateFileCapture(	
			"./tests/data/test.avi"
		));
		chroma = new Chroma(theInput);
	}

	void TearDown() {
		delete chroma;
	}

	It(has_a_name_that_can_be_set) {
		string aName = "aName";

		chroma->setName(aName);

		Assert::That(
			chroma->name(), 
			Is().EqualTo(aName)
		);
	}

	It(has_a_default_name) {
		Assert::That(
			chroma->name(), 
			Is().EqualTo("ChromaPrototype")
		);
	}

	It(has_an_input_that_can_be_set) {
		Video* aVideo = new Video();

		chroma->setInput(aVideo);

		Assert::That(
			chroma->getInput(), 
			Is().EqualTo(aVideo)
		);
	}

};
