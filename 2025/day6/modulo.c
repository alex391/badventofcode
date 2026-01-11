#include "ints.c"
// Source - https://stackoverflow.com/a/14997413
// Posted by Martin B, modified by community. See post 'Timeline' for change history
// Retrieved 2025-12-01, License - CC BY-SA 3.0

i32 positive_modulo(i32 i, i32 n) {
    return (i % n + n) % n;
}
