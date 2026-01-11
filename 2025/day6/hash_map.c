#include "hash.c"
#include <stdlib.h>
#include "panic.c"
#include <stdbool.h>
#include "slice.c"
#include <string.h>
#include <ctype.h>

struct hash_map {
	void *storage;
	usize item_size;
	u32 storage_length;
};

// Reference hashable data. Hash map data should at a minimum have some data
// that's convertible to a str to use as a key, and a way to tell if it's null
// or some sort of empty sentinel
struct pair_shortstr_i32 {
	i32 value;
	union shortstr key;
};

// Good for sets of integers
struct pair_i64_b8 {
	i64 key;
	b8 value;
};

// So that the following functions know how to hash it:
struct str pair_shortstr_i32_get_key(void *item)
{
	struct pair_shortstr_i32 *casted_item = item;
	return shortstr_to_str(casted_item->key);
}

struct str pair_i64_b8_get_key(void *item)
{
	struct pair_i64_b8 *casted_item = item;
	c8 key_bytes[sizeof(i64)] = { };
	memcpy(key_bytes, &casted_item->key, sizeof(i64));
	return (struct str) { .s = key_bytes, .len = sizeof(i64)};
}

// It's considered empty if the shortstr is empty
b8 pair_shortstr_i32_is_empty(void *item)
{
	return ((struct pair_shortstr_i32 *)item)->key.len == 0;
}

b8 pair_i64_b8_is_empty(void *item) {
	return ((struct pair_i64_b8 *)item)->key == false;
}

// Allocate a new hash_map (using calloc)
// storage_length should be the maximum number of items you'll store, and
// item_size is sizeof(<your hashmap item>)
struct hash_map *hash_map_new(u32 storage_length, usize item_size)
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
b8 hash_map_update(struct hash_map *updating, struct str (*get_key)(void *), b8 (*is_empty)(void *), void *item)
{
	// Calculate index, check key, then memcpy
	struct str key = get_key(item);
	u32 index = hash(key.s, key.len) % updating->storage_length;
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

b8 hash_map_update_pair_int64_t_bool(struct hash_map *updating, struct pair_i64_b8 item)
{
	panic_if_not_equal(updating->item_size, sizeof(struct pair_i64_b8));
	return hash_map_update(updating, pair_i64_b8_get_key, pair_i64_b8_is_empty, &item);
}

// Get a pointer to an item, using get_key to read keys from items in the hash
// map storage, and is_empty to tell if an item in storage is empty. Returns NULL if
// not found
//
// You may want to write a helper function for this
void *hash_map_get(struct hash_map *getting, struct str (*get_key)(void *), b8 (*is_empty)(void *), struct str key)
{
	u32 index = hash(key.s, key.len) % getting->storage_length;
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

b8 hash_map_get_pair_i64_b8(struct hash_map *getting, i64 key)
{
	b8 out = false;
	c8 key_bytes[sizeof(i64)] = {  };
	memcpy(key_bytes, &key, sizeof(i64));
	out = *(b8 *)hash_map_get(getting, pair_i64_b8_get_key, pair_i64_b8_is_empty, (struct str) { .s = key_bytes, .len = sizeof(i64)});
	return out;
}

// Interpret hash map storage bytes as char, and print them, along with the item
// size and storage length
void hash_map_debug_print(struct hash_map *printing)
{
	printf("storage: \n");
	c8 *storage_as_char = printing->storage;
	for (usize i = 0; i < printing->item_size * printing->storage_length; i++) {
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
