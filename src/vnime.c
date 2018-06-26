#include <stdlib.h>
#include <string.h>

#include "gokien.h"

#define BASE_QUERY "SELECT chchar, simpchar, code, frequency\n" \
                   "FROM chars\n" \
                   "INNER JOIN codes on chars.char_index=codes.char_index\n" \
                   "WHERE version=%d "

// Since the results will be added to a linked list by "prepend",
// the order of results would be the reverse order of this query.
// Hence we use ASC in the order-by-clause to get a DESC order
// in the actual results.
#define ORDER_BY_CLAUSE "ORDER BY frequency ASC"

// Longest possible filter query has a length of 127:
//     " AND ( big5 = 1 OR hkscs = 1 OR punct = 1 OR ... ) "
#define MAX_LEN_FILTER_QUERY 106

// Longest possible code query has a length of 19:
//     "AND code GLOB '%q';"
#define MAX_LEN_CODE_QUERY 19


const char *gokien_radicals[] = {
    "\xE6\x97\xA5", // 日
    "\xE6\x9C\x88", // 月
    "\xE9\x87\x91", // 金
    "\xE6\x9C\xA8", // 木
    "\xE6\xB0\xB4", // 水
    "\xE7\x81\xAB", // 火
    "\xE5\x9C\x9F", // 土
    "\xE7\xAB\xB9", // 竹
    "\xE6\x88\x88", // 戈
    "\xE5\x8D\x81", // 十
    "\xE5\xA4\xA7", // 大
    "\xE4\xB8\xAD", // 中
    "\xE4\xB8\x80", // 一
    "\xE5\xBC\x93", // 弓
    "\xE4\xBA\xBA", // 人
    "\xE5\xBF\x83", // 心
    "\xE6\x89\x8B", // 手
    "\xE5\x8F\xA3", // 口
    "\xE5\xB0\xB8", // 尸
    "\xE5\xBB\xBF", // 廿
    "\xE5\xB1\xB1", // 山
    "\xE5\xA5\xB3", // 女
    "\xE7\x94\xB0", // 田
    "\xE9\x9B\xA3", // 難
    "\xE5\x8D\x9C", // 卜
    "\xEF\xBC\xBA", // Ｚ
};

static void strcat_or_operator(uint32_t *first, char *query) {
    if (! *first) {
        strcat(query, " OR ");
    } else {
        *first = 0;
    }
}

int gokien_get_filter_query(Gokien *cj, char **query) {
    uint32_t first = 1;
    if (cj->filter_flags == 0) {
        // No filter means pass all, so let's return an empty string
        *query = calloc(1, sizeof(char));
        if (query == NULL) {
            return GOKIEN_NOMEM;
        }

        return GOKIEN_OK;
    }

    *query = calloc(MAX_LEN_FILTER_QUERY + 1, sizeof(char));
    if (query == NULL) {
        return GOKIEN_NOMEM;
    }

    strcat(*query, "AND (");

    if (cj->filter_flags & GOKIEN_FILTER_BIG5) {
        strcat(*query, "big5=1");
        first = 0;
    }

    if (cj->filter_flags & GOKIEN_FILTER_HKSCS) {
        strcat_or_operator(&first, *query);
        strcat(*query, "hkscs=1");
    }

    if (cj->filter_flags & GOKIEN_FILTER_PUNCTUATION) {
        strcat_or_operator(&first, *query);
        strcat(*query, "punct=1");
    }

    if (cj->filter_flags & GOKIEN_FILTER_CHINESE) {
        strcat_or_operator(&first, *query);
        strcat(*query, "zh=1");
    }

    if (cj->filter_flags & GOKIEN_FILTER_ZHUYIN) {
        strcat_or_operator(&first, *query);
        strcat(*query, "zhuyin=1");
    }

    if (cj->filter_flags & GOKIEN_FILTER_KANJI) {
        strcat_or_operator(&first, *query);
        strcat(*query, "kanji=1");
    }

    if (cj->filter_flags & GOKIEN_FILTER_KATAKANA) {
        strcat_or_operator(&first, *query);
        strcat(*query, "katakana=1");
    }

    if (cj->filter_flags & GOKIEN_FILTER_HIRAGANA) {
        strcat_or_operator(&first, *query);
        strcat(*query, "hiragana=1");
    }

    if (cj->filter_flags & GOKIEN_FILTER_SYMBOLS) {
        strcat_or_operator(&first, *query);
        strcat(*query, "symbol=1");
    }

    strcat(*query, ") ");

    return GOKIEN_OK;
}

