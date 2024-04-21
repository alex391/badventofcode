#define ARENA_IMPLEMENTATION
#include "lib/arena/arena.h"
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#define MAX_ROWS 150
#define MAX_COLUMNS 150

struct pipe_t {
	char name;
	int connections[2];
};

struct pipe_t_slice {
	struct pipe_t *buffer;
	size_t length;
};

enum direction {
	NORTH,
	EAST,
	SOUTH,
	WEST,
	NONE,
};

struct pipe_t_slice *filter_pipes(const struct pipe_t *pipes, size_t size,
				  int direction, Arena *a);
bool pipe_t_in(struct pipe_t *needle, struct pipe_t_slice *haystack);
const struct pipe_t pipe_types[] = {
	{ '|', { NORTH, SOUTH } }, { '-', { EAST, WEST } },
	{ 'L', { NORTH, EAST } },  { 'J', { NORTH, WEST } },
	{ '7', { SOUTH, WEST } },  { 'F', { SOUTH, EAST } },
	{ '.', { NONE, NONE } },
};

struct pipe_t *lookup_pipe(char pipe_name)
{
	// Just linear search
	struct pipe_t *pipe = (struct pipe_t *)pipe_types;
	while (pipe <
	       (pipe_types + (sizeof(pipe_types) / sizeof(struct pipe_t)))) {
		if (pipe->name == pipe_name) {
			return pipe;
		}
		pipe++;
	}
	return pipe; // Just return the last one ('.')
}

// Find out which pipe the start is
struct pipe_t *start_type(size_t start_x, size_t start_y,
			  char grid[][MAX_COLUMNS], size_t grid_height,
			  size_t grid_width, Arena *b)
{
	Arena a = { 0 };
	size_t connections_index = 0;
	struct pipe_t *start = arena_alloc(b, sizeof(struct pipe_t));
	start->name = 'S';
	start->connections[0] = NONE;
	start->connections[1] = NONE;

	// Idea: check all four directions, and check if it can connect

	// North:
	// Is one of the south-facing pipes connected?
	if (start_y >= 1) {
		struct pipe_t_slice *south_pipes = filter_pipes(
			pipe_types, sizeof(pipe_types) / sizeof(struct pipe_t),
			SOUTH, &a);
		char up_pipe_name = grid[start_y - 1][start_x];
		struct pipe_t *up_pipe = lookup_pipe(up_pipe_name);
		// check if up_pipe is in south_pipes, and add it to the start
		if (pipe_t_in(up_pipe, south_pipes)) {
			start->connections[connections_index++] = NORTH;
		}
	}

	// South
	if (start_y + 1 < grid_height) {
		struct pipe_t_slice *pipes = filter_pipes(
			pipe_types, sizeof(pipe_types) / sizeof(struct pipe_t),
			NORTH, &a);
		char pipe_name = grid[start_y + 1][start_x];
		struct pipe_t *pipe = lookup_pipe(pipe_name);
		// check if up_pipe is in south_pipes, and add it to the start
		if (pipe_t_in(pipe, pipes)) {
			start->connections[connections_index++] = SOUTH;
		}
	}

	// East
	if (start_x + 1 < grid_width) {
		struct pipe_t_slice *pipes = filter_pipes(
			pipe_types, sizeof(pipe_types) / sizeof(struct pipe_t),
			WEST, &a);
		char pipe_name = grid[start_y][start_x + 1];
		struct pipe_t *pipe = lookup_pipe(pipe_name);
		// check if up_pipe is in south_pipes, and add it to the start
		if (pipe_t_in(pipe, pipes)) {
			start->connections[connections_index++] = EAST;
		}
	}

	// West
	if (start_x >= 1) {
		struct pipe_t_slice *pipes = filter_pipes(
			pipe_types, sizeof(pipe_types) / sizeof(struct pipe_t),
			EAST, &a);
		char pipe_name = grid[start_y][start_x - 1];
		struct pipe_t *pipe = lookup_pipe(pipe_name);
		// check if up_pipe is in south_pipes, and add it to the start
		if (pipe_t_in(pipe, pipes)) {
			start->connections[connections_index++] = WEST;
		}
	}
	arena_free(&a);
	return start;
}

