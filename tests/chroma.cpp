#include <igloo/igloo_alt.h>
#include "chroma.h"
#include "camera.h"

using namespace igloo;
using namespace std;

Describe(a_chroma) {

	Chroma *chroma;

	Camera *theInput;

	void SetUp() {
		theInput = new Camera();
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
		Camera* aCamera = new Camera();

		chroma->setInput(aCamera);

		Assert::That(
			chroma->getInput(), 
			Is().EqualTo(aCamera)
		);
	}

};
