#include "ints.c"
#include "slice.c"

// Thanks to https://stackoverflow.com/a/3381098/12203444

// Get the size in bytes of a file
isize file_size(FILE *f)
{
	panic_if_not_equal(fseek(f, 0, SEEK_END), 0);
	isize size = ftell(f);
	panic_if_equal(size, -1);
	panic_if_not_equal(fseek(f, 0, SEEK_SET), 0);
	return size;
}

// Read the whole file into a new str
struct str *str_new_from_file(const c8 *filename)
{
	FILE *f = panic_if_zero(fopen(filename, "rb"));
	isize size = file_size(f);
	panic_if_greater(size, UINT32_MAX);
	struct str *file_contents = str_new_cleared(size);
	panic_if_less(fread(file_contents->s, 1, size, f), size);
	fclose(f);
	return file_contents;
}
