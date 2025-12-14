#include "grid.c"
#include <stdint.h>
#include <inttypes.h>

uint8_t count_neighbors(struct grid *grid, long x, long y)
{
	uint8_t count = 0;
	for (int i = 0; i < ORDINAL_DIRECTIONS_COUNT; i++) {
		struct point offset = ORDINAL_DIRECTIONS[i];
		short neighbor = grid_get(grid, offset.x + x, offset.y + y);
		if (neighbor == -1) {
			continue;
		}
		if (neighbor == '.') {
			continue;
		}
		count++;
	}
	return count;
}

int main()
{
	auto grid = grid_new_from_file("input.txt");
	int64_t accessable = 0;
	for (long x = 0; x < grid->width; x++) {
		for (long y = 0; y < grid->height; y++) {
			if (grid_get(grid, x, y) == '.') {
				continue;
			}
			if (count_neighbors(grid, x, y) < 4) {
				accessable++;
			}
		}
	}
	grid_free(grid);
	printf("%" PRIi64 "\n", accessable);
}
