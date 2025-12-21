#include <stdint.h>
// Source - https://stackoverflow.com/a/14997413
// Posted by Martin B, modified by community. See post 'Timeline' for change history
// Retrieved 2025-12-01, License - CC BY-SA 3.0

int32_t positive_modulo(int32_t i, int32_t n) {
    return (i % n + n) % n;
}
