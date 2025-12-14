#include <stdint.h>
#include <stddef.h>

uint32_t hash(const void *data, size_t size)
{
	uint32_t hash = 5381;
	for(size_t i = 0; i < size; i++) {
		hash = ((hash << 5) + hash) + ((char *)data)[i];
	}
	return hash;
}
