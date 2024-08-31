#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

size_t min(size_t a, size_t b) { return a < b ? a : b; }

size_t max(size_t a, size_t b) { return a > b ? a : b; }

struct galaxy {
  // Just store its position
  size_t x;
  size_t y;
};

// Modified version of FreeBSD strlen
// https://stackoverflow.com/a/2070588/12203444
size_t linelen(const char *str) {
  const char *s;
  for (s = str; *s && *s != '\n'; ++s)
    ;
  return (s - str);
}

size_t count_ocurrences(const char *str, char c) {
  size_t count = 0;
  for (const char *s = str; *s; ++s) {
    if (*s == c) {
      ++count;
    }
  }
  return count;
}

// Assuming new line at the end!
size_t lines_in(const char *str) { return count_ocurrences(str, '\n'); }

size_t string_offset_2d(size_t x, size_t y, size_t cols) {
  return (y * (cols + 1 /* because of newline character */)) + x;
}

int main() {
  // https://stackoverflow.com/questions/14002954/c-how-to-read-an-entire-file-into-a-buffer
  FILE *f = fopen("input.txt", "rb");
  if (!f) {
    fprintf(stderr, "The file input.txt doesn't exist!\n");
    return 1;
  }
  fseek(f, 0, SEEK_END);
  long fsize = ftell(f);
  fseek(f, 0, SEEK_SET); /* same as rewind(f); */

  char *string = malloc(fsize + 1);
  fread(string, fsize, 1, f);
  fclose(f);

  string[fsize] = 0;

  size_t grid_width = linelen(string);
  size_t grid_height = lines_in(string);

  int64_t *horizontal_distances = malloc(sizeof(int64_t) * grid_width);
  int64_t *vertical_distances = malloc(sizeof(int64_t) * grid_height);
  // Distances are expanded by defualt, and then changed to be not expanded
  // later
  const int64_t expansion = 1000000;
  for (size_t i = 0; i < grid_width; i++) {
    horizontal_distances[i] = expansion;
  }

  for (size_t i = 0; i < grid_height; i++) {
    vertical_distances[i] = expansion;
  }

  size_t galaxy_count = count_ocurrences(string, '#');

  struct galaxy *galaxies = malloc(sizeof(struct galaxy) * galaxy_count);
  struct galaxy *galaxy_cursor = galaxies;

  for (size_t x = 0; x < grid_width; x++) {
    for (size_t y = 0; y < grid_height; y++) {
      if (string[string_offset_2d(x, y, grid_width)] == '#') {
        galaxy_cursor->x = x;
        galaxy_cursor->y = y;
        galaxy_cursor++;
        horizontal_distances[x] = 1;
        vertical_distances[y] = 1;
      }
    }
  }
  free(string);

  printf("horizontal distances:\n");
  for (size_t i = 0; i < grid_width; i++) {
    printf("%d,", horizontal_distances[i]);
  }

  printf("\nvertical distances:\n");
  for (size_t i = 0; i < grid_height; i++) {
    printf("%d,", vertical_distances[i]);
  }
  printf("\n");

  printf("galaxies:\n");
  for (size_t i = 0; i < galaxy_count; i++) {
    printf("{%llu, %llu}, ", galaxies[i].x, galaxies[i].y);
  }
  printf("\n");

  int64_t total_distance = 0;

  for (size_t g = 0; g < galaxy_count; g++) {
    for (size_t c = g + 1; c < galaxy_count; c++) {
      for (size_t i = min(galaxies[g].x, galaxies[c].x); i < max(galaxies[c].x, galaxies[g].x); i++) {
        total_distance += horizontal_distances[i];
      }
      for (size_t i = min(galaxies[g].y, galaxies[c].y); i < max(galaxies[c].y, galaxies[g].y); i++) {
        total_distance += vertical_distances[i];
      }
    }
  }
  free(horizontal_distances);
  free(vertical_distances);
  free(galaxies);
  printf("%" PRId64 "\n", total_distance);
}
