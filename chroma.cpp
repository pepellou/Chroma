#include "chroma.h"

using namespace std;

Chroma::Chroma(
) {
	this->_name = "ChromaPrototype";
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
