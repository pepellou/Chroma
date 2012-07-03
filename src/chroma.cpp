#include <iostream>

#include "chroma.h"
#include "window.h"

using namespace std;

Chroma::Chroma(
) {
	this->binarize_threshold = 40;

	this->video_fentos = new Camera();
	this->video_fentos->setInput(cvCaptureFromAVI("./tests/data/fentos_base.mov"));

	this->fps = this->video_fentos->getFps();
	cout << "FPS = " << fps << endl;
	flush(cout);

	this->_name = "ChromaPrototype";
	this->_input = Camera::theDefaultCamera();

	this->wInput = new Window((char *) "INPUT", 1, 1);
	this->wModel = new Window((char *) "MODEL", 400, 1);
	this->wDistorsion = new Window((char *) "DISTORSION", 1, 700);
	this->wDifference = new Window((char *) "DIFFERENCE", 800, 1);
	this->wMask = new Window(
		(char *) "MASK", 
		1, 
		350
	);
	this->wOutput = new Window(
		(char *) "OUTPUT", 
		400, 
		350, 
		CV_WINDOW_AUTOSIZE
	);

	this->inputSignal = NULL;
	this->outputSignal = NULL;
	this->difference = NULL;
	this->distorsion = NULL;
	this->mask = NULL;
	this->model = NULL;
	this->staticScene = NULL;

	grabStaticScene();

	this->background = new Image();
	this->background->setInput("./tests/data/fondo.jpg");
	this->background->resizeLike(this->staticScene);
	this->maxCropWidth = this->background->getInput()->width;
	this->maxCropHeight = this->background->getInput()->height;
	this->resetCropDimensions();

	this->operateOnModel();

	this->weight_difference_r = 1.0;
	this->weight_difference_g = 1.0;
	this->weight_difference_b = 1.0;
	this->weight_output_r = 1.0;
	this->weight_output_g = 1.0;
	this->weight_output_b = 1.0;
	this->weight_model_r = 1.0;
	this->weight_model_g = 1.0;
	this->weight_model_b = 1.0;
}

void Chroma::operateOnDifference(
) {
	this->currentOperationImage = this->difference;
	cout << "Current operation on DIFFERENCE" << endl;
}

void Chroma::operateOnModel(
) {
	this->currentOperationImage = this->model;
	cout << "Current operation on MODEL" << endl;
}

void Chroma::operateOnOutput(
) {
	this->currentOperationImage = this->outputSignal;
	cout << "Current operation on OUTPUT" << endl;
}

void Chroma::increaseBinarizationThreshold(
) {
	if (this->binarize_threshold < 255)
		this->binarize_threshold++;
	this->outputBinarizationThreshold();
}

void Chroma::decreaseBinarizationThreshold(
) {
	if (this->binarize_threshold > 0)
		this->binarize_threshold--;
	this->outputBinarizationThreshold();
}

void Chroma::increaseCropWidth(
) {
	if (this->crop_width < this->maxCropWidth)
		this->crop_width++;
	this->adjustCropPosition();
	this->outputCropDimensions();
}

void Chroma::decreaseCropWidth(
) {
	if (this->crop_width > 0)
		this->crop_width--;
	this->adjustCropPosition();
	this->outputCropDimensions();
}

void Chroma::increaseCropHeight(
) {
	if (this->crop_height < this->maxCropHeight)
		this->crop_height++;
	this->adjustCropPosition();
	this->outputCropDimensions();
}

void Chroma::decreaseCropHeight(
) {
	if (this->crop_height > 0)
		this->crop_height--;
	this->adjustCropPosition();
	this->outputCropDimensions();
}

void Chroma::moveCropLeft(
) {
	this->crop_x--;
	this->adjustCropPosition();
	this->outputCropDimensions();
}

void Chroma::moveCropRight(
) {
	this->crop_x++;
	this->adjustCropPosition();
	this->outputCropDimensions();
}

