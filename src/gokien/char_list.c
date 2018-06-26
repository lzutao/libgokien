#include <stdlib.h>

#include "char_list.h"

int gokien_char_list_new_elem(struct GokienCharList **l,
                             struct GokienChar      *c) {
    struct GokienCharList *tmp = calloc(1, sizeof(*tmp));
    if (tmp == NULL) {
        return GOKIEN_NOMEM;
    }

    tmp->c = c;
    tmp->prev = NULL;
    tmp->next = NULL;

    *l = tmp;

    return GOKIEN_OK;
}

int gokien_char_list_append(struct GokienCharList **l,
                             struct GokienChar      *c) {
    struct GokienCharList *new_list;
    struct GokienCharList *last;

    int ret = gokien_char_list_new_elem(&new_list, c);
    if (ret != GOKIEN_OK) {
        return ret;
    }

    if (*l != NULL) {
        last = *l;
        while (last->next) {
            last = last->next;
        }

        last->next = new_list;
        new_list->prev = last;

        return GOKIEN_OK;
    }

    *l = new_list;

    return GOKIEN_OK;
}

int gokien_char_list_prepend(struct GokienCharList **l,
                             struct GokienChar      *c) {
    struct GokienCharList *new_list;

    int ret = gokien_char_list_new_elem(&new_list, c);
    if (ret != GOKIEN_OK) {
        return ret;
    }

    if (*l != NULL) {
        (*l)->prev = new_list;
        new_list->next = *l;
    }

    *l = new_list;

    return GOKIEN_OK;
}

int gokien_char_list_free(struct GokienCharList *l) {
    if (l == NULL) {
        return GOKIEN_OK;
    }

    if (l->next != NULL) {
        gokien_char_list_free(l->next);
    }
    gokien_char_free(l->c);
    free(l);

    return GOKIEN_OK;
}
