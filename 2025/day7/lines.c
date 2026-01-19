#include "vec.c"

// returns a new vec of shortstrs
struct vec *lines_new(const c8 *filename) 
{
	FILE *f = panic_if_zero(fopen(filename, "rb"));
	struct vec *lines = vec_new(sizeof(union shortstr));
	
	union shortstr s = { 0 };
	i32 c;
	while(true) {
		while (s.len < UINT8_MAX) {
			c = getc(f);
			if (c == '\n') {
				i32 peeked = fgetc(f);
				if (peeked == EOF) {
					// polite \n at end of file
					c = EOF;
					break;
				}
				ungetc(peeked, f);
				break;
			}
			if (c == EOF) {
				break;
			}
			s.data[s.len] = c;
			s.len++;
		}
		vec_push_shortstr(lines, s);
		s = (union shortstr){ 0 };
		if (c == EOF) {
			break;
		}
	}
	fclose(f);
	return lines;
}
