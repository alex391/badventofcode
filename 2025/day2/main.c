#include "read_file.c"

bool invalid(int64_t id)
{
	struct str *id_as_str = int64_t_to_new_str(id);
	if (id_as_str->len % 2 == 1) {
		return false;
	}
	struct str first_half = { .s = id_as_str->s, .len = id_as_str->len / 2 };
	struct str second_half = { .s = id_as_str->s + id_as_str->len / 2, .len = id_as_str->len / 2 };
	bool result = str_equal(&first_half, &second_half);	
	str_free(id_as_str);
	return result;
}

int main()
{
	struct str *file = str_new_from_file("input.txt");
	// drop newline
	file->len--;
	struct vec *split = split_new(file, (struct str) { .s = ",-", .len = 2 });

	int64_t invalid_ids_sum = 0;

	for (uint32_t i = 0; i < split->len; i += 2) {
		int64_t first_number = str_to_int64_t(slice_to_str(vec_get_slice(split, i), file->s));
		int64_t second_number = str_to_int64_t(slice_to_str(vec_get_slice(split, i + 1), file->s));
		for (int64_t j = first_number; j <= second_number; j++) {
			if (invalid(j)) {
				invalid_ids_sum += j;
			}
		}
	}

	printf("%" PRIi64 "\n", invalid_ids_sum);

	vec_free(split);
	str_free(file);
}
