#include <stdio.h>
#include "dseq.h"

int main(int argc, char *argv[])
{
    dseq ds = dseq_init();

    dseq_push(ds, "a");
    dseq_push(ds, "c");
    dseq_push(ds, "g");
    dseq_push(ds, "t");
    dseq_push(ds, "UGCA");
    dseq_push(ds, "UGC");
    dseq_push(ds, "AGct");

    char *s = malloc(dseq_num_symbols(ds) + 1);
    dseq_read(ds, s);
    printf("%s\n", s);
    free(s);
    return 0;
}
