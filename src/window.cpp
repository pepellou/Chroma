#include "window.h"

Window::Window(
	char *name,
	int x,
	int y
) {
	cvNamedWindow(name, 0);
	cvMoveWindow(name, x, y);
	this->_name = name;
}

void Window::renderImage(
	Image *image
) {
	cvShowImage(this->_name, image->cvImage());
}