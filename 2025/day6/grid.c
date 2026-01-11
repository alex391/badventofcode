#include "panic.c"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "ints.c"

// Struct for storing rectangular grid
struct grid {
	isize width;
	isize height;
	c8 *data;
};

// Struct for storing points
struct point {
	isize x;
	isize y;
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
constexpr i32 ORDINAL_DIRECTIONS_COUNT = (NORTH_WEST - NORTH) + 1;

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
struct grid *grid_new(isize width, isize height)
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
isize count_lines(FILE *f)
{
	isize start_position = ftell(f);
	panic_if_equal(start_position, -1);
	
	isize lines = 0;
	i32 c = 0;
	while(true) {
		c = getc(f);
		if (c == '\n' || c == EOF) {
			lines++;
			if (c == EOF) {
				break;
			}
			i32 temp = getc(f);
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
struct grid *grid_new_from_file(const c8 *filename)
{
	FILE *f = panic_if_zero(fopen(filename, "rb"));

	i32 c = 0;
	isize width = 0;
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
	isize i = 0;
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
// corner (or the i16 -1 if it's out of bounds)
i16 grid_get(struct grid *grid, isize x, isize y)
{
	if (x >= grid->width || x < 0 || y >= grid->height || y < 0) {
		return -1;
	}
	return (u8)grid->data[y * grid->width + x];
}

// Print the grid
void grid_print(struct grid *grid)
{
	for (isize y = 0; y < grid->height; y++) {
		for (isize x = 0; x < grid->width; x++) {
			i16 c = grid_get(grid, x, y);
			panic_if_equal(c, -1);
			printf("%c", c);
		}
		printf("\n");
	}
}

// If x and y are in bounds, write c to that coordinate of the grid
// Otherwise, do nothing
void grid_set(struct grid *grid, isize x, isize y, c8 c)
{
	if (x >= grid->width || x < 0 || y >= grid->height || y < 0) {
		return;
	}
	grid->data[y * grid->width + x] = c;
}

// Copy a grid from src to dest (src should be the same size)
void grid_copy(struct grid *restrict dest, struct grid *restrict src)
{
	panic_if_not_equal(dest->width, src->width);
	panic_if_not_equal(dest->height, src->height);
	memcpy(dest->data, src->data, dest->width * dest->height);
}
