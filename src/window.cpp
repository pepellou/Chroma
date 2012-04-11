#include "window.h"

Window::Window(
	char *name,
	int x,
	int y,
	int flags
) {
	cvNamedWindow(name, flags);
	cvMoveWindow(name, x, y);
	this->_name = name;
}

void Window::renderImage(
	Image *image
) {
	cvShowImage(this->_name, image->cvImage());
}

void Window::release(
) {
	cvDestroyWindow(this->_name);
}
