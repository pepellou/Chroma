#ifndef WINDOW_H
#define WINDOW_H

#include <opencv/cv.h>
#include <opencv/highgui.h>

#include "image.h"

class Window {

	public:
		Window(char *name, int x, int y, int flags = 0);
		void renderImage(Image *image);
		void release();

	private:
		char *_name;

};

#endif
