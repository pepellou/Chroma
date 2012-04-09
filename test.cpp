#include <igloo/igloo_alt.h>
#include "chroma.h"

using namespace igloo;

Describe(a_chroma) {

	Chroma chroma;

	void SetUp() {
	}

	It(has_a_name_that_can_be_set) {
		char *aName = "aName";
		chroma.setName(aName);
		Assert::That(chroma.name(), Equals(aName));
	}

	It(has_a_default_name) {
		Assert::That(chroma.name(), Equals("ChromaPrototype"));
	}

};

int main() { return TestRunner::RunAllTests(); }
