#ifndef DSEQ_H_
#define DSEQ_H_

#include <stdlib.h>
#include <string.h>
#include "dvector.h"

const uint8_t nt4map[256] =
{
    4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,
    4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,
    4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,
    4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,

    4, 0, 4, 1,  4, 4, 4, 2,  4, 4, 4, 4,  4, 4, 0, 4,
    4, 4, 4, 4,  3, 3, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,
    4, 0, 4, 1,  4, 4, 4, 2,  4, 4, 4, 4,  4, 4, 0, 4,
    4, 4, 4, 4,  3, 3, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,

    4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,
    4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,
    4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,
    4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,

    4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,
    4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,
    4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,
    4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4,  4, 4, 4, 4
};

const char nt4char[4] = {'A', 'C', 'G', 'T'};

#define NT4CODE(c) (nt4map[((uint64_t)(c))])
#define NT4CHAR(e) (nt4char[(e)])

typedef struct
{
    size_t num_symbols, slots_used, slots_avail;
    uint8_t *slots;
    int cur_slot_offset;
} dseq_s, *dseq;

dseq dseq_init()
{
    dseq ds             = malloc(sizeof(*ds));
    ds->num_symbols     = 0;
    ds->slots_used      = 0;
    ds->slots_avail     = 1;
    ds->slots           = malloc(ds->slots_avail);
    ds->cur_slot_offset = 0;

    return ds;
}

void dseq_push(dseq ds, const char *symbols)
{
    size_t slen = strlen(symbols);

    size_t i = 0;

    size_t slots_needed = u32rndup(1 + ds->slots_used + (slen/4));

    if (ds->cur_slot_offset != 0)
    {
        for (; ds->cur_slot_offset != 0; ds->cur_slot_offset %= 4)
        {
            ds->slots[ds->slots_used] |= NT4CODE(symbols[i++]) << 2*ds->cur_slot_offset++;
            ds->num_symbols++;
        }

        ds->slots_used++;
    }

    if (i == slen)
        return;

    if (slots_needed > ds->slots_avail)
    {
        ds->slots_avail = slots_needed;
        ds->slots = realloc(ds->slots, ds->slots_avail);
    }

    while (i < slen)
    {
        int vv = NT4CODE(symbols[i++]) << 2*ds->cur_slot_offset++;
        ds->slots[ds->slots_used] |= vv;
        ds->num_symbols++;
        ds->cur_slot_offset %= 4;
        if (ds->cur_slot_offset == 0)
            ds->slots_used++;
    }
}

void dseq_read(dseq ds, char **symbols)
{
    *symbols = malloc(ds->num_symbols + 1);
    char *p = *symbols;

    int r = 0, s = 0;
    for (size_t i = 0; i < ds->num_symbols; ++i)
    {
        *p++ = NT4CHAR((ds->slots[s] >> (2*r++))&3);
        r %= 4;
        s = (!r)? s+1 : s;
    }

    *p = 0;
}

#endif
