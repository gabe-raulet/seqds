#include "dseq.h"

struct dseq_s
{
    size_t num_symbols, slots_filled, slots_avail;
    uint8_t *slots;
    int cur_slot_offset;
};

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

dseq dseq_init()
{
    dseq ds             = malloc(sizeof(*ds));
    ds->num_symbols     = 0;
    ds->slots_filled    = 0;
    ds->slots_avail     = 1;
    ds->slots           = malloc(ds->slots_avail);
    ds->cur_slot_offset = 0;

    return ds;
}

void dseq_push(dseq ds, const char *symbols)
{
    size_t slen = strlen(symbols);
    size_t i = 0;

    if (ds->cur_slot_offset > 0)
    {
        for (; i < slen && ds->cur_slot_offset != 0; ds->cur_slot_offset %= 4)
        {
            ds->slots[ds->slots_filled] |= NT4CODE(symbols[i++]) << 2*ds->cur_slot_offset++;
            ds->num_symbols++;
        }

        if (ds->cur_slot_offset == 0)
            ds->slots_filled++;
    }

    if (i == slen)
        return;

    size_t slots_needed = ds->slots_filled + (1 + ((slen - i) / 4));
    slots_needed = u32rndup(slots_needed);

    if (slots_needed > ds->slots_avail)
    {
        ds->slots_avail = slots_needed;
        ds->slots = realloc(ds->slots, ds->slots_avail);
    }

    while (i < slen)
    {
        ds->slots[ds->slots_filled] |= NT4CODE(symbols[i++]) << 2*ds->cur_slot_offset++;
        ds->num_symbols++;
        ds->cur_slot_offset %= 4;
        ds->slots_filled = (!ds->cur_slot_offset) ? ds->slots_filled + 1 : ds->slots_filled;
    }
}

void dseq_read(dseq ds, char *symbols)
{
    char *p = symbols;

    int r, s;
    r = s = 0;

    for (size_t i = 0; i < ds->num_symbols; ++i)
    {
        *p++ = NT4CHAR(((ds->slots[s]) >> (2*r++))&3);
        r %= 4;
        s = (!r) ? s+1 : s;
    }

    *p = 0;
}

void dseq_shrink_to_fit(dseq ds)
{
   if (ds->slots_avail != ds->slots_filled)
   {
       ds->slots_avail = ds->slots_filled;
       ds->slots = realloc(ds->slots, ds->slots_avail);
   }
}

size_t dseq_num_symbols(const dseq ds) { return ds->num_symbols; }
size_t dseq_slots_filled(const dseq ds) { return ds->slots_filled; }
size_t dseq_slots_avail(const dseq ds) { return ds->slots_avail; }
