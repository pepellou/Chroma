#include <igloo/igloo_alt.h>
#include "image.h"

using namespace igloo;

Describe(an_image) {

	Image image;

	void SetUp() {
		image.setInput("./tests/data/fondo.jpg");
	}

	void TearDown() {
	}

	It(has_input) {
		Assert::That(
			image.getInput(),
			Is().Not().EqualTo((IplImage *) NULL)
		);
	}

	It(has_origin_position) {
		Assert::That(
			image.getOriginPosition(),
			Is().EqualTo(TOP_LEFT)
		);
	}

	It(has_default_values) {
		Image anotherImage;
		Assert::That(
			anotherImage.getInput(),
			Is().EqualTo((IplImage *) NULL)
		);
		Assert::That(
			anotherImage.getOriginPosition(),
			Is().EqualTo(-1)
		);
	}

};
