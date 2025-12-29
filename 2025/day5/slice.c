#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "panic.c"
#include <ctype.h>
#include "vec.c"
#include <limits.h>
#include <assert.h>
#include <stdckdint.h>
#include "ints.c"

#ifndef SLICE_C
#define SLICE_C

// -INT64_MIN
constexpr u64 POSITIVE_INT64_MIN = (9223372036854775808ull);
// The number of digits including the minus sign of INT64_MIN (because llabs will fail!)
constexpr u8 INT64_MIN_LENGTH = (20);

struct vec *vec_new(usize item_size);
// For reaitively short strings
// Unlike str, as long as all reads are done through data, and all indexes fit
// in u8, all reads are safe, so reads are unchecked
union shortstr {
	struct {
		c8 s[UINT8_MAX]; // 255 bytes of char
		u8 len; // and 1 byte of len
	};
	c8 data[UINT8_MAX + 1]; // or, all 256 bytes (u8)data[UINT8_MAX] gets you the len
};

// A string slice, never read or write more than len chars from s
// Use str_get and str_set to read and write rather than indexing into s
// Note intentional simalarity to slice below - str can own it's memory,
// slice does not
struct str {
	c8 *s;
	u32 len;
};

// A slice that stores its elemnts in a diffrenet array
// no new for this because it's just calloc(1, sizeof(struct slice));
// and since it's only 8 bytes you should just pass it by copy usually
// indexes into this array from index to index + (len - 1) should be valid
struct slice {
	u32 index;
	u32 len;
};

void vec_push_slice(struct vec *vec, struct slice slice);

// Convert null terminated to shortstr
union shortstr shortstr_from_null_terminated(const c8 *null_terminated)
{
	union shortstr new_shortstr = { 0 };
	strncpy(new_shortstr.data, null_terminated, sizeof(new_shortstr.data)); // the null will end up in len, which is fine
	usize len = strlen(new_shortstr.data); // Just this once, data is null terminated
	new_shortstr.len = len;
	return new_shortstr;
}

// Return a str where s is a pointer to shortstr's s
struct str shortstr_to_str(union shortstr converting)
{
	return (struct str) { .s = converting.s, .len = converting.len };
}


// index into a slice of an array of char
// if i is >= indexed_len.len, return 0 instead
char checked_get_char(struct slice slice, const char *arr, u32 i)
{
	if (i < slice.len) {
		return arr[i + slice.index];
	}
	return 0;
}

// write into a slice of an array of char
// if i is >= index_len.len, do nothing instead
void checked_set_char(struct slice index_len, c8 *arr, u32 i, c8 value)
{
	if (i < index_len.len) {
		arr[i + index_len.index] = value;
	}
}

