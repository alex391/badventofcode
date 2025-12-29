#include "ints.c"
#include "vec.c"

i32 compare_ints(const void* a, const void* b)
{
    int arg1 = *(const int*)a;
    int arg2 = *(const int*)b;
 
    if (arg1 < arg2) return -1;
    if (arg1 > arg2) return 1;
    return 0;
 
}
 
int main() {
	struct vec *ints = vec_new(sizeof(i32));
	
	for (i32 i = 100; i-- > -100;){
		vec_push_i32(ints, i);
	}

	vec_sort(ints, compare_ints);

	for (u32 i = 0; i < ints->len; i++) {
		printf("%d\n", vec_get_i32(ints, i));
	}
}

