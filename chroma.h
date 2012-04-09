#ifndef CHROMA_H
#define CHROMA_H

class Chroma {
	private:
		char *_name;
	public:
		Chroma();
		void setName(char *name);
		char *name();
};

#endif
