// Loosely based on the deque from Rust
// https://doc.rust-lang.org/src/alloc/collections/vec_deque/mod.rs.html
//
// The following code is
// distributed under the MIT license:
// Copyright (c) The Rust Project Contributors, Alex Leute.
//
// Permission is hereby granted, free of charge, to any
// person obtaining a copy of this software and associated
// documentation files (the "Software"), to deal in the
// Software without restriction, including without
// limitation the rights to use, copy, modify, merge,
// publish, distribute, sublicense, and/or sell copies of
// the Software, and to permit persons to whom the Software
// is furnished to do so, subject to the following
// conditions:
//
// The above copyright notice and this permission notice
// shall be included in all copies or substantial portions
// of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
// ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED
// TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A
// PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT
// SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
// IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.

#include "ints.c"
#include "panic.c"

struct i64_deque {
	i64 *buf;
	u32 capacity;
	u32 head;
	u32 len;
};

// Don't read value if is_none is true
struct option_i64 {
	b8 is_none;
	i64 value;
};

u32 to_physical_idx(struct i64_deque *deque, u32 idx);
i64 *buffer_write(struct i64_deque *deque, u32 off, i64 value);

// Take an empty buffer, make a deque out of it (leave allocation up to the
// caller!) - do try to allocate more than enough space
struct i64_deque i64_deque_new(i64 *buffer, u32 capacity)
{
	return (struct i64_deque) { .buf = buffer, .capacity = capacity, .head = 0, .len = 0 };
}

i64 *i64_deque_push_back(struct i64_deque *deque, i64 value)
{
	auto len = deque->len;
	deque->len++;
	panic_if_greater_line(deque->len, deque->capacity, __LINE__);
	return buffer_write(deque, to_physical_idx(deque, len), value); 
}

i64 *buffer_write(struct i64_deque *deque, u32 off, i64 value)
{
	panic_if_greater(off, deque->capacity);
	i64 *ptr = deque->buf + off;
	*ptr = value;
	return ptr;
}

u32 wrap_index(u32 logical_index, u32 capacity)
{
	panic_if_equal_line(
		(logical_index == 0 && capacity == 0)
			|| logical_index < capacity
			|| (logical_index - capacity) < capacity,
		0,
		__LINE__
	);

	if (logical_index >= capacity) {
		return logical_index - capacity;
	}
	return logical_index;
}

u32 wrap_add(struct i64_deque *deque, u32 idx, u32 addend)
{
	// In rust, wrapping is unusual behivior, but for unsigned ints in c, it's
	// default, so no wrap_add needed
	return wrap_index(idx + addend, deque->capacity);
}

u32 to_physical_idx(struct i64_deque *deque, u32 idx)
{
	return wrap_add(deque, deque->head, idx);
}

// Returns null in the case that index < len
i64 *i64_deque_get(struct i64_deque *deque, u32 index)
{
	if (index < deque->len) {
		auto idx = to_physical_idx(deque, index);
		return deque->buf + idx;
	}
	return nullptr;
}

i64 buffer_read(struct i64_deque *deque, u32 off)
{
	return *(deque->buf + off);
}

struct option_i64 i64_deque_pop_front(struct i64_deque *deque)
{
	if (deque->len == 0) {
		return (struct option_i64){ .is_none = true };
	}
	auto old_head = deque->head;
	deque->head = to_physical_idx(deque, 1);
	deque->len--;
	return (struct option_i64){ .is_none = false, .value = buffer_read(deque, old_head)};
}