void Chroma::moveCropUp(
) {
	this->crop_y--;
	this->adjustCropPosition();
	this->outputCropDimensions();
}

void Chroma::moveCropDown(
) {
	this->crop_y++;
	this->adjustCropPosition();
	this->outputCropDimensions();
}

void Chroma::adjustCropPosition(
) {
	if (this->crop_x < 0)
		this->crop_x = 0;
	if (this->crop_y < 0)
		this->crop_y = 0;
	if (this->crop_x > this->maxCropWidth - this->crop_width)
		this->crop_x = this->maxCropWidth - this->crop_width;
	if (this->crop_y > this->maxCropHeight - this->crop_height)
		this->crop_y = this->maxCropHeight - this->crop_height;
}

void Chroma::resetCropDimensions(
) {
	this->crop_width = this->maxCropWidth;
	this->crop_height = this->maxCropHeight;
	this->crop_x = 0;
	this->crop_y = 0;
	this->outputCropDimensions();
}

void Chroma::increaseWeightRed(
) {
	if (this->currentOperationImage == NULL) {
		cout << "Not selected an operation image" << endl;
	}
	
	if (this->currentOperationImage == this->difference) {
		if (this->weight_difference_r < 1.0)
			this->weight_difference_r += 0.1;
	}

	if (this->currentOperationImage == this->model) {
		if (this->weight_model_r < 1.0)
			this->weight_model_r += 0.1;
	}

	if (this->currentOperationImage == this->outputSignal) {
		if (this->weight_output_r < 1.0)
			this->weight_output_r += 0.1;
	}

	this->outputRGBWeights();
}

void Chroma::decreaseWeightRed(
) {
	if (this->currentOperationImage == NULL) {
		cout << "Not selected an operation image" << endl;
	}
	
	if (this->currentOperationImage == this->difference) {
		if (this->weight_difference_r > 0.0)
			this->weight_difference_r -= 0.1;
	}

	if (this->currentOperationImage == this->model) {
		if (this->weight_model_r > 0.0)
			this->weight_model_r -= 0.1;
	}

	if (this->currentOperationImage == this->outputSignal) {
		if (this->weight_output_r > 0.0)
			this->weight_output_r -= 0.1;
	}

	this->outputRGBWeights();
}

void Chroma::increaseWeightGreen(
) {
	if (this->currentOperationImage == NULL) {
		cout << "Not selected an operation image" << endl;
	}
	
	if (this->currentOperationImage == this->difference) {
		if (this->weight_difference_g < 1.0)
			this->weight_difference_g += 0.1;
	}

	if (this->currentOperationImage == this->model) {
		if (this->weight_model_g < 1.0)
			this->weight_model_g += 0.1;
	}

	if (this->currentOperationImage == this->outputSignal) {
		if (this->weight_output_g < 1.0)
			this->weight_output_g += 0.1;
	}

	this->outputRGBWeights();
}

void Chroma::decreaseWeightGreen(
) {
	if (this->currentOperationImage == NULL) {
		cout << "Not selected an operation image" << endl;
	}
	
	if (this->currentOperationImage == this->difference) {
		if (this->weight_difference_g > 0.0)
			this->weight_difference_g -= 0.1;
	}

	if (this->currentOperationImage == this->model) {
		if (this->weight_model_g > 0.0)
			this->weight_model_g -= 0.1;
	}

	if (this->currentOperationImage == this->outputSignal) {
		if (this->weight_output_g > 0.0)
			this->weight_output_g -= 0.1;
	}

	this->outputRGBWeights();
}

