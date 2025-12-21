#include "hash.c"
#include <stdint.h>
#include <stdlib.h>
#include "panic.c"
#include <stdbool.h>
#include "slice.c"
#include <string.h>
#include <inttypes.h>
#include <ctype.h>

struct hash_map {
	void *storage;
	size_t item_size;
	uint32_t storage_length;
};

// Reference hashable data. Hash map data should at a minimum have some data
// that's convertible to a str to use as a key, and a way to tell if it's null
// or some sort of empty sentinel
struct pair_shortstr_int32_t
{
	int32_t value;
	union shortstr key;
};

// So that the following functions know how to hash it:
struct str pair_shortstr_int32_t_get_key(void *item)
{
	struct pair_shortstr_int32_t *casted_item = item;
	return shortstr_to_str(casted_item->key);
}

// It's considered empty if the shortstr is empty
bool pair_shortstr_int32_t_is_empty(void *item)
{
	return ((struct pair_shortstr_int32_t *)item)->key.len == 0;
}

// Allocate a new hash_map (using calloc)
// storage_length should be the maximum number of items you'll store, and
// item_size is sizeof(<your hashmap item>)
struct hash_map *hash_map_new(uint32_t storage_length, size_t item_size)
{
	struct hash_map *new = panic_if_zero(calloc(1, sizeof(struct hash_map)));
	new->storage = calloc(storage_length, item_size);
	new->storage_length = storage_length;
	new->item_size = item_size;
	return new;
}


// Free hash map and its storage
void hash_map_free(struct hash_map *freeing)
{
	free(freeing->storage);
	free(freeing);
}

// Add or update item to updating, using get_key to get the key from the item,
// and is_empty to check if the item is empty (null or some sentinal)
// returns true if it succeded in the insert, false if it ran out of space
//
// You may want to write a helper function for this
bool hash_map_update(struct hash_map *updating, struct str (*get_key)(void *), bool (*is_empty)(void *), void *item)
{
	// Calculate index, check key, then memcpy
	struct str key = get_key(item);
	uint32_t index = hash(key.s, key.len) % updating->storage_length;
	void *pointer_to_storage = NULL;
	for (; index < updating->storage_length; index++)
	{
		pointer_to_storage = updating->storage + (updating->item_size * index);
		if(is_empty(pointer_to_storage)) { // insert
			goto found_space;
		}
		struct str this_key = get_key(pointer_to_storage);
		if (str_equal(&this_key, &key)) { // replace
			goto found_space;
		}
	}
	return false;
found_space:
	// memcpy the item over the existing item 
	memcpy(pointer_to_storage, item, updating->item_size);
	return true;
}

// Get a pointer to an item, using get_key to read keys from items in the hash
// map storage, and is_empty to tell if an item in storage is empty. Returns NULL if
// not found
//
// You may want to write a helper function for this
void *hash_map_get(struct hash_map *getting, struct str (*get_key)(void *), bool (*is_empty)(void *), struct str key)
{
	uint32_t index = hash(key.s, key.len) % getting->storage_length;
	for(; index < getting->storage_length; index++)
	{
		void *pointer_to_storage = getting->storage + (getting->item_size * index);
		if (is_empty(pointer_to_storage)) {
			return NULL;
		}
		struct str this_key = get_key(pointer_to_storage);
		if(str_equal(&key, &this_key)) {
			return pointer_to_storage;
		}
	}
	return NULL;

}

// Interpret hash map storage bytes as char, and print them, along with the item
// size and storage length
void hash_map_debug_print(struct hash_map *printing)
{
	printf("storage: \n");
	char *storage_as_char = printing->storage;
	for (size_t i = 0; i < printing->item_size * printing->storage_length; i++) {
		if (isprint(storage_as_char[i])) {
			printf("%c ", storage_as_char[i]);
		}
		else {
			printf("  ");
		}
	}
	printf("\n");
	printf("item_size: %zu\n", printing->item_size);
	printf("storage_length: %" PRIu32 "\n", printing->storage_length);
}	
