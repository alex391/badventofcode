#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


struct point {
	int x;
	int y;
};

struct robot {
	struct point p;
	struct point v;
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
	rewind(f);
	struct robot *robots = malloc(lines * sizeof(struct robot));
	struct robot *this_robot = robots;
	while(true) {
		int s = fscanf(f, "p=%d,%d v=%d,%d", &this_robot->p.x,
				&this_robot->p.y, &this_robot->v.x, &this_robot->v.y);
		if (s == EOF) {
			break;
		}
		this_robot++;
	}
	fclose(f);
}
