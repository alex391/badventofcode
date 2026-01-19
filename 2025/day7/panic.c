#include <stdio.h>
#include <stdlib.h>
#include "ints.c"

#ifndef PANIC_C
#define PANIC_C

// Returns a pointer that's guaranteed to not be zero (if it returns)
void *panic_if_zero(void *p)
{
	if (!p) {
		// If debugging, put a breakpoint here:
		fprintf(stderr, "p is zero, something has gone terribly wrong!!");
		exit(1);
	}
	return p;
}

// Exit if i == bad
void panic_if_equal(i64 i, i64 bad)
{
	if (i == bad) {
		fprintf(stderr, "i is expected to not be " Pi64, bad);
		exit(1);
	}
}

// Exit if i > max_good
void panic_if_greater(i64 i, i64 max_good)
{
	if (i > max_good) {
		fprintf(stderr, "i is expected to not be greater than " Pi64, max_good);
		exit(1);
	}
}

// Same as above, but for comparing values which would overflow i64 
void panic_if_greater_unsigned(u64 i, u64 max_good)
{
	if (i > max_good) {
		fprintf(stderr, "i is expected to not be greater than " Pu64, max_good);
		exit(1);
	}
}

// Exit if i < min_good
void panic_if_less(i64 i, i64 min_good)
{
	if (i < min_good) {
		fprintf(stderr, "i is expected to not be less than " Pu64, min_good);
		exit(1);
	}
}

// Exit if i != good
void panic_if_not_equal(i64 i, i64 good)
{
	if (i != good) {
		fprintf(stderr, "i is expected to be " Pi64 ", but was " Pi64, good, i);
		exit(1);
	}
}

#endif
