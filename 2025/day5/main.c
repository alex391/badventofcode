#include <stdint.h>
#include "slice.c"
#include "lines.c"
#include "min_max.c"
#include <stdckdint.h>

struct range {
	i64 lower;
	i64 upper;
};

struct range vec_get_range(struct vec *vec, u32 index)
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

void vec_set_range(struct vec *vec, u32 index, struct range in)
{
	panic_if_not_equal(vec->item_size, sizeof(struct range));
	vec_set(vec, index, &in);
}

void vec_print_ranges(struct vec *vec)
{
	for (u32 i = 0; i < vec->len; i++) {
		struct range this = vec_get_range(vec, i);
		printf(Pi64 ", " Pi64 "\n", this.lower, this.upper);
	}
}

i32 range_comp_lower(const void *lhs, const void *rhs)
{
	struct range lhs_as_range = *(const struct range *)lhs;
	struct range rhs_as_range = *(const struct range *)rhs;

	if (lhs_as_range.lower < rhs_as_range.lower) {
		return -1;
	}

	if (lhs_as_range.lower > rhs_as_range.lower) {
		return 1;
	}

	if (lhs_as_range.upper < rhs_as_range.upper) {
		return -1;
	}

	if (lhs_as_range.upper > rhs_as_range.upper) {
		return 1;
	}

	return 0;
}

i32 range_comp_upper(const void *lhs, const void *rhs)
{
	struct range lhs_as_range = *(const struct range *)lhs;
	struct range rhs_as_range = *(const struct range *)rhs;

	if (lhs_as_range.upper < rhs_as_range.upper) {
		return -1;
	}

	if (lhs_as_range.upper > rhs_as_range.upper) {
		return 1;
	}

	if (lhs_as_range.lower < rhs_as_range.lower) {
		return -1;
	}

	if (lhs_as_range.lower > rhs_as_range.lower) {
		return 1;
	}

	return 0;
}


b8 in_range(struct range range, i64 test)
{
	if (test < range.lower) {
		return false;
	}
	if (test > range.upper) {
		return false;
	}
	return true;
}

struct range range_from_str(struct str *s)
{
	auto split = split_new(s, (struct str) { .s = "-", .len = 1 });

	auto lower = str_to_i64(slice_to_str(vec_get_slice(split, 0), s->s));
	auto upper = str_to_i64(slice_to_str(vec_get_slice(split, 1), s->s));
	
	vec_free(split);

	return (struct range){ .lower = lower, .upper = upper };
}


int main()
{
	auto lines = lines_new("input.txt");

	auto ranges = vec_new(sizeof(struct range));

	u32 i = 0;
	for (; i < lines->len; i++) {
		auto line = vec_get_shortstr(lines, i);
		if (line.len == 0) {
			break;
		}
		auto line_as_str = shortstr_to_str(line);
		auto range = range_from_str(&line_as_str);
		vec_push_range(ranges, range);
	}
	vec_free(lines);
	
	vec_sort(ranges, range_comp_lower);

	// vec_print_ranges(ranges);
	// printf("--------------------\n");

	struct vec *ranges_merged = vec_new(sizeof(struct range));

	for (u32 i = 0; i < ranges->len; i++) {
		struct range this = vec_get_range(ranges, i);
		if (ranges_merged->len > 0) {
			// Check if you should merge, then merge
			struct range previous = vec_get_range(ranges_merged, ranges_merged->len - 1);
			b8 should_merge = in_range(previous, this.lower);
			if (!should_merge) {
				goto just_add;
			}
			previous.upper = max(previous.upper, this.upper);
			vec_set_range(ranges_merged, ranges_merged->len - 1, previous);
			continue;
		}
just_add:
		vec_push_range(ranges_merged, this);
	}
	vec_free(ranges);

	// vec_print_ranges(ranges_merged);

	i64 fresh_ids = 0;
	for (u32 i = 0; i < ranges_merged->len; i++) {
		struct range this = vec_get_range(ranges_merged, i);
		panic_if_equal(ckd_add(&fresh_ids, fresh_ids, this.upper - this.lower + 1), true);
	}

	printf("%" PRIi64 "\n", fresh_ids);
}
