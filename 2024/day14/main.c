#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define WIDTH 101
#define HEIGHT 103
#define TIME 100

struct robot {
	int px;
	int py;
	int vx;
	int vy;
};

size_t count_lines(FILE *f)
{
	size_t count = 0;
	int c = 0;
	while((c = getc(f)) != EOF) {
		if (c == '\n') {
			count++;
		}
	}
	return count;
}

int positive_modulo(int i, int n) {
	// https://stackoverflow.com/a/14997413
	return (i % n + n) % n;
}

// 0, 1, 2, 3, or negitive if none
int quadrant(int x, int y)
{
	int x_mid = (HEIGHT / 2) + 1;
	int y_mid = (WIDTH / 2) + 1;
	if (x == x_mid || y == y_mid) {
		return -1;
	}
	if (x < x_mid) {
		if (y < y_mid) {
			return 0;
		}
		return 1;
	}
	if(y < y_mid) {
		return 3;
	}
	return 2;
}

int main()
{
	FILE *f = fopen("input.txt", "r");
	if (!f) {
		fprintf(stderr, "Could not read input.txt\n");
		exit(1);
	}
	size_t lines = count_lines(f);
	if (fseek(f, 0, SEEK_SET) == -1) {
		fprintf(stderr, "File error: Could not seek to the start\n");
		exit(2);
	}
	struct robot *robots = malloc(lines * sizeof(struct robot));
	struct robot *this_robot = robots;
	while(true) {
		char buff[20] = { 0 };
		if (fgets(buff, sizeof(buff), f) == NULL) {
			if (feof(f)) {
				break;
			}
			fprintf(stderr, "File error: fgets read error");
			exit(3);
		}
		int px = 0;
		int py = 0;
		int vx = 0;
		int vy = 0;
		sscanf(buff, "p=%d,%d v=%d,%d", &px, &py, &vx, &vy);
		this_robot->px = px;
		this_robot->py = py;
		this_robot->vx = vx;
		this_robot->vy = vy;

		this_robot++;
	}
	fclose(f);
	for (int t = 0; t < TIME; t++) {
		for (size_t i = 0; i < lines; i++) {
			robots[i].px += robots[i].vx;
			robots[i].py += robots[i].vy;
			
			robots[i].px = positive_modulo(robots[i].px, WIDTH);
			robots[i].py = positive_modulo(robots[i].py, HEIGHT);
		}
	}
	int quadrants[4] = { 0 };
	for (size_t i = 0; i < lines; i++) {
		int q = quadrant(robots[i].px, robots[i].py);
		if (q < 0) {
			continue;
		}
		quadrants[q]++;
	}
	int saftey_factor = quadrants[0] * quadrants[1] * quadrants[2] * quadrants[3];
	printf("Part 1: %d", saftey_factor);
}
