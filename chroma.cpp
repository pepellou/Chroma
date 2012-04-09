#include "chroma.h"

Chroma::Chroma(
) {
	this->_name = "ChromaPrototype";
}

void Chroma::setName(
	char *name
) {
	this->_name = name;
}

char *Chroma::name(
) {
	return this->_name;
}
