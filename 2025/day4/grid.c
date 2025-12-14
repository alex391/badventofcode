#include "panic.c"
#include <stdio.h>
#include <stdlib.h>

// Struct for storing rectangular grid
struct grid {
	long width;
	long height;
	char *data;
};


// Struct for storing points
struct point {
	long x;
	long y;
};

// The names of the directions in ORDINAL_DIRECTIONS
enum direction_names {
	NORTH,
	NORTH_EAST,
	EAST,
	SOUTH_EAST,
	SOUTH,
	SOUTH_WEST,
	WEST,
	NORTH_WEST
};

// The number of directions in ORDINAL_DIRECTIONS
constexpr int ORDINAL_DIRECTIONS_COUNT = (NORTH_WEST - NORTH) + 1;

constexpr struct point ORDINAL_DIRECTIONS[ORDINAL_DIRECTIONS_COUNT] = {
	{ 0, -1 },
	{ 1,  -1 },
	{ 1,  0 },
	{ 1,  1 },
	{ 0,  1 },
	{ -1,  1 },
	{ -1,  0 },
	{ -1,  -1 },
};


// Allocate a new grid with calloc. Width and height should be positive.
struct grid *grid_new(long width, long height)
{
	struct grid *new_grid = panic_if_zero(calloc(1, sizeof(struct grid)));
	if (width < 0) {
		width = 0;
	}
	if (height < 0) {
		height = 0;
	}
	new_grid->data = panic_if_zero(calloc(1, width * height));
	new_grid->width = width;
	new_grid->height = height;
	return new_grid;
}

// Free freeing and its data
void grid_free(struct grid *freeing)
{
	free(freeing->data);
	free(freeing);
}

// Count the number of lines in a file
long count_lines(FILE *f)
{
	long start_position = ftell(f);
	panic_if_equal(start_position, -1);
	
	long lines = 0;
	int c = 0;
	while(true) {
		c = getc(f);
		if (c == '\n' || c == EOF) {
			lines++;
			if (c == EOF) {
				break;
			}
			auto temp = getc(f);
			if (temp == EOF) {
				break;
			}
			ungetc(temp, f);
		}
	}
	panic_if_not_equal(fseek(f, start_position, SEEK_SET), 0);
	return lines;
}

// Create a new grid, given null-terminated filename (assuming that the file
// contains rectangular data, where the first line is the same length as the
// rest of the lines)
struct grid *grid_new_from_file(const char *filename)
{
	FILE *f = panic_if_zero(fopen(filename, "rb"));

	int c = 0;
	long width = 0;
	while(true) {
		c = getc(f);
		if (c == '\n' || c == EOF) {
			break;
		}
		width++;
	}
	rewind(f);
	auto height = count_lines(f);
	auto grid = grid_new(width, height);
	long i = 0;
	while(true) {
		c = getc(f);
		if (c == EOF) {
			break;
		}
		if (c == '\n') {
			continue;
		}
		grid->data[i++] = c;
	}
	fclose(f);
	
	return grid;
}

// Get the character at the coordinate (x, y), where (0, 0) is the top left
// corner (or the short -1 if it's out of bounds)
short grid_get(struct grid *grid, long x, long y)
{
	if (x >= grid->width || x < 0 || y >= grid->height || y < 0) {
		return -1;
	}
	return (unsigned char)grid->data[y * grid->width + x];
}

// Print the grid
void grid_print(struct grid *grid)
{
	for (long y = 0; y < grid->height; y++) {
		for (long x = 0; x < grid->width; x++) {
			short c = grid_get(grid, x, y);
			panic_if_equal(c, -1);
			printf("%c", c);
		}
		printf("\n");
	}
}

// If x and y are in bounds, write c to that coordinate of the grid
// Otherwise, do nothing
void grid_set(struct grid *grid, long x, long y, char c)
{
	if (x >= grid->width || x < 0 || y >= grid->height || y < 0) {
		return;
	}
	grid->data[y * grid->width + x] = c;
}
