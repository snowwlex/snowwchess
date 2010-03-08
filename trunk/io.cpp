/*
 * io.cpp
 *
 *
 *      Author: snowwlex
 */

#include <cstdlib>
#include <string>
#include "io.h"

int makeInt(std::string value) {
	char* endPtr;
	long int val = strtol(value.c_str(), &endPtr, 10);
	if (*endPtr) { endPtr++; }
	return (int)val;

}



