#include "grid.c"
#include "slice.c"

constexpr u32 MAX_NUMBERS = 10;
constexpr u32 MAX_DIGITS = 10;

int main()
{
	auto grid = grid_new_from_file("input.txt");
	
	i64 sum_of_problems = 0;
	
	i64 numbers[MAX_NUMBERS] = { };
	u32 numbers_count = 0;

	for (isize column = grid->width; column--;) {
		c8 digits_buff[MAX_DIGITS] = { };
		struct str digits = { .s = digits_buff, .len = 0 };
		c8 operator = 0;
		for (isize row = 0; row < grid->height; row++) {
			i16 c = grid_get(grid, column, row);
			panic_if_equal(c, -1);
			if (c == ' ') {
				continue;
			}
			if (c == '+' || c == '*') {
				operator = c;
				break;
			}
			panic_if_equal(digits.len, MAX_DIGITS);
			digits.s[digits.len++] = c;
		}
		if (digits.len == 0) {
			continue;
		}
		i64 number = str_to_i64(digits);

		switch (operator) {
			case 0:
				panic_if_equal(numbers_count, MAX_NUMBERS);
				numbers[numbers_count++] = number;
				break;
			case '+':
				i64 sum = 0;
				for (u32 i = 0; i < numbers_count; i++) {
					panic_if_equal(ckd_add(&sum, sum, numbers[i]), true);
				}
				panic_if_equal(ckd_add(&sum, sum, number), true);
				panic_if_equal(ckd_add(&sum_of_problems, sum_of_problems, sum), true);
				memset(numbers, 0, sizeof(numbers));
				numbers_count = 0;
				break;
			case '*':
				i64 product = 1;
				for (u32 i = 0; i < numbers_count; i++) {
					panic_if_equal(ckd_mul(&product, product, numbers[i]), true);
				}
				panic_if_equal(ckd_mul(&product, product, number), true);
				panic_if_equal(ckd_add(&sum_of_problems, sum_of_problems, product), true);
				memset(numbers, 0, sizeof(numbers));
				numbers_count = 0;
				break;
		}
	}
	grid_free(grid);
	printf(Pi64, sum_of_problems);
}