int gokien_new(Gokien        **cj,
                Gokienversion   version,
                unsigned int     filter_flags) {
    char *filter_query;
    int ret;
    char *database_path;
    Gokien *tmp = calloc(1, sizeof(Gokien));
    if (tmp == NULL) {
        return GOKIEN_NOMEM;
    }

    tmp->version = version;
    tmp->filter_flags = filter_flags;

    // Prepare the main Gokien query
    tmp->cj_query = calloc(strlen(BASE_QUERY) + MAX_LEN_FILTER_QUERY + 1,
                             sizeof(char));
    if (tmp->cj_query == NULL) {
        gokien_free(tmp);
        return GOKIEN_NOMEM;
    }

    strcat(tmp->cj_query, BASE_QUERY);

    ret = gokien_get_filter_query(tmp, &filter_query);
    if (ret != GOKIEN_OK) {
        gokien_free(tmp);
        return ret;
    }

    strcat(tmp->cj_query, filter_query);
    free(filter_query);

    // Prepare the query by short code
    tmp->shortcode_query = calloc(strlen(BASE_QUERY) + MAX_LEN_CODE_QUERY +
                                  strlen(ORDER_BY_CLAUSE) + 1, sizeof(char));
    if (tmp->shortcode_query == NULL) {
        gokien_free(tmp);
        return GOKIEN_NOMEM;
    }

    strcat(tmp->shortcode_query, BASE_QUERY);
    strcat(tmp->shortcode_query, "AND code = '%q' ");
    strcat(tmp->shortcode_query, ORDER_BY_CLAUSE);

    // Check the GOKIEN_DB env var (it is useful for local testing)
    database_path = getenv("GOKIEN_DB");
    if (database_path != NULL) {
        ret = sqlite3_open_v2(database_path, &tmp->db, SQLITE_OPEN_READONLY, NULL);
    } else {
        ret = sqlite3_open_v2(GOKIEN_DB, &tmp->db, SQLITE_OPEN_READONLY, NULL);
    }
    if (ret == SQLITE_CANTOPEN) {
        gokien_free(tmp);
        return GOKIEN_DBOPEN;
    } else if (ret != SQLITE_OK) {
        gokien_free(tmp);
        // FIXME: Unhandled error codes
        return ret;
    }

    *cj = tmp;

    return GOKIEN_OK;
}

