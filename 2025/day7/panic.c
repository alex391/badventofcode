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

// Same as above, but print a line number also
void panic_if_less_line(i64 i, i64 min_good, i32 line)
{
	if (i < min_good) {
		fprintf(stderr, "i is expected to not be less than " Pu64 " line %d", min_good, line);
		exit(1);
	}
}

// Exit if i == bad, also print line number
void panic_if_equal_line(i64 i, i64 bad, i32 line)
{
	if (i == bad) {
		fprintf(stderr, "i is expected to not be " Pi64 " line %d", bad, line);
		exit(1);
	}
}

// Exit if i > max_good, also print line number
void panic_if_greater_line(i64 i, i64 max_good, i32 line)
{
	if (i > max_good) {
		fprintf(stderr, "i is expected to not be greater than " Pi64 " line %d", max_good, line);
		exit(1);
	}
}
// Exit always
[[noreturn]]
void panic_unconditionally() {
	fprintf(stderr, "This should be unreachable!!");
	exit(1);
}

// Exit always, and print line
[[noreturn]]
void panic_unconditionally_line(i32 line) {
	fprintf(stderr, "line %d should be unreachable!!", line);
	exit(1);
}
#endif
