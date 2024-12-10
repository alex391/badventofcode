#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>

#define WIDTH 140
#define HEIGHT 140

int check_x(size_t search_x, size_t search_y, char word_search[HEIGHT][WIDTH + 1])
{
	int matches = 0;
	const char *mas = "MAS";

	const int direction_lookup[8][2] = {
		// You could like, calculate this table, but eh, here it is:
		{ 0, -1 },
		{ 1, -1 },
		{ 1, 0 },
		{ 1, 1 },
		{ 0, 1 },
		{ -1, 1 },
		{ -1, 0 },
		{ -1, -1 },
	};

	// Check in all 8 directions for "MAS"
	for(size_t j = 0; j < 8; j++) {
		for(size_t i = 0, len = strlen(mas); i < len; i++) {
			// Lookup in the table which offset we need to check (multiplied by i + 1):
			// This has a problem with diagonals (even j)
			int offset[2] = { 
				direction_lookup[j][0] * (i + 1), 
				direction_lookup[j][1] * (i + 1)
			};
			// Check if that's the letter in MAS that we're expecting
			// If it isn't, then skip adding one to matches
			ptrdiff_t x = offset[0] + search_x;
			if (x >= WIDTH || x < 0) {
				goto skip_add;
			}

			ptrdiff_t y = offset[1] + search_y;
			if (y >= HEIGHT || y < 0) {
				goto skip_add;
			}

			printf("\tOffset: %d, %d\n", offset[0], offset[1]);
			printf("\tChecking %td, %td, direction %zu: %c, mas: %c\n", x, y, j, word_search[y][x], mas[i]);
			
			if(word_search[y][x] != mas[i]) {
				goto skip_add;
			}
			
		}
		matches++;
skip_add:
		(void)0; // (do nothing in the skip_add case)
	}
	printf("%d matches at %zu, %zu\n", matches, search_x, search_y);
	return matches;
}

bool check_a(size_t search_x, size_t search_y, char word_search[HEIGHT][WIDTH + 1])
{
	// top left and bottom right must have an M and S
	// bottom right and top left must have and M and S
	
	const int nw_lookup[2][2] = {
		{ -1, -1 },
		{ 1, 1 }
	};

	const int se_lookup[2][2] = {
		{ 1, -1 },
		{ -1, 1 }
	};

	bool nw_m = false;
	bool nw_s = false;

	bool se_m = false;
	bool se_s = false;

	for(size_t i = 0; i < 2; i++) {
		ptrdiff_t x = nw_lookup[i][0] + search_x;
		if (x < 0 || x >= WIDTH) {
			return false;	
		}

		ptrdiff_t y = nw_lookup[i][1] + search_y;
		if (y < 0 || y >= HEIGHT) {
			return false;	
		}

		if (word_search[y][x] == 'M') {
			nw_m = true;
		}

		if (word_search[y][x] == 'S') {
			nw_s = true;
		}
	}

	for(size_t i = 0; i < 2; i++) {
		ptrdiff_t x = se_lookup[i][0] + search_x;
		if (x < 0 || x >= WIDTH) {
			return false;	
		}

		ptrdiff_t y = se_lookup[i][1] + search_y;
		if (y < 0 || y >= HEIGHT) {
			return false;	
		}

		if (word_search[y][x] == 'M') {
			se_m = true;
		}

		if (word_search[y][x] == 'S') {
			se_s = true;
		}
	}
	if (nw_m && nw_s && se_m && se_s) {
		printf("Match at %zu, %zu\n", search_x, search_y);
		return true;
	}
	return false;
}

void part_one(char word_search[HEIGHT][WIDTH + 1])
{
	int count = 0;
	for (size_t y = 0; y < HEIGHT; y++) {
		for (size_t x = 0; x < WIDTH; x++) {
			if (word_search[y][x] == 'X') {
				count += check_x(x, y, word_search);
			}
		}
	}
	printf("%d\n", count);
}

void part_two(char word_search[HEIGHT][WIDTH + 1])
{
	int count = 0;
	for (size_t y = 0; y < HEIGHT; y++) {
		for (size_t x = 0; x < WIDTH; x++) {
			if (word_search[y][x] == 'A') {
				if (check_a(x, y, word_search)) {
					count++;
				}
			}
		}
	}
	printf("%d\n", count);
}

int main()
{
	char word_search[HEIGHT][WIDTH + 1] = { 0 };		
	FILE *f = fopen("input.txt", "r");
	if (f == NULL) {
		fprintf(stderr, "Can't open input.txt!\n");
		return 1;
	}
	int c = 0;
	size_t x = 0;
	size_t y = 0;
	while ((c = fgetc(f)) != EOF) {
		if (c == '\n') {
			 y++;
			 x = 0;
			 continue;
		}
		word_search[y][x] = c;
		x++;
	}
	fclose(f);
	// part_one(word_search);
	part_two(word_search);
}
