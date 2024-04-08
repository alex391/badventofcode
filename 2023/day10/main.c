#define ARENA_IMPLEMENTATION
#include "lib/arena/arena.h"
#include <stdint.h>
#include <stdio.h>

#define MAX_ROWS 150
#define MAX_COLUMNS 150

struct node {
  struct node *forwards;
  struct node *backwards;
};

enum directions { NORTH, EAST, SOUTH, WEST, NONE };

struct pipe_type {
  enum directions first_outlet;
  enum directions second_outlet;
};

struct pipe_type *character_to_type(char c, Arena* arena) {
  struct pipe_type* type = arena_alloc(arena, sizeof(type));
  switch (c) {
  case '|':
    
  default:
    type->first_outlet = NONE;
    type->second_outlet = NONE;
  }
  return type;
}

// Find out which pipe the start is
enum directions *connected_to_start(size_t start_x, size_t start_y,
                                    char **grid) {

  return NULL;
}

int main() {
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
    if (c == '\n') {
      max_column = column; // Input is rectangular
      column = 0;
      row++;
    } else {
      column++;
    }
    if (c == 'S') {
      start_x = column;
      start_y = row;
    }
    if (row > MAX_ROWS || column > MAX_COLUMNS) {
      fprintf(stderr, "Grid is out of range!");
      return 1;
    }
    grid[row][column] = c;
  }

  struct node start;
  start.forwards = NULL;
  start.backwards = NULL;
}
