#include <stdio.h>
#include "dseq.h"

int main(int argc, char *argv[])
{
    dseq ds = dseq_init();

    dseq_push(ds, "TTTTTTTTTTTTAATT");
    char *symbols = dseq_read(ds);

    printf("%s\n", symbols);
    free(symbols);
    return 0;
}
