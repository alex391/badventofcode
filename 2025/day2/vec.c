#ifndef VEC_C
#define VEC_C

#include <stdlib.h>
#include <stdint.h>
#include "panic.c"
#include <string.h>
#include "slice.c"


#define DEFAULT_CAPACITY 10

struct vec {
	void *data;
	uint32_t len;
	uint32_t capacity;
	size_t item_size;
};

// Returns a new vec, guaranteed to not be null, allocated with calloc (so len,
// item_size, and capacity are both 0, and data is an invalid pointer)
struct vec *vec_new_cleared()
{
	return panic_if_zero(calloc(1, sizeof(struct vec)));
}

// Return a vec new vec, with data initialized
struct vec *vec_new(size_t item_size)
{
	struct vec *new = vec_new_cleared();
	new->capacity = DEFAULT_CAPACITY;
	new->item_size = item_size;

	new->data = panic_if_zero(calloc(new->capacity, new->item_size));
	
	return new;
}

// Free a vec and it's data (if data is not zero)
void vec_free(struct vec *freeing)
{
	if (freeing->data) {
		free(freeing->data);
	}
	free(freeing);
}

// Add data to the end of the vec, resizing if necessary
// You may want to write a convenience for this for your type (see below)
void vec_push(struct vec *vec, void *data)
{
	if (vec->len == vec->capacity) {
		panic_if_greater(vec->capacity, UINT32_MAX / 2);
		vec->capacity *= 2;
		vec->data = panic_if_zero(realloc(vec->data, vec->capacity * vec->item_size));
		// For safety, reading indexes past len - 1 is still valid (as long as the index is less than capacity - 1)
		memset(vec->data + (vec->len * vec->item_size), 0, (vec->capacity * vec->item_size) - (vec->len * vec->item_size));
	}
	memcpy(vec->data + (vec->len * vec->item_size), data, vec->item_size);
	panic_if_equal(vec->len, UINT32_MAX);
	vec->len++;
}

void vec_push_int32_t(struct vec *vec, int32_t data)
{
	panic_if_not_equal(vec->item_size, sizeof(uint32_t));
	vec_push(vec, &data);
}

void vec_push_shortstr(struct vec *vec, union shortstr s)
{
	panic_if_not_equal(vec->item_size, sizeof(union shortstr));
	vec_push(vec, &s);
}

void vec_push_slice(struct vec *vec, struct slice slice)
{
	panic_if_not_equal(vec->item_size, sizeof(struct slice));
	vec_push(vec, &slice);
}

// Put the item at the index into out
// You may want to write a convenience for this for your type (see below)
void vec_get(struct vec *vec, uint32_t index, void *out)
{
	if (index < vec->len) {
		memcpy(out, vec->data + (index * vec->item_size), vec->item_size);
	}
}

int32_t vec_get_int32_t(struct vec *vec, uint32_t index)
{
	panic_if_not_equal(vec->item_size, sizeof(int32_t));
	int32_t out = 0;
	vec_get(vec, index, &out);
	return out;
}

union shortstr vec_get_shortstr(struct vec *vec, uint32_t index)
{
	panic_if_not_equal(vec->item_size, sizeof(union shortstr));
	union shortstr out = { 0 };
	vec_get(vec, index, &out);
	return out;
}

struct slice vec_get_slice(struct vec *vec, uint32_t index)
{
	panic_if_not_equal(vec->item_size, sizeof(struct slice));
	struct slice out = { 0 };
	vec_get(vec, index, &out);
	return out;
}



// Same as vec_get, but also decreases len by 1 (effectively deleting the last
// element)
void vec_pop(struct vec *vec, void *out)
{
	vec_get(vec, vec->len, out);
	vec->len--;
}

// Sort (wrapper for qsort)
void vec_sort(struct vec *vec, int (*comp)(const void *lhs, const void *rhs))
{
	qsort(vec->data, vec->len, vec->item_size, comp);
}

#endif
