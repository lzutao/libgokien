#ifndef GOKIEN_CHAR_H
#define GOKIEN_CHAR_H

#include "common.h"

GOKIEN_BEGIN_DECL

struct GokienChar {
    char chchar[5];
    char simpchar[5];
    char code[6];
};

GOKIEN_EXTERN struct GokienChar *
gokien_char_new(char const *chchar,
               char const *simpchar,
               char const *code);

GOKIEN_EXTERN int
gokien_char_free(struct GokienChar *c);

GOKIEN_END_DECL

#endif /* GOKIEN_CHAR_H */
