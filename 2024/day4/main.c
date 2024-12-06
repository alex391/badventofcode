#include <stdio.h>

#define WIDTH 10
#define HEIGHT 10

#define MASK_COUNT 8
#define MASK_WIDTH 4
#define MASK_HEIGHT 4

// characters X, M, A, S, or '.' for ignore
// '\0' at the end
//         mask count
//         |           y
//         |           |  x
//         v           v  v
char masks[MASK_COUNT][MASK_HEIGHT][MASK_WIDTH + 1] = { 
	{
		"XMAS", 
		"....",
		"....",
		"...."
	}, // 0
	{
		"SAMX", 
		"....",
		"....",
		"...."
	}, // 1
	{
		"X...", 
		"M...",
		"A...",
		"S..."
	}, // 2
	{
		"S...", 
		"A...",
		"M...",
		"X..."
	}, // 3
	{
		"X...", 
		".M..",
		"..A.",
		"...S"
	}, // 4
	{
		"S...", 
		".A..",
		"..M.",
		"...X"
	}, // 5
	{
		"...X", 
		"..M.",
		".A..",
		"S..."
	}, // 6
	{
		"...S", 
		"..A.",
		".M..",
		"X..."
	}, // 7

};

int check_masks(size_t search_x, size_t search_y, char word_search[HEIGHT][WIDTH + 1])
{
	int matches = 0;
	for (size_t i = 0; i < MASK_COUNT; i++) {
		int matched = 0;
		for (size_t y = 0; y < MASK_HEIGHT; y++) {
			for (size_t x = 0; x < MASK_WIDTH; x++) {
				char testing = masks[i][x][y];
				if (testing == '.') {
					continue;
				} 
				char word_search_letter = word_search[search_y + y][search_x + x];
				if (word_search_letter == testing) {
					matched++;
				}
			}

		}
		if (matched == 4) {
			matches++;
			printf("match at (%zu, %zu), mask %zu\n", search_x, search_y, i);
		}
	}

	return matches;
}

void part_one(char word_search[HEIGHT][WIDTH + 1])
{
	int count = 0;
	for (size_t y = 0; y < HEIGHT; y++) {
		for (size_t x = 0; x < WIDTH; x++) {
			count += check_masks(x, y, word_search);
		}
	}
	printf("%d\n", count);
}
int main()
{
	char word_search[HEIGHT][WIDTH + 1] = { 0 };		
	FILE *f = fopen("input.txt", "r");
	if (f == NULL) {
		fprintf(stderr, "Can't open input.txt!");
		return 1;
	}
	int c = 0;
	size_t x = 0;
	size_t y = 0;
	while ((c = fgetc(f)) != EOF) {
		if (c == '\n') {
			 y++;
			 x = 0;
		}
		word_search[y][x] = c;
		x++;
	}
	fclose(f);
	part_one(word_search);
}
