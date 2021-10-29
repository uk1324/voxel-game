#include <Error/Panic.hpp>

#include <cstdio>
#include <cstdlib>
#include <cstdarg>

void panicImplementation(const char* filename, int line, const char* format, ...)
{
	fprintf(stderr, "%s:%d ", filename, line);
	va_list args;
	va_start(args, format);
	vfprintf(stderr, format, args);
	va_end(args);
	exit(EXIT_FAILURE);
	fputc('\n', stderr);
}