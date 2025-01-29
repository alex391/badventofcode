#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

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
		int px = 0;
		int py = 0;
		int vx = 0;
		int vy = 0;
		char buff[20] = { 0 };
		if (fgets(buff, sizeof(buff), f) == NULL) {
			if (feof(f)) {
				break;
			}
			fprintf(stderr, "File error: fgets read error");
			exit(3);
		}
		sscanf(buff, "p=%d,%d v=%d,%d", &px, &py, &vx, &vy);
		this_robot->px = px;
		this_robot->py = py;
		this_robot->vx = vx;
		this_robot->vy = vy;

		this_robot++;
	}
	fclose(f);
}
