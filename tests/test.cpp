#include <igloo/igloo_alt.h>
#include "chroma.h"

using namespace igloo;
using namespace std;

Describe(a_chroma) {

	Chroma chroma;

	CvCapture *anAviFile;

	void SetUp() {
		anAviFile = cvCreateFileCapture("./tests/data/test.avi");
	}

	void TearDown() {
		cvReleaseCapture(&anAviFile);
	}

	It(has_a_name_that_can_be_set) {
		string aName = "aName";
		chroma.setName(aName);
		Assert::That(chroma.name(), Equals(aName));
	}

	It(has_a_default_name) {
		Assert::That(chroma.name(), Equals("ChromaPrototype"));
	}

	It(has_an_input_that_can_be_set) {
		Camera* anInput = new Camera(anAviFile);
		chroma.setInput(anInput);
		Assert::That(chroma.input(), Equals(anInput));
	}

	It(has_a_default_input) {
		Assert::That(chroma.input(), Equals(Camera::theDefaultCamera()));
	}

};

int main() { return TestRunner::RunAllTests(); }