void Chroma::increaseWeightBlue(
) {
	if (this->currentOperationImage == NULL) {
		cout << "Not selected an operation image" << endl;
	}
	
	if (this->currentOperationImage == this->difference) {
		if (this->weight_difference_b < 1.0)
			this->weight_difference_b += 0.1;
	}

	if (this->currentOperationImage == this->model) {
		if (this->weight_model_b < 1.0)
			this->weight_model_b += 0.1;
	}

	if (this->currentOperationImage == this->outputSignal) {
		if (this->weight_output_b < 1.0)
			this->weight_output_b += 0.1;
	}

	this->outputRGBWeights();
}

void Chroma::decreaseWeightBlue(
) {
	if (this->currentOperationImage == NULL) {
		cout << "Not selected an operation image" << endl;
	}
	
	if (this->currentOperationImage == this->difference) {
		if (this->weight_difference_b > 0.0)
			this->weight_difference_b -= 0.1;
	}

	if (this->currentOperationImage == this->model) {
		if (this->weight_model_b > 0.0)
			this->weight_model_b -= 0.1;
	}

	if (this->currentOperationImage == this->outputSignal) {
		if (this->weight_output_b > 0.0)
			this->weight_output_b -= 0.1;
	}

	this->outputRGBWeights();
}

void Chroma::adjustImage(
	Image *image, 
	float r, 
	float g, 
	float b
) {
	Image *channel1 = image->cloneJustDimensions(1, IPL_DEPTH_8U);
	Image *channel2 = image->cloneJustDimensions(1, IPL_DEPTH_8U);
	Image *channel3 = image->cloneJustDimensions(1, IPL_DEPTH_8U);

	image->splitTo(channel1, channel2, channel3);

	cvScale(channel1->getInput(), channel1->getInput(), r);
	cvScale(channel2->getInput(), channel2->getInput(), g);
	cvScale(channel3->getInput(), channel3->getInput(), b);

	cvMerge(
		channel1->getInput(),
		channel2->getInput(),
		channel3->getInput(),
		NULL,
		image->getInput()
	);

	channel1->release();
	channel2->release();
	channel3->release();
}

void Chroma::adjustOutput(
) {
	this->adjustImage(
		this->outputSignal,
		this->weight_output_r,
		this->weight_output_g,
		this->weight_output_b
	);
}

void Chroma::adjustModel(
) {
	this->adjustImage(
		this->model,
		this->weight_model_r,
		this->weight_model_g,
		this->weight_model_b
	);
}

void Chroma::adjustDifference(
) {
	this->adjustImage(
		this->difference,
		this->weight_difference_r,
		this->weight_difference_g,
		this->weight_difference_b
	);
}

void Chroma::cropOutput(
) {
	cvRectangle(
		this->inputSignal->getInput(),
		cvPoint(this->crop_x, this->crop_y),
		cvPoint(this->crop_x + this->crop_width, this->crop_y + this->crop_height),
		cvScalar(0x00, 0x00, 0xff)
	);
	
}

void Chroma::outputCropDimensions(
) {
	cout 
		<< "Crop dimensions = " 
		<< this->crop_width << " x " << this->crop_height
		<< " placed at (" << this->crop_x << ", " << this->crop_y << ")"
		<< endl;
	flush(cout);
}

void Chroma::outputBinarizationThreshold(
) {
	cout 
		<< "Binarization threshold = " 
		<< this->binarize_threshold 
		<< endl;
	flush(cout);
}

void Chroma::outputRGBWeights(
) {
	cout
		<< "RGB weights: MODEL [ "
		<< this->weight_model_r << ", "
		<< this->weight_model_g << ", "
		<< this->weight_model_b << " ], DIFFERENCE [ "
		<< this->weight_difference_r << ", "
		<< this->weight_difference_g << ", "
		<< this->weight_difference_b << " ], OUTPUT [ "
		<< this->weight_output_r << ", "
		<< this->weight_output_g << ", "
		<< this->weight_output_b << " ]"
		<< endl;
	flush(cout);
}