// print the characters in this slice
void slice_print(struct slice slice, c8 *arr)
{
	for (u32 i = 0; i < slice.len; i++) {
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
struct str *str_new_cleared(u32 len) 
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
struct str *str_new(const c8 *buff, u32 len)
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
	usize len = strlen(null_string);
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
c8 str_get(const struct str *s, u32 i)
{
	return checked_get_char((struct slice) { .index = 0, .len = s->len }, s->s, i);
}

// Checked set, do nothing if i is out of bounds
// It's usually cheap and sometimes free to use this function instead of
// s->s[i] = c, so always use it
void str_set(struct str *s, u32 i, char c)
{
	checked_set_char((struct slice) { .index = 0, .len = s->len }, s->s, i, c);
}

// Allocate a new char array with a '\0' at the end (at new_array[s->len])
char *str_to_new_null_terminated(struct str *s)
{
	c8 *null_terminated = malloc(s->len + 1);
	strncpy(null_terminated, s->s, s->len);
	return null_terminated;
}

// Get str from slice and a char array
struct str slice_to_str(struct slice slice, char *array)
{
	return (struct str) { .s = array + slice.index, .len = slice.len };
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
b8 str_equal(const struct str *a, const struct str *b)
{
	if (a->len != b->len) {
		return false;
	}
	for (u32 i = 0; i < a->len; i++) {
		if (str_get(a, i) != str_get(b, i)) {
			return false;
		}

	}
	return true;
}

// Check if a contains the same characters as b
b8 shortstr_equal(union shortstr a, union shortstr b)
{
	if (a.len != b.len) {
		return false;
	}
	for (u8 i = 0; i < a.len; i++) {
		if (a.s[i] != b.s[i]) {
			return false;
		}
	}
	return true;
}


// Parse str into an int64_t (undocumented (but defined) if the str isn't digits)
i64 str_to_i64(struct str s)
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
	uint64_t value = 0;

	// "-123"
	// -1 * ( 1 * 100 + 2 * 10 + 3 * 1)
	
	// starts off at the 1s place, then goes to 10s place, etc
	uint64_t ns_place = 1;
	for (uint32_t i = value_part.len; i--;) {
		if (!isdigit(value_part.s[i])) {
			break;
		}
		value += (value_part.s[i] - '0') * ns_place;
		ns_place *= 10;
	}
	if (sign > 0) {
#if NDEBUG
		if (value > INT64_MAX) {
			return 0;
		}
#else
		panic_if_greater_unsigned(value, INT64_MAX);
#endif
	}
	else {
#if NDEBUG
		if (value > POSITIVE_INT64_MIN) {
			return 0;
		}
#else
		panic_if_greater_unsigned(value, POSITIVE_INT64_MIN);
#endif

	}
	return value * sign;
}

i32 str_to_i32_t(struct str s)
{

	int64_t converted = str_to_i64(s);
#if NDEBUG
	if (converted > INT32_MAX || converted < INT32_MIN) {
		return 0;
	}
#else
	panic_if_greater(converted, INT32_MAX);
	panic_if_less(converted, INT32_MIN);
#endif
	return converted;
}

// Returns the number of digits in i (so 1 returns 1, -1 returns 2, 10 returns 2, etc)
u8 i32_digits(i32 i)
{
	i64 abs = llabs(i); // because INT_MIN would overflow abs()
	uint8_t minus = i < 0;
	uint8_t digits = 1;

	// uint32_t would overflow because INT_MAX and UINT_MAX have the same magnitude
	uint64_t power_of_ten = 10;
	while(true) {
		if ((uint64_t)abs < power_of_ten) {
			break;
		}
		digits++;
		power_of_ten *= 10;
	}

	return minus + digits;
}

// Returns the number of digits in i (so 1 returns 1, -1 returns 2, 10 returns 2, etc)
u8 i64_digits(i64 i)
{
	if (i == INT64_MIN) {
		// avoid overflow in llabs
		return INT64_MIN_LENGTH;
	}
	i64 abs = llabs(i);
	uint8_t minus = i < 0;
	uint8_t digits = 1;

	// Won't overflow (UINT64_MAX is ~2x10^19, one order of magnitude greater than INT64_MAX (~9x10^18))
	uint64_t power_of_ten = 10;
	while(true) {
		if ((uint64_t)abs < power_of_ten) {
			break;
		}
		digits++;
		power_of_ten *= 10;
	}

	return minus + digits;
}

// Write integer to a new str (allocated with calloc)
struct str *i32_to_new_str(i32 i)
{
	uint8_t length = i32_digits(i);
	struct str *new_str = str_new_cleared(length + 1); // + 1 for snprintf
	snprintf(new_str->s, length + 1, "%" PRIi32, i);
	new_str->len = length; // Don't rely on the extra '\0' here
	return new_str;
}

// Write integer to a new str (allocated with calloc)
struct str *int64_t_to_new_str(i64 i)
{
	uint8_t length = i64_digits(i);
	struct str *new_str = str_new_cleared(length + 1); // + 1 for snprintf
	snprintf(new_str->s, length + 1, "%" PRIi64, i);
	new_str->len = length; // Don't rely on the extra '\0' here
	return new_str;
}

// return true if c is equal to any character in s
b8 char_in(c8 c, struct str s)
{
	for (uint32_t i = 0; i < s.len; i++) {
		if (c == str_get(&s, i)) {
			return true;
		}
	}
	return false;
}
// Returns a vec of slices (size 8) into splitting, based on the chars in split_on
struct vec *split_new(const struct str *splitting, struct str split_on)
{
	struct vec *split = vec_new(sizeof(struct slice));
	uint32_t next_split_start = 0;

	uint32_t i = 0;
	for (;i < splitting->len; i++){
		if (char_in(str_get(splitting, i), split_on)) {
			vec_push_slice(split, (struct slice) { .index = next_split_start, .len = i - next_split_start});
			next_split_start = i + 1;
		}
	}
	if (next_split_start < splitting->len) {
		vec_push_slice(split, (struct slice) { .index = next_split_start, .len = splitting->len - next_split_start});
	}
	return split;
}

#endif
