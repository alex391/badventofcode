#include "read_file.c"

#define PCRE2_CODE_UNIT_WIDTH 8

#include <stdio.h>
#include <string.h>
#include <pcre2.h>


// Thanks to https://pcre2project.github.io/pcre2/doc/pcre2demo/

bool invalid(int64_t id, PCRE2_SPTR pattern)
{
	struct str *id_as_str = int64_t_to_new_str(id);

	bool result = false;

	pcre2_code *re;
	PCRE2_SPTR subject = (PCRE2_SPTR)id_as_str->s;
	PCRE2_SIZE subject_length = (PCRE2_SIZE)id_as_str->len;

	int errornumber = 0;
	PCRE2_SIZE erroroffset = 0;
	pcre2_match_data *match_data;

	re = pcre2_compile(
		pattern,
		PCRE2_ZERO_TERMINATED,
		0,
		&errornumber,
		&erroroffset,
		NULL
	);

	if (re == NULL) {
		PCRE2_UCHAR buffer[256] = { 0 };
		pcre2_get_error_message(errornumber, buffer, sizeof(buffer));
		fprintf(stderr, "PCRE2 compilation failed at offset %zu: %s\n", erroroffset, buffer);
		exit(1);
	}

	match_data = pcre2_match_data_create_from_pattern(re, NULL);

	int rc = 0;
	rc = pcre2_match(
		re,
		subject,
		subject_length,
		0,
		0,
		match_data,
		NULL
	);

	if (rc < 0) {
		switch(rc) {
			case PCRE2_ERROR_NOMATCH:
				goto cleanup;
			default:
				fprintf(stderr, "Matching error: %d\n", rc);
				exit(1);
		}
	}
	result = true;
cleanup:
	pcre2_match_data_free(match_data);
	pcre2_code_free(re);
	str_free(id_as_str);
	return result;
}

int main()
{
	PCRE2_SPTR pattern;
	pattern = (PCRE2_SPTR)"^([0-9]+)\\1+$";

	struct str *file = str_new_from_file("input.txt");
	// drop newline
	file->len--;
	struct vec *split = split_new(file, (struct str) { .s = ",-", .len = 2 });

	int64_t invalid_ids_sum = 0;

	for (uint32_t i = 0; i < split->len; i += 2) {
		int64_t first_number = str_to_int64_t(slice_to_str(vec_get_slice(split, i), file->s));
		int64_t second_number = str_to_int64_t(slice_to_str(vec_get_slice(split, i + 1), file->s));
		for (int64_t j = first_number; j <= second_number; j++) {
			if (invalid(j, pattern)) {
//				printf("%" PRIi64 "\n", j);
				invalid_ids_sum += j;
			}
		}
	}

	printf("%" PRIi64 "\n", invalid_ids_sum);

	vec_free(split);
	str_free(file);
}
