#include <stdckdint.h>
#include "lines.c"

#define BATTERIES 12

int64_t best_joltage(union shortstr battery_bank)
{
	panic_if_less(battery_bank.len, BATTERIES);

	char digits[BATTERIES] = { 0 };
	uint8_t digit_indexes[BATTERIES] = { 0 };

	for (int32_t battery = 0; battery < BATTERIES; battery++) {
		uint8_t first_battery = battery == 0 ? 0 : digit_indexes[battery - 1] + 1;
		for(uint8_t i = first_battery; i < battery_bank.len - (BATTERIES - (battery + 1)); i++) {
			if (battery_bank.data[i] > digits[battery]) {
				digits[battery] = battery_bank.data[i];
				digit_indexes[battery] = i;
			}
		}
	}

	return str_to_int64_t((struct str) { .s = digits, .len = BATTERIES });
}

int main()
{
	struct vec *batteries = lines_new("input.txt");

	
	int64_t total_joltage = 0;
	for (uint32_t i = 0; i < batteries->len; i++) {
		int64_t this_joltage = best_joltage(vec_get_shortstr(batteries, i));
		shortstr_print(vec_get_shortstr(batteries, i));
		printf(": %" PRIi64 "\n", this_joltage);
		panic_if_not_equal(ckd_add(&total_joltage, total_joltage, this_joltage), false);
	}

	printf("Total: %" PRIu64 "\n", total_joltage);

	vec_free(batteries);
}
