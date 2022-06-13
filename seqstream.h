#ifndef SEQSTREAM_H_
#define SEQSTREAM_H_

#include "dseq.h"

typedef struct dseqstore *dseqstore;
typedef struct seqstream *seqstream;

seqstream seqstream_init(const char *filename);
void seqstream_free(seqstream stream);
int seqstream_read(seqstream stream, dseq ds, char **name);

#endif
