#include <stdio.h>
#include "dseq.h"

int main(int argc, char *argv[])
{
    dseq ds = dseq_init();

    dseq_push(ds, "CGTACTTCTTGGGGGGcgtAGCTGctgcTAtACTgcTGAgtaccgagcagctACATCATTTCTAGCGACAGGggcGCACATCGTACGTAGTgtgtgTAGCATCGATCGATGTCGCGCGCCCCCCCCCGCGCGCTACGAGCTACGATCGTAGTACGTA");

    char *s;
    dseq_read(ds, &s);
    printf("%s\n", s);
    free(s);
    return 0;
}
