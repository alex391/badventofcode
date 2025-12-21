#include <stdint.h>
#include "slice.c"
#include "lines.c"

struct range {
	int64_t lower;
	int64_t upper;
};


struct range vec_get_range(struct vec *vec, uint32_t index)
{
	panic_if_not_equal(vec->item_size, sizeof(struct range));
	struct range out = { };
	vec_get(vec, index, &out);
	return out;
}

void vec_push_range(struct vec *vec, struct range range)
{
	panic_if_not_equal(vec->item_size, sizeof(struct range));
	vec_push(vec, &range);
}


bool in_range(struct range range, int64_t test)
{
	if (test < range.lower) {
		return false;
	}
	if (test > range.upper) {
		return false;
	}
	return true;
}

bool in_any_range(struct vec *ranges, int64_t test)
{
	for (uint32_t i = 0; i < ranges->len; i++) {
		auto range = vec_get_range(ranges, i);
		if (in_range(range, test)) {
			return true;
		}
	}
	return false;
}

struct range range_from_str(struct str *s)
{
	auto split = split_new(s, (struct str) { .s = "-", .len = 1 });

	auto lower = str_to_int64_t(slice_to_str(vec_get_slice(split, 0), s->s));
	auto upper = str_to_int64_t(slice_to_str(vec_get_slice(split, 1), s->s));
	
	vec_free(split);

	return (struct range){ .lower = lower, .upper = upper };
}


int main()
{
	auto lines = lines_new("input.txt");

	auto ranges = vec_new(sizeof(struct range));

	uint32_t i = 0;
	for (; i < lines->len; i++) {
		auto line = vec_get_shortstr(lines, i);
		if (line.len == 0) {
			break;
		}
		auto line_as_str = shortstr_to_str(line);
		vec_push_range(ranges, range_from_str(&line_as_str));
	}

	int64_t fresh_count = 0;
	for (; i < lines->len; i++) {
		auto line = vec_get_shortstr(lines, i);
		auto id = str_to_int64_t(shortstr_to_str(line));
		if(in_any_range(ranges, id)) {
			fresh_count++;
		}
	}
	vec_free(ranges);
	vec_free(lines);
	printf("%" PRIi64 "\n", fresh_count);
}
