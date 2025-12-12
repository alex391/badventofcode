#include "lines.c"

uint8_t best_joltage(union shortstr battery_bank)
{
	uint8_t first_digit = 0;
	uint8_t first_digit_index = 0;
	panic_if_less(battery_bank.len, 1);
	for (uint8_t i = 0; i < battery_bank.len - 1; i++) {
		if (battery_bank.data[i] > first_digit) {
			first_digit = battery_bank.data[i];
			first_digit_index = i;
		}
	}
	first_digit -= '0';

	uint8_t second_digit = 0;
	for (uint8_t i = first_digit_index + 1; i < battery_bank.len; i++) {
		if (battery_bank.data[i] > second_digit) {
			second_digit = battery_bank.data[i];
		}
	}
	second_digit -= '0';

	return first_digit * 10 + second_digit;
}

int main()
{
	struct vec *batteries = lines_new("input.txt");

	
	uint64_t total_joltage = 0;
	for (uint32_t i = 0; i < batteries->len; i++) {
		uint8_t this_joltage = best_joltage(vec_get_shortstr(batteries, i));
		total_joltage += this_joltage;
	}

	printf("Total: %" PRIu64 "\n", total_joltage);

	vec_free(batteries);
}
