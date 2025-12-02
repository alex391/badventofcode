#include "vec.c"

// returns a new vec of shortstrs
struct vec *lines_new(FILE *f)
{
	struct vec *lines = vec_new(sizeof(union shortstr));
	
	union shortstr s = { 0 };
	int c;
	while(true) {
		while (s.len < UINT8_MAX) {
			c = getc(f);
			if (c == '\n') {
				int peeked = fgetc(f);
				if (peeked == EOF) {
					// polite \n at end of file
					return lines;
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
			return lines;
		}
	}
}
