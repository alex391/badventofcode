#include "grid.c"

int main()
{
	auto grid = grid_new_from_file("input.txt");


	i64 splits = 0;
	for (isize y = 0; y < grid->height; y++) {
		for (isize x = 0; x < grid->width; x++) {
			constexpr struct point north = ORDINAL_DIRECTIONS[NORTH];
			i16 above = grid_get(grid, x + north.x, y + north.y);
			if (above == 'S') {
				above = '|';
			}
			i16 this = grid_get(grid, x, y);
			panic_if_equal(this, -1);
			if (this == '|') {
				continue;
			}
			if (this == '^') {
				if (above != '|') {
					continue;
				}
				constexpr struct point west = ORDINAL_DIRECTIONS[WEST];
				constexpr struct point east = ORDINAL_DIRECTIONS[EAST];
				auto west_is_beam = grid_get(grid, x + west.x, y + west.y) == '|';
				if (!west_is_beam) {
					grid_set(grid, west.x + x, west.y + y, '|');
				}
				auto east_is_beam = grid_get(grid, x + east.x, y + east.y) == '|';
				if (!east_is_beam ) {
					grid_set(grid, east.x + x, east.y + y, '|');
				}
				splits++;
			}
			if (this == '.') {
				if (above == '|') {
					grid_set(grid, x, y, '|');
				}
			}
		}
	}

	grid_print(grid);
	grid_free(grid);
	
	printf(Pi64, splits);
}
