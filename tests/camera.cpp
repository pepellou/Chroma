#include <igloo/igloo_alt.h>
#include "camera.h"

using namespace igloo;

Describe(a_camera) {

	Camera *camera;

	CvCapture *theInput;

	void SetUp() {
		theInput = cvCaptureFromAVI(
			"./tests/data/fentos_base.mov"
		);
		camera = new Camera();
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
		Camera *anotherCamera = new Camera();
		Assert::That(
			anotherCamera->getFps(), 
			Is().EqualTo(0)
		);
		Assert::That(
			anotherCamera->getInput(), 
			Is().EqualTo((CvCapture *) NULL)
		);
		delete anotherCamera;
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
		Camera *theDefaultCamera = Camera::theDefaultCamera();
		Camera *anotherInstance = Camera::theDefaultCamera();
		Assert::That(
			theDefaultCamera,
			Is().Not().EqualTo((Camera *) NULL)
		);
		Assert::That(
			anotherInstance,
			Is().EqualTo(theDefaultCamera)
		);
	}

};
