#include <stdio.h>
#include "seqstream.h"
#include "dseq.h"

int main(int argc, char *argv[])
{
    seqstream stream = seqstream_init("ecoli_reads.fa");

    char *name, *seq;
    dseq ds = dseq_init();

    while (seqstream_read(stream, ds, &name) != -1)
    {
        seq = dseq_read(ds);
        printf("%s\n", seq);
        free(seq);
        free(name);
        dseq_clear(ds);
    }

    dseq_free(ds);
    seqstream_free(stream);
    return 0;
}
