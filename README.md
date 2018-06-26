# libgokien - Library for Vietnamese Input Method

[![travis-shield]][travis-url]
[![license-shield]][license-url]

[travis-url]: https://travis-ci.org/Cangjians/libgokien
[travis-shield]: https://api.travis-ci.org/Cangjians/libgokien.svg?branch=master
[license-url]: http://www.gnu.org/licenses/lgpl.html
[license-shield]: https://img.shields.io/github/license/cangjians/libgokien.svg

This is a C library implementing the Gokien input method.

Below is a trivial example of how to use it:

```c
#include <stdio.h>
#include <gokien.h>

int main() {
    Gokien *cj;
    int ret = gokien_new(&cj, GOKIEN_VERSION_3,
                          GOKIEN_FILTER_BIG5 | GOKIEN_FILTER_HKSCS);
    GokienCharList *chars;
    GokienCharList *iter;
    ret = gokien_get_characters(cj, "d*d", &chars);

    if (ret == GOKIEN_NOCHARS) {
        printf("No chars with code '%s'\n", "d*d");
        gokien_free(cj);
        return 1;
    }

    iter = chars;

    while (1) {
        if (iter == NULL)
            break;

        printf("Char: %s, SimpChar: %s, code: '%s', classic frequency: %d\n",
               iter->c->chchar, iter->c->simpchar, iter->c->code, iter->c->frequency);

        iter = iter->next;
    }

    gokien_char_list_free(chars);
    gokien_free(cj);
    return 0;
}
```

For more details, refer to the documentation, either
[online](http://cangjians.github.io/projects/libgokien/documentation)
or the one shipped with this software.

Development happens [on github](https://github.com/Cangjians/libgokien), and
stable release tarballs can be found in
[the download section](http://cangjians.github.io/downloads/libgokien/) of
our website.

## History

This library is based on
[Wan Leung Wong's original libgokien](https://github.com/wanleung/libgokien).

In fact, when writing it, we tried to stay close to the original libgokien
API, because Wan Leung had done a pretty good job with it.

However, we felt the need to start afresh for a few reasons:

* due to real life constraints, Wan Leung didn't have any time to dedicate to
  libgokien any more

* we felt that some of the technical decisions in the original libgokien were
  not the wisest choice

Nevertheless, this library would probably not exist if Wan Leung hadn't opened
the way, so we feel it is important to give him credit.

Thank you very much Wan Leung!

## Legalities

libgokien is offered under the terms of the
[GNU Lesser General Public License, either version 3 or any later version][license-url].

We won't ask you to sign a copyright assignment or any other kind of silly and
tedious legal document, so just send us patches and/or pull requests!
