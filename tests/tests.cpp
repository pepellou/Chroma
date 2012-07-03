#include <igloo/igloo_alt.h>

#include "messages.h"

using namespace igloo;

int main(
) { 
	Messages::enable = false;

	return TestRunner::RunAllTests(); 
}
