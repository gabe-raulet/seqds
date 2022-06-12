#ifndef DVECTOR_H_
#define DVECTOR_H_

/* dvector - simple dynamic vector header for C programs */

#include <stdlib.h>
#include <string.h>

static inline uint32_t u32rndup(uint32_t x)
{
    --x;
    x |= (x>>1);
    x |= (x>>2);
    x |= (x>>4);
    x |= (x>>8);
    x |= (x>>16);
    return ++x;
}

#define dvector_type(type) struct { type *a; size_t l, m; }
#define dvector_init(v) ((v).l = (v).m = 0, (v).a = 0)
#define dvector_free(v) (free((v).a))
#define dvector_reserve(v, n) do { (v).m = u32rndup((n)), (v).a = realloc((v).a, *(v).a) * (v).m; } while (0)
#define dvector_at(v, i) ((v).a[(i)])
#define dvector_cap(v) ((v).m)
#define dvector_size(v) ((v).l)
#define dvector_empty(v) (!(v).l)
#define dvector_pop(v) ((v).a[--(v).l])
#define dvector_push(v, x) do { if ((v).l + 1 >= (v).m) { (v).m = u32rndup((v).l + 1); dvector_reserve(v, (v).m); } (v).a[(v).l++] = (x); } while (0)

#endif
