#include "grid.c"
#include "deque.c"

constexpr u32 DEQUE_SIZE = 10000;

union packed_coordinate {
	struct {
		i32 x;
		i32 y;
	};
	i64 packed;
};

// takes in an array to store the edges in, and returns the number of edges
// actually found
i32 adjacent_edges(struct grid *grid, union packed_coordinate vertex, union packed_coordinate edges[2])
{
	union packed_coordinate below_coordinate = {
		.x = vertex.x + ORDINAL_DIRECTIONS[SOUTH].x, 
		.y = vertex.y + ORDINAL_DIRECTIONS[SOUTH].y
	};
	i16 below = grid_get(grid, vertex.x + ORDINAL_DIRECTIONS[SOUTH].x, vertex.y + ORDINAL_DIRECTIONS[SOUTH].y);
	if (below == -1)
	{
		return 0;
	}
	if (below == '.' || below == '|') {
		edges[0] = below_coordinate;
		return 1;
	}
	if (below == '^') {
		union packed_coordinate down_left_coordinate = {
			.x = vertex.x + ORDINAL_DIRECTIONS[SOUTH_WEST].x,
			.y = vertex.y + ORDINAL_DIRECTIONS[SOUTH_WEST].y
		};
		union packed_coordinate down_right_coordinate = {
			.x = vertex.x + ORDINAL_DIRECTIONS[SOUTH_EAST].x,
			.y = vertex.y + ORDINAL_DIRECTIONS[SOUTH_EAST].y
		};
		edges[0] = down_left_coordinate;
		edges[1] = down_right_coordinate;
		return 2;
	}
	panic_unconditionally_line(__LINE__);
}

i64 bfs(struct grid *grid, i32 x, i32 y)
{
	i64 timelines = 1;
	// Thanks to https://en.wikipedia.org/wiki/Breadth-first_search
	i64 *buffer = panic_if_zero(calloc(DEQUE_SIZE, sizeof(i64)));
	struct i64_deque q = i64_deque_new(buffer, DEQUE_SIZE);
	grid_set(grid, x, y, '|');
	i64_deque_push_back(&q, (union packed_coordinate) { .x = x, .y = y }.packed);
	while(q.len > 0)
	{	
		// value is safe because len > 0 here
		union packed_coordinate v = (union packed_coordinate) { .packed = i64_deque_pop_front(&q).value };
		i16 grid_value = grid_get(grid, v.x, v.y);
		panic_if_equal_line(grid_value, -1, __LINE__);
                // the adjacent edges are either just the one below, or if
                // there's a ^ below, then the one to the right and the left of
                // that
		// we also get +1 timeline if there's 2 ways to go 
		union packed_coordinate edges[2] = { };	
		i32 edges_count = adjacent_edges(grid, v, edges);
		if (edges_count == 2) {
			timelines++;
		}
		for (i32 i = 0; i < edges_count; i++) {
			auto w = grid_get(grid, edges[i].x, edges[i].y);
			panic_if_equal_line(w, -1, __LINE__);
			if (w == '.' || w == '|'){
				grid_set(grid, edges[i].x, edges[i].y, '|');
				i64_deque_push_back(&q, (union packed_coordinate) { .x = edges[i].x, y = edges[i].y }.packed);
			}
		}
        }
	free(buffer);

	return timelines;
}

int main()
{
	auto grid = grid_new_from_file("input.txt");

	isize s_x = 0;
	for (; s_x < grid->width; s_x++) {
		if (grid_get(grid, s_x, 0) == 'S') {
			break;
		}
	}

	auto splits_count = bfs(grid, s_x, 0);
	grid_print(grid);
	grid_free(grid);
	printf(Pi64, splits_count);
}
