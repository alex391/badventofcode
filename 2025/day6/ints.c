#include <stdint.h>
#include <inttypes.h>

// All the reasonable integer types are in here, no need to use anything else or
// think too hard about how big "long", what the differnece between size_t and
// uintptr_t is, and just less typing

// unsigned
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

// signed
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

// boolean
typedef i8 b8;

// character
typedef char c8;

// pointer sized
typedef uintptr_t usize;
typedef intptr_t isize;

// for the others, just use %d or %u
#define Pu64 "%" PRIu64
#define Pi64 "%" PRIi64
#define Pisize "%" PRIiPTR
#define Pusize "%" PRIuPTR
