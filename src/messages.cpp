#include <iostream>
#include <stdarg.h>

#include "messages.h"

using namespace std;

bool Messages::enable = true;

void Messages::error(
	const char* message
) {
	forcePut(message);
	flushLine();
}

void Messages::info(
	const char* message
) {
	put(message);
	flushLine();
}

void Messages::info(
	const char* message,
	int value
) {
	put(message);
	put(value);
	flushLine();
}

void Messages::info(
	const char *message1, 
	int value1,
	const char *message2, 
	int value2,
	const char *message3, 
	int value3,
	const char *message4, 
	int value4,
	const char *message5
) {
	put(message1);
	put(value1);
	put(message2);
	put(value2);
	put(message3);
	put(value3);
	put(message4);
	put(value4);
	put(message5);
	flushLine();
}

void Messages::info(
	const char *message1, 
	float value1,
	const char *message2, 
	float value2,
	const char *message3, 
	float value3,
	const char *message4, 
	float value4,
	const char *message5, 
	float value5,
	const char *message6, 
	float value6,
	const char *message7, 
	float value7,
	const char *message8, 
	float value8,
	const char *message9,
	float value9,
	const char *message10
) {
	put(message1);
	put(value1);
	put(message2);
	put(value2);
	put(message3);
	put(value3);
	put(message4);
	put(value4);
	put(message5);
	put(value5);
	put(message6);
	put(value6);
	put(message7);
	put(value7);
	put(message8);
	put(value8);
	put(message9);
	put(value9);
	put(message10);
	flushLine();
}

void Messages::forcePut(
	const char *text
) {
	cout << text;
}

void Messages::put(
	const char *text
) {
	if (Messages::enable) {
		cout << text;
	}
}

void Messages::put(
	int value
) {
	if (Messages::enable) {
		cout << value;
	}
}

void Messages::put(
	float value
) {
	if (Messages::enable) {
		cout << value;
	}
}

void Messages::flushLine(
) {
	if (Messages::enable) {
		cout << endl;
		flush(cout);
	}
}