void Chroma::outputHelp(
) {
	cout 
		<< "COMMANDS " << endl
		<< "           <: Decrease crop width" << endl 
		<< "           >: Increase crop width" << endl 
		<< "           =: Set default crop dimensions" << endl 
		<< "           -: Decrease crop height" << endl 
		<< "           +: Increase crop height" << endl 
		<< "         1/2: Decrease/increase RED weight" << endl 
		<< "         3/4: Decrease/increase GREEN weight" << endl 
		<< "         5/6: Decrease/increase BLUE weight" << endl 
		<< "           ?: This help" << endl
		<< "           G: Grab static scene" << endl 
		<< "           D: Operate on DIFFERENCE" << endl 
		<< "           M: Operate on MODEL" << endl 
		<< "           O: Operate on OUTPUT" << endl 
		<< "     <RePag>: Increase binarization threshold" << endl 
		<< "     <AvPag>: Decrease binarization threshold" << endl 
		<< endl << "   <CURSORS>: Move crop" << endl
		<< endl << "       <ESC>: Exit program" << endl 
	;
	flush(cout);
}

void Chroma::release(
) {
	this->wInput->release();
	this->wModel->release();
	this->wDifference->release();
	this->wDistorsion->release();
	this->wMask->release();
	this->wOutput->release();
	this->_input->release();
	this->staticScene->release();
	this->model->release();
	this->background->release();
	if (this->outputSignal != NULL)
		this->outputSignal->release();
	if (this->difference != NULL)
		this->difference->release();
	if (this->distorsion != NULL)
		this->distorsion->release();
	if (this->mask != NULL)
		this->mask->release();
}

void Chroma::grabStaticScene(
) {
	Camera* camera = input();
	if (this->staticScene != NULL) {
		this->staticScene->release();
	}
	this->staticScene = camera->getCurrentFrame()->clone();
}

void Chroma::copyStaticToModel(
) {
	if (this->model == NULL) {
		this->model = this->staticScene->clone();
	} else {
		this->staticScene->cloneTo(this->model);
	}
}

void Chroma::copyInputToOutput(
) {
	if (this->outputSignal != NULL)
		this->inputSignal->cloneTo(this->outputSignal);
	else
		this->outputSignal = this->inputSignal->clone();
}

void Chroma::computeDistorsion(
) {
	if (this->distorsion != NULL) {
		this->inputSignal->storeDistorsionWith(
			this->staticScene,
			this->distorsion 
		);
	} else {
		this->distorsion = 
			this->inputSignal->distorsionWith(
				this->staticScene
			);
	}
	cvSetImageROI(this->distorsion->getInput(), cvRect(0, 0, 20, 20));
	CvScalar theDistorsion = cvAvg(this->distorsion->getInput());
	cvResetImageROI(this->distorsion->getInput());
	cout << "Distorsion = [ " 
		<< theDistorsion.val[0] << ", "
		<< theDistorsion.val[1] << ", "
		<< theDistorsion.val[2] << ", "
		<< theDistorsion.val[3] << "]" << endl;
	uchar *data = (uchar *) this->model->getInput()->imageData;
	int i, j, k;
	int width = this->model->getInput()->width;
	int height = this->model->getInput()->height;
	int channels = this->model->getInput()->nChannels;
	int step = this->model->getInput()->widthStep;
	for(i=0;i<height;i++)
		for(j=0;j<width;j++)
			for(k=0;k<channels;k++)
				data[i*step+j*channels+k]=data[i*step+j*channels+k] * theDistorsion.val[k];
}

void Chroma::computeDifference(
) {
	if (this->difference != NULL) {
		this->model->storeDifferenceWith(
			this->inputSignal, 
			this->difference
		);
	} else {
		this->difference = 
			this->model->differenceWith(
				this->inputSignal
			);
		this->mask = 
			this->difference->cloneJustDimensions(1);
	}

	this->difference->mergeChannels(
		this->mask,
		this->weight_difference_r,
		this->weight_difference_g,
		this->weight_difference_b
	);
	this->mask->binarize(this->binarize_threshold);
	this->mask->negativize();
	//this->mask->cleanIsolatedDots();
}

