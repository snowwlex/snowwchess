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
	char* end_ptr;
	long int val = strtol(value.c_str(), &end_ptr, 10);
	if (*end_ptr) { end_ptr++; }
	return (int)val;

}



