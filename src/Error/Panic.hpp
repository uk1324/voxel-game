#pragma once

void panicImplementation(const char* filename, int line, const char* format, ...);

#define panic(format, ...) \
	panicImplementation(__FILE__, __LINE__, format, __VA_ARGS__)