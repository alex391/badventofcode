#include <stddef.h>
#include "ints.c"

u32 hash(const void *data, usize size)
{
	u32 hash = 5381;
	for(usize i = 0; i < size; i++) {
		hash = ((hash << 5) + hash) + ((c8 *)data)[i];
	}
	return hash;
}
