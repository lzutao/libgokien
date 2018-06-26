#include <stdlib.h>
#include <string.h>

#include "char.h"

int gokien_char_new(struct GokienChar **c,
                     const char   *chchar,
                     const char   *simpchar,
                     const char   *code,
                     uint32_t      frequency) {
    struct GokienChar *tmp = calloc(1, sizeof(*tmp));
    if (tmp == NULL) {
        return GOKIEN_NOMEM;
    }

    // Copy at most 4 bytes, it's a single UTF-8 encoded character
    strncpy(tmp->chchar, chchar, 4);

    // Copy at most 4 bytes, it's a single UTF-8 encoded character
    strncpy(tmp->simpchar, simpchar, 4);

    // Copy at most 5 bytes, that's the longest a Gokien input code can be
    strncpy(tmp->code, code, 5);

    tmp->frequency = frequency;

    *c = tmp;

    return GOKIEN_OK;
}

int gokien_char_free(struct GokienChar *c) {
    free(c);

    return GOKIEN_OK;
}
