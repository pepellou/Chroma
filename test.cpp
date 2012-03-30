#include <igloo/igloo_alt.h>
#include "chroma.h"

using namespace igloo;

Describe(a_chroma) {

	void SetUp() {
		chroma.setName("Name");
	}

	It(has_a_name) {
		Assert::That(chroma.name(), Equals("Blah"));
	}

	Chroma chroma;

};

int main() { return TestRunner::RunAllTests(); }
