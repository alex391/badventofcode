#include <stdio.h>
#include <stdint.h>
#include "slice.c"
#include "modulo.c"
#include "vec.c"
#include "lines.c"

int32_t parse_rotation(union shortstr rotation)
{
	panic_if_less(rotation.len, 1);
	int8_t sign = rotation.data[0] == 'R' ? 1 : -1;
	panic_if_less(rotation.len, 2);
	struct str value_part = { .s = rotation.s + 1, .len = rotation.len -1 };
	int32_t value = str_to_int32_t(value_part);
	return sign * value;
}

int main()
{
	FILE *input = fopen("input.txt", "r");
	struct vec *lines = lines_new(input);
	fclose(input);

	int32_t dial = 50;
	int32_t password = 0;
	for (uint32_t i = 0; i < lines->len; i++) {
		dial += parse_rotation(vec_get_shortstr(lines, i));
		dial = positive_modulo(dial, 100);
		if (dial == 0) {
			password++;
		}
	}
	vec_free(lines);
	printf("%" PRIi32 "\n", password);
}
