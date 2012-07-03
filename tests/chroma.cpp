#include <igloo/igloo_alt.h>
#include "chroma.h"

using namespace igloo;
using namespace std;

Describe(a_chroma) {

	Chroma chroma;

	CvCapture *theInput;

	void SetUp() {
		theInput = cvCreateFileCapture(	
			"./tests/data/test.avi"
		);
	}

	void TearDown() {
		cvReleaseCapture(&theInput);
	}

	It(has_a_name_that_can_be_set) {
		string aName = "aName";

		chroma.setName(aName);

		Assert::That(
			chroma.name(), 
			Is().EqualTo(aName)
		);
	}

	It(has_a_default_name) {
		Assert::That(
			chroma.name(), 
			Is().EqualTo("ChromaPrototype")
		);
	}

	It(has_an_input_that_can_be_set) {
		Camera* aCamera = new Camera();

		chroma.setInput(aCamera);

		Assert::That(
			chroma.input(), 
			Is().EqualTo(aCamera)
		);
	}

	It(has_a_default_input) {
		Assert::That(
			chroma.input(), 
			Is().EqualTo(Camera::theDefaultCamera())
		);
	}

};