// Just a linear search
bool int_in(int needle, const int *haystack, size_t size)
{
	for (size_t i = 0; i < size; i++) {
		if (haystack[i] == needle) {
			return true;
		}
	}
	return false;
}

bool pipe_t_in(struct pipe_t *needle, struct pipe_t_slice *haystack)
{
	for (size_t i = 0; i < haystack->length; i++) {
		if (haystack->buffer[i].name == needle->name) {
			return true;
		}
	}
	return false;
}

// Filter pipes by what direction they contain
struct pipe_t_slice *filter_pipes(const struct pipe_t *pipes, size_t size,
				  int direction, Arena *a)
{
	struct pipe_t_slice *filtered =
		arena_alloc(a, sizeof(struct pipe_t_slice));
	filtered->buffer = arena_alloc(a, sizeof(struct pipe_t_slice));
	filtered->length = 0;
	for (size_t i = 0; i < size; i++) {
		if (int_in(direction, pipes[i].connections,
			   2 /* the number of connections each pipe has */)) {
			filtered->buffer[filtered->length++] = pipes[i];
		}
	}
	return filtered;
}

uint32_t steps_to_furthest(size_t start_x, size_t start_y,
			   struct pipe_t *start_pipe, char grid[][MAX_COLUMNS],
			   size_t grid_height, size_t grid_width)
{
	size_t position_x = start_x;
	size_t position_y = start_y;
	struct pipe_t *current_pipe = start_pipe;
	int direction =
		current_pipe->connections[0]; // Just pick a diection to go first
	uint32_t count = 0;
	do {
		switch (direction) {
		// I'm assuming the problem is nice enough to not put us out of bounds.
		case NORTH:
			position_y--;
			break;
		case SOUTH:
			position_y++; // South is actually positive y
			break;
		case EAST:
			position_x++;
			break;
		case WEST:
			position_x--;
			break;
		default:
			// And you may ask yourself, "Well, how did I get here?"
			fprintf(stderr, "Bad direction %d\n", direction);
			exit(2);
		}
		count++;
		current_pipe = lookup_pipe(grid[position_y][position_x]);
		// change the direction to the direction that woudn't cause us to just go backwards
		if (current_pipe->connections[0] != (direction + 2) % 4) {
			direction = current_pipe->connections[0];
		} else {
			direction = current_pipe->connections[1];
		}

	} while (position_x != start_x || position_y != start_y);

	return count / 2;
}

int main()
{
	FILE *fp;
	fp = fopen("input.txt", "r");
	if (fp == NULL) {
		fprintf(stderr, "Couldn't open the file!");
		return 1;
	}
	char grid[MAX_ROWS][MAX_COLUMNS];
	int c;
	size_t row = 0;
	size_t column = 0;
	size_t max_column = 0; // Because \nEOF resets column, store the max
	size_t start_x, start_y;
	while ((c = getc(fp)) != EOF) {
		if (c == 'S') {
			start_x = column;
			start_y = row;
		}
		if (row > MAX_ROWS || column > MAX_COLUMNS) {
			fprintf(stderr, "Grid is out of range!");
			return 1;
		}
		grid[row][column] = c;
		if (c == '\n') {
			if (column > max_column) {
				max_column = column;
			}
			column = 0;
			row++;
		} else {
			column++;
		}
	}
	Arena scratch = { 0 };
	struct pipe_t *start_pipe =
		start_type(start_x, start_y, grid, row, max_column, &scratch);

	uint32_t steps = steps_to_furthest(start_x, start_y, start_pipe, grid,
					   row, max_column);
	printf("steps: %u\n", steps);
}
