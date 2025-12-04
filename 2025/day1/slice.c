#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include "panic.c"
#include <ctype.h>

#ifndef SLICE_C
#define SLICE_C

// For reaitively short strings
// Unlike str, as long as all reads are done through data, and all indexes fit
// in uint8_t, all reads are safe, so reads are unchecked
union shortstr {
	struct {
		char s[UINT8_MAX]; // 255 bytes of char
		uint8_t len; // and 1 byte of len
	};
	char data[UINT8_MAX + 1]; // or, all 256 bytes (uint8_t)data[UINT8_MAX] gets you the len
};

// A string slice, never read or write more than len chars from s
// Use str_get and str_set to read and write rather than indexing into s
// Note intentional simalarity to slice below - str _usually_ owns it's memory,
// slice does not
struct str {
	char *s;
	uint32_t len;
};

// A slice that stores its elemnts in a diffrenet array
// no new for this because it's just calloc(1, sizeof(struct slice));
// and since it's only 8 bytes you should just pass it by copy usually
// indexes into this array from index to index + (len - 1) should be valid
struct slice {
	uint32_t index;
	uint32_t len;
};

size_t size_t_min(size_t a, size_t b)
{
	if (a < b) {
		return a;
	}
	return b;
}

// Convert null terminated to shortstr
union shortstr shortstr_from_null_terminated(const char *null_terminated)
{
	union shortstr new_shortstr = { 0 };
	strncpy(new_shortstr.data, null_terminated, sizeof(new_shortstr.data)); // the null will end up in len, which is fine
	size_t null_terminated_len = strlen(null_terminated);
	new_shortstr.len = size_t_min(UINT8_MAX, null_terminated_len);
	return new_shortstr;
}

// Return a str where s is a pointer to shortstr's s
struct str shortstr_to_str(union shortstr converting)
{
	return (struct str) { .s = converting.s, .len = converting.len };
}


// index into a slice of an array
// if i is >= indexed_len.len, return 0 instead
char checked_get_char(struct slice slice, const char *arr, uint32_t i)
{
	if (i < slice.len) {
		return arr[i + slice.index];
	}
	return 0;
}

// write into a slice of an array
// if i is >= index_len.len, do nothing instead
void checked_set_char(struct slice index_len, char *arr, uint32_t i, char value)
{
	if (i < index_len.len) {
		arr[i + index_len.index] = value;
	}
}

// print the characters in this slice
void slice_print(struct slice slice, char *arr)
{
	for (uint32_t i = 0; i < slice.len; i++) {
		putchar(checked_get_char(slice, arr, i));
	}
}

// allocate a new str with len 0 and an invalid s
struct str *str_new_empty()
{
	struct str *new_str = calloc(1, sizeof(struct str));
	panic_if_zero(new_str);
	return new_str;
}

// allocate a new str with all '\0' in s
struct str *str_new_cleared(uint32_t len) 
{
	struct str *cleared = str_new_empty();
	cleared->s = calloc(len, sizeof(char));
	panic_if_zero(cleared->s);
	cleared->len = len;
	return cleared;
}

// allocate a new strgiven a character buffer and a length
// the buffer should have at least len chars in it
// Note: consider using just 
// struct str s = { .s = (some buffer or sring constant), .len = (length of that buffer) }
struct str *str_new(const char *buff, uint32_t len)
{
	struct str *str = str_new_empty();
	str->s = malloc(len);
	panic_if_zero(str->s);
	memcpy(str->s, buff, len);
	str->len = len;
	return str;
}

// Get a new str from a null terminated char array
// If the string is longer than UINT32_MAX, then only the first UINT32_MAX chars will be copied into the str
// Note: especially for constant strings, consider just using
// struct str s = { .s = (your buffer), .len = (sizeof(your buffer))
struct str *str_new_from_null_terminated(const char *null_string)
{
	size_t len = strlen(null_string);
	if (len > UINT32_MAX) {
		len = UINT32_MAX;
	}
	return str_new(null_string, len);
}

// free freeing, and any of it's members
// Note: only usefull for strs that have been allocated with a function with new
// in the name
void str_free(struct str *freeing)
{
	if (freeing->s) {
		free(freeing->s);
	}
	free(freeing);
}

// Bounds checked get, returning 0 if the bounds check fails
// It's usually cheap and sometimes free to use this function instead of
// s->s[i], so always use it
char str_get(const struct str *s, uint32_t i)
{
	return checked_get_char((struct slice) { .index = 0, .len = s->len }, s->s, i);
}

// Checked set, do nothing if i is out of bounds
// It's usually cheap and sometimes free to use this function instead of
// s->s[i] = c, so always use it
void str_set(struct str *s, uint32_t i, char c)
{
	checked_set_char((struct slice) { .index = 0, .len = s->len }, s->s, i, c);
}

// Allocate a new char array with a '\0' at the end (at new_array[s->len])
char *str_to_new_null_terminated(struct str *s)
{
	char *null_terminated = malloc(s->len + 1);
	strncpy(null_terminated, s->s, s->len);
	return null_terminated;
}

// print a str
void str_print(struct str *s)
{
	slice_print((struct slice) { .index = 0, .len = s->len }, s->s);
}

// print a shortstr
void shortstr_print(union shortstr s)
{
	slice_print((struct slice) { .index = 0, .len = s.len}, s.s);
}
// Check if a contains the same characters as b
bool str_equal(const struct str *a, const struct str *b)
{
	if (a->len != b->len) {
		return false;
	}
	for (uint32_t i = 0; i < a->len; i++) {
		if (str_get(a, i) != str_get(b, i)) {
			return false;
		}

	}
	return true;
}

// Check if a contains the same characters as b
bool shortstr_equal(union shortstr a, union shortstr b)
{
	if (a.len != b.len) {
		return false;
	}
	for (uint8_t i = 0; i < a.len; i++) {
		if (a.s[i] != b.s[i]) {
			return false;
		}
	}
	return true;
}

// Parse str into an int32_t (undocumented (but defined) if the str isn't digits)
int32_t str_to_int32_t(struct str s)
{
	if (s.len == 0) {
		return 0;
	}
	char first_char = s.s[0];
	int8_t sign = 1;
	struct str value_part = s;
	if (first_char == '-') {
		if (s.len == 1) {
			return 0;
		}
		sign = -1;
		value_part = (struct str) { .s = s.s + 1, .len = s.len - 1 };
	}
	uint32_t value = 0;

	// "-123"
	// -1 * ( 1 * 100 + 2 * 10 + 3 * 1)
	
	// starts off at the 1s place, then goes to 10s place, etc
	uint32_t ns_place = 1;
	for (uint32_t i = value_part.len; i--;) {
		if (!isdigit(value_part.s[i])) {
			break;
		}
		value += (value_part.s[i] - '0') * ns_place;
		ns_place *= 10;
	}
	return value * sign;
}

#endif
