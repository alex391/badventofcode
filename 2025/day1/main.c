#include <stdio.h>
#include <stdint.h>
#include "slice.c"
#include "vec.c"
#include "lines.c"
#include "modulo.c"
#include <stdbool.h>

struct rotation {
	int32_t value;
	bool right;
};

struct rotation parse_rotation(union shortstr rotation)
{
	panic_if_less(rotation.len, 1);
	bool right = rotation.data[0] == 'R' ? true : false;
	panic_if_less(rotation.len, 2);
	struct str value_part = { .s = rotation.s + 1, .len = rotation.len -1 };
	int32_t value = str_to_int32_t(value_part);
	return (struct rotation) { .value = value, .right = right };
}

int main()
{
	FILE *input = fopen("input.txt", "r");
	struct vec *lines = lines_new(input);
	fclose(input);

	int64_t dial = 50;
	int64_t password = 0;
	for (uint32_t i = 0; i < lines->len; i++) {
		// brute force!!
		struct rotation rotation = parse_rotation(vec_get_shortstr(lines, i));
		for (int32_t i = 0; i < rotation.value; i++) {
			if (rotation.right) {
				dial++;
			}
			else {
				dial--;
			}
			dial = positive_modulo(dial, 100);
			if (dial == 0) {
				password++;
			}
		}
	}
	vec_free(lines);
	printf("%" PRIi64 "\n", password);
}
