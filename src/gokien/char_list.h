#ifndef GOKIEN_CHARLIST_H
#define GOKIEN_CHARLIST_H

#include "common.h"
#include "char.h"

GOKIEN_BEGIN_DECL

struct GokienCharList {
    struct GokienChar *c;
    struct GokienCharList *prev;
    struct GokienCharList *next;
};

GOKIEN_EXTERN int
gokien_char_list_append(struct GokienCharList **l, struct GokienChar *c);

GOKIEN_EXTERN int
gokien_char_list_prepend(struct GokienCharList **l, struct GokienChar *c);

GOKIEN_EXTERN int
gokien_char_list_free(struct GokienCharList *l);

GOKIEN_END_DECL

#endif /* GOKIEN_CHARLIST_H */