int gokien_get_characters(Gokien          *cj,
                           char             *input_code,
                           Gokiencharlist **l) {
    Gokiencharlist *tmp = NULL;
    sqlite3_stmt *stmt;
    char *cj_query;
    char *query_code;
    char *star_ptr;
    char *query;
    int ret;

    if (input_code == NULL || strlen(input_code) == 0 || strlen(input_code) > 5) {
        return GOKIEN_INVALID;
    }

    if (input_code[0] == '*' || input_code[strlen(input_code) - 1] == '*') {
        return GOKIEN_INVALID;
    }

    // Start with the Gokien instance's cj_query
    cj_query = calloc(strlen(cj->cj_query) + MAX_LEN_CODE_QUERY +
                      strlen(ORDER_BY_CLAUSE) + 1, sizeof(char));
    if (cj_query == NULL) {
        return GOKIEN_NOMEM;
    }

    strcpy(cj_query, cj->cj_query);

    query_code = calloc(6, sizeof(char));
    if (query_code == NULL) {
        free(cj_query);
        return GOKIEN_NOMEM;
    }
    strncpy(query_code, input_code, 5);

    // Handle optional wildcards
    star_ptr = strchr(query_code, '*');
    if (star_ptr == NULL) {
        strcat(cj_query, "AND code = '%q' ");
    } else {
        strcat(cj_query, "AND code GLOB '%q' ");
    }

    // Attach the order by clause to the end
    strcat(cj_query, ORDER_BY_CLAUSE);

    query = sqlite3_mprintf(cj_query, cj->version, query_code);

    free(query_code);
    free(cj_query);

    if (query == NULL) {
        return GOKIEN_NOMEM;
    }

    ret = sqlite3_prepare_v2(cj->db, query, -1, &stmt, 0);
    if (ret != SQLITE_OK) {
        // FIXME: Unhandled error codes
        return ret;
    }

    sqlite3_free(query);

    while (1) {
        ret = sqlite3_step(stmt);

        if (ret == SQLITE_ROW) {
            char *chchar = (char *)sqlite3_column_text(stmt, 0);
            char *simpchar = (char *)sqlite3_column_text(stmt, 1);
            char *code = (char *)sqlite3_column_text(stmt, 2);
            uint32_t frequency = (uint32_t)sqlite3_column_int(stmt, 3);

            Gokienchar *c;
            int ret = gokien_char_new(&c, chchar, simpchar, code, frequency);
            if (ret != GOKIEN_OK) {
                return ret;
            }

            ret = gokien_char_list_prepend(&tmp, c);
            if (ret != GOKIEN_OK) {
                return ret;
            }
        } else if(ret == SQLITE_DONE) {
            // All rows finished
            sqlite3_finalize(stmt);
            break;
        } else {
            // Some error encountered
            return GOKIEN_DBERROR;
        }
    }

    if (tmp == NULL) {
        return GOKIEN_NOCHARS;
    }

    *l = tmp;

    return GOKIEN_OK;
}

int gokien_get_characters_by_shortcode(Gokien          *cj,
                                        char             *input_code,
                                        Gokiencharlist **l) {
    Gokiencharlist *tmp = NULL;

    sqlite3_stmt *stmt;
    int ret;
    char *query;

    if (input_code == NULL || strlen(input_code) != 1) {
        return GOKIEN_INVALID;
    }

    query = sqlite3_mprintf(cj->shortcode_query, 0, input_code);
    if (query == NULL) {
        return GOKIEN_NOMEM;
    }

    ret = sqlite3_prepare_v2(cj->db, query, -1, &stmt, 0);
    if (ret != SQLITE_OK) {
        // FIXME: Unhandled error codes
        return ret;
    }

    sqlite3_free(query);

    while (1) {
        ret = sqlite3_step(stmt);

        if (ret == SQLITE_ROW) {
            char *chchar = (char *)sqlite3_column_text(stmt, 0);
            char *simpchar = (char *)sqlite3_column_text(stmt, 1);
            uint32_t frequency = (uint32_t)sqlite3_column_int(stmt, 3);

            Gokienchar *c;
            int ret = gokien_char_new(&c, chchar, simpchar, input_code, frequency);
            if (ret != GOKIEN_OK) {
                return ret;
            }

            ret = gokien_char_list_prepend(&tmp, c);
            if (ret != GOKIEN_OK) {
                return ret;
            }
        } else if(ret == SQLITE_DONE) {
            // All rows finished
            sqlite3_finalize(stmt);
            break;
        } else {
            // Some error encountered
            return GOKIEN_DBERROR;
        }
    }

    if (tmp == NULL) {
        return GOKIEN_NOCHARS;
    }

    *l = tmp;

    return GOKIEN_OK;
}

int gokien_get_radical(Gokien     *cj,
                        const char   key,
                        char       **radical) {
    if ((key < 'a' || key > 'z') && (key != '*')) {
        return GOKIEN_INVALID;
    }

    if (key == '*') {
        // Special case for the wildcard '*'
        *radical = "＊";
    } else {
        // The actual Gokien radicals
        *radical = (char *)gokien_radicals[key - 'a'];
    }

    return GOKIEN_OK;
}

int gokien_is_input_key(Gokien    *cj,
                         const char  key) {
    if (key < 'a' || key > 'z') {
        return GOKIEN_INVALID;
    }

    return GOKIEN_OK;
}

int gokien_free(Gokien *cj) {
    sqlite3_close(cj->db);
    free(cj->cj_query);
    free(cj->shortcode_query);
    free(cj);

    return GOKIEN_OK;
}