void Chroma::applyBackgroundToOutput(
) {
	this->background->cloneTo(
		this->outputSignal, 
		this->mask
	);
}

void Chroma::applyFentosToOutput(
) {
	Image *frame = this->video_fentos->getCurrentFrame();
	if (frame != NULL) {
		Image *fentos = frame->clone();
		fentos->resizeLike(this->staticScene);
		Image *fentosMask = fentos->cloneJustDimensions(1, IPL_DEPTH_8U);
		cvInRangeS(
			fentos->getInput(),
			cvScalar(1, 1, 1, 0),
			cvScalar(255, 255, 255, 255),
			fentosMask->getInput()
		);
		fentos->cloneTo(
			this->outputSignal,
			fentosMask
		);
		fentos->release();
		fentosMask->release();
	}
}

void Chroma::renderWindows(
) {
	this->wInput->renderImage(this->inputSignal);
	this->wModel->renderImage(this->model);
	this->wDifference->renderImage(this->difference);
	//this->wDistorsion->renderImage(this->distorsion);
	this->wMask->renderImage(this->mask);
	this->wOutput->renderImage(this->outputSignal);
}

bool Chroma::processKeys(
) {
	int speed = 3;
	char key = cvWaitKey( 1000 * speed / this->fps / speed );

	switch (key) {
		case 45:
			this->decreaseCropHeight();
			break;
		case '?':
			this->outputHelp();
			break;
		case 60:
			this->decreaseCropWidth();
			break;
		case 43:
			this->increaseCropHeight();
			break;
		case 'g':
		case 'G':
			this->grabStaticScene();
			break;
		case 62:
			this->increaseCropWidth();
			break;
		case 61:
			this->resetCropDimensions();
			break;
		case 'U':
			this->increaseBinarizationThreshold();
			break;
		case 'V':
			this->decreaseBinarizationThreshold();
			break;
		case 'Q':
			this->moveCropLeft();
			break;
		case 'R':
			this->moveCropUp();
			break;
		case 'T':
			this->moveCropDown();
			break;
		case 'S':
			this->moveCropRight();
			break;
		case '1':
			this->decreaseWeightRed();
			break;
		case '2':
			this->increaseWeightRed();
			break;
		case '3':
			this->decreaseWeightGreen();
			break;
		case '4':
			this->increaseWeightGreen();
			break;
		case '5':
			this->decreaseWeightBlue();
			break;
		case '6':
			this->increaseWeightBlue();
			break;
		case 'd':
		case 'D':
			this->operateOnDifference();
			break;
		case 'm':
		case 'M':
			this->operateOnModel();
			break;
		case 'o':
		case 'O':
			this->operateOnOutput();
			break;
		case 27: return false;
	}

	return true;
}

void Chroma::grabInputSignal(
) {
	Camera* camera = input();
	this->inputSignal = camera->getCurrentFrame();
	if (this->inputSignal->getOriginPosition() == BOTTOM_LEFT)
		this->inputSignal->flip();
}

void Chroma::setName(
	string name
) {
	this->_name = name;
}

string Chroma::name(
) {
	return this->_name;
}

void Chroma::setInput(
	Camera *input
) {
	this->_input = input;
}

Camera *Chroma::input(
) {
	return this->_input;
}

int Chroma::mainLoop(
) {
	bool running = true;
	while (running) {
		this->grabInputSignal();
		this->copyInputToOutput();

		this->copyStaticToModel();
		this->adjustModel();
		//this->computeDistorsion();

		this->computeDifference();
		this->adjustDifference();

		this->applyBackgroundToOutput();
		this->applyFentosToOutput();
		this->cropOutput();
		this->adjustOutput();

		this->renderWindows();

		running = this->processKeys();
	}
	this->release();
	return 0;
}
