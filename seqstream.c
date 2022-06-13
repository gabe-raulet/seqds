#include "seqstream.h"
#include "dvector.h"
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <assert.h>

typedef dvector_type(char) charstring;

#define BUFLEN (16384)

struct seqstream
{
    char *buf;
    int fd, start, end, is_eof;
};

seqstream seqstream_init(const char *filename)
{
    int fd = open(filename, O_RDONLY);

    if (fd < 0) return NULL;

    seqstream stream = malloc(sizeof(*stream));
    stream->fd = fd;
    stream->start = stream->end = 0;
    stream->is_eof = 0;
    stream->buf = malloc(BUFLEN);

    return stream;
}

void seqstream_free(seqstream stream)
{
    close(stream->fd);
    free(stream->buf);
    free(stream);
}

int seqstream_peek(seqstream stream)
{
    if (stream->is_eof)
        return -1;

    if (stream->start >= stream->end)
    {
        stream->start = 0;
        stream->end = read(stream->fd, stream->buf, BUFLEN);
        if (stream->end < 0) { stream->is_eof = -1; return -1; }
        if (!stream->end) { stream->is_eof = 1; return -1; }
    }

    return stream->buf[stream->start];
}

int seqstream_getc(seqstream stream)
{
    int c = seqstream_peek(stream);
    stream->start++;
    return c;
}


int seqstream_fastforward(seqstream stream, int delim)
{
    /* fastforward stream to point one symbol to the right of @delim */

    int c;

    while ((c = seqstream_getc(stream)) >= 0)
        if (c == delim)
            return c;

    return -1;
}

int seqstream_collect(seqstream stream, charstring *collect, int delim, int nullterminate)
{
    char c;

    for (;;)
    {
        c = seqstream_getc(stream);

        if (c < 0)
            break;

        if (c == delim)
            break;

        dvector_push(*collect, c);
    }

    if (nullterminate)
        dvector_push(*collect, '\0');

    if (c == delim)
        return 0;

    return -1;
}

int seqstream_read(seqstream stream, dseq ds, char **name)
{
    charstring name_collect, seq_collect;
    dvector_init(seq_collect);
    dvector_init(name_collect);

    if (seqstream_fastforward(stream, '>') < 0)
        return -1;

    seqstream_collect(stream, &name_collect, '\n', 1);

    while (seqstream_collect(stream, &seq_collect, '\n', 0) != -1)
        if (seqstream_peek(stream) == '>')
            break;

    dvector_push(seq_collect, '\0');

    dseq_push(ds, seq_collect.a);

    if (name) *name = strdup(name_collect.a);

    return 0;
}
