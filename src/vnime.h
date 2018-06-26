#ifndef GOKIEN_H
#define GOKIEN_H

#include "gokien/common.h"
#include "gokien/char_list.h"

GOKIEN_BEGIN_DECL

enum GokienInputMethod {
    GK_IM_TELEX = 1,
    GK_IM_VNI,
    GK_IM_SIMPLE_TELEX,
    GK_IM_SIMPLE_TELEX2,
    GK_IM_USER_DEFINED,
    GK_IM_COUNT,
};

/*
 * Those value are taken from vnconv.h of Unikey Engine.
 */
enum GokienOutputCharset {
    GK_CHARSET_UTF8 = 1,
    //GK_CHARSET_UNICODE,
    //GK_CHARSET_WINCP1258,

    //GK_CHARSET_VIQR,
    //GK_CHARSET_UTF8VIQR,
    //GK_CHARSET_XUTF8,

    //GK_CHARSET_TCVN3,
    //GK_CHARSET_VPS,
    //GK_CHARSET_VISCII,
    //GK_CHARSET_BKHCM1,
    //GK_CHARSET_VIETWAREF,
    //GK_CHARSET_ISC,

    //GK_CHARSET_VNIWIN,
    //GK_CHARSET_BKHCM2,
    //GK_CHARSET_VIETWAREX,
    //GK_CHARSET_VNIMAC,
};

enum GokienOption {
    GK_OPTION_FREE_MARKING    = 1 << 0,
    //GK_OPTION_MODERN_STYLE    = 1 << 1,
    //GK_OPTION_MACRO_ENABLE    = 1 << 2,
    //GK_OPTION_USE_CLIPBOARD   = 1 << 3,
    //GK_OPTION_ALWAYS_MACRO    = 1 << 4,
    //GK_OPTION_STRICT_SPELLING = 1 << 5,
    //GK_OPTION_SPELL_CHECK     = 1 << 6,
    GK_OPTION_RESTORE_NON_VN  = 1 << 7,
    //GK_OPTION_USE_IME         = 1<< 8, // Use input method (Win32 only)
};

struct Gokien {
    uint32_t option_flags; // GokienOption
    uint32_t input_method; // GokienInputMethod
    uint32_t charset; // GokienOutputCharset
};

/*
 * Inits a allocated Gokien object.
 * Should call gokien_destroy() when unused.
 */
GOKIEN_EXTERN int
gokien_init(struct Gokien *self, uint32_t option_flags);

/*
 * Free all member in object but not the object itself.
 */
GOKIEN_EXTERN int
gokien_destroy(struct Gokien *self);

/*
 * Allocate a new Gokien object and initialize it.
 * When it is possible, use gokien_init() instead.
 *
 * Should call gokien_free() when unused.
 */
GOKIEN_EXTERN struct Gokien *
gokien_new(uint32_t option_flags);

/*
 * Free all member in Gokien object and itself.
 * It should call gokien_destroy() internally.
 */
GOKIEN_EXTERN int
gokien_free(Gokien *self);

GOKIEN_EXTERN int
gokien_get_characters(Gokien *self, char const *code, struct GokienCharList **l);

GOKIEN_EXTERN int
gokien_is_input_key(Gokien *self, const char key);

GOKIEN_END_DECL

#endif /* GOKIEN_H */
