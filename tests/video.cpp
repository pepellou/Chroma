#include <igloo/igloo_alt.h>
#include "video.h"

using namespace igloo;

Describe(a_video) {

	Video *camera;

	CvCapture *theInput;

	void SetUp() {
		theInput = cvCaptureFromAVI(
			"./tests/data/fentos_base.mov"
		);
		camera = new Video();
		camera->setInput(theInput);
	}

	void TearDown() {
		delete camera;
	}

	It(has_fps) {
		Assert::That(
			camera->getFps(), 
			Is().EqualTo(25)
		);
	}

	It(has_input) {
		Assert::That(
			camera->getInput(), 
			Is().EqualTo(theInput)
		);
	}

	It(has_default_values) {
		Video *anotherVideo = new Video();
		Assert::That(
			anotherVideo->getFps(), 
			Is().EqualTo(0)
		);
		Assert::That(
			anotherVideo->getInput(), 
			Is().EqualTo((CvCapture *) NULL)
		);
		delete anotherVideo;
	}

	It(returns_current_frame) {
		Assert::That(
			camera->getCurrentFrame(),
			Is().Not().EqualTo((Image *) NULL)
		);
		Assert::That(
			camera->getCurrentFrame()->isValid(),
			Is().True()
		);
	}

	It(gives_a_unique_default_camera) {
		Video *theDefaultCamera = Video::theDefaultCamera();
		Video *anotherInstance = Video::theDefaultCamera();
		Assert::That(
			theDefaultCamera,
			Is().Not().EqualTo((Video *) NULL)
		);
		Assert::That(
			anotherInstance,
			Is().EqualTo(theDefaultCamera)
		);
	}

};
