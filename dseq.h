#ifndef DSEQ_H_
#define DSEQ_H_

#include <stdlib.h>
#include <string.h>
#include "dvector.h"

/* Sequences are stored in a chain of one-byte slots. Each byte-long slot can        */
/* store up to 4 symbols. Each symbol is encoded using two bits: 00 for A, 01 for C, */
/* 10 for G, and 11 for T.                                                           */

typedef struct dseq_s dseq_s, *dseq;

dseq dseq_init();
void dseq_push(dseq ds, const char *symbols);
char *dseq_read(dseq ds);
char *dseq_slice(dseq ds, size_t i, size_t j);
void dseq_read_into(dseq ds, char *symbols);
void dseq_slice_into(dseq ds, size_t i, size_t j, char *slice);
void dseq_shrink_to_fit(dseq ds);

size_t dseq_num_symbols(const dseq ds);
size_t dseq_slots_filled(const dseq ds);
size_t dseq_slots_avail(const dseq ds);

#endif
