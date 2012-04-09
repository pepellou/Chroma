#ifndef CHROMA_H
#define CHROMA_H

#include <string>

class Chroma {
	private:
		std::string _name;
	public:
		Chroma();
		void setName(std::string name);
		std::string name();
};

#endif
