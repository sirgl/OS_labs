#include <cstdio>
#include <cstring>
#include <string>
#include "tools.h"

std::string get_error(int error) {
	return std::string(strerror(error));
}