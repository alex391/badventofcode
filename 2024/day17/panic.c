#include <stdio.h>
#include <stdlib.h>

#ifndef PANIC_C
#define PANIC_C

// Returns a pointer that's guaranteed to not be zero
void *panic_if_zero(void *p)
{
	if (!p) {
		// If debugging, put a breakpoint here:
		fprintf(stderr, "p is zero, something has gone terribly wrong!!");
		exit(1);
	}
	return p;
}

#endif
