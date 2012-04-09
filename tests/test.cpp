#include <igloo/igloo_alt.h>
#include "chroma.h"

using namespace igloo;
using namespace std;

Describe(a_chroma) {

	Chroma chroma;

	const char *anAviFile;

	void SetUp() {
		anAviFile = "./tests/data/test.avi";
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
		CvCapture* anInput = cvCreateFileCapture(anAviFile);
		chroma.setInput(anInput);
		Assert::That(chroma.input(), Equals(anInput));
	}

	It(has_a_default_input) {
		Assert::That(chroma.input(), Equals(Camera::theDefaultCamera()));
	}

};

int main() { return TestRunner::RunAllTests(); }
