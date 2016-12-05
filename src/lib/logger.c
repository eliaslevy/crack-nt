/*
# This program was written by and is copyright Alec Muffett 1991,
# 1992, 1993, 1994, 1995, and 1996, and is provided as part of the
# Crack v5.0 Password Cracking package.
#
# The copyright holder disclaims all responsibility or liability with
# respect to its usage or its effect upon hardware or computer
# systems, and maintains copyright as set out in the "LICENCE"
# document which accompanies distributions of Crack v5.0 and upwards.
*/

#include "libcrack.h"

void
Logger(key, fmt, a, b, c, d, e, f)
char key;
char *fmt;
char *a, *b, *c, *d, *e, *f;
{
    time_t t;

    time(&t);

    fprintf(stdout, "%c:%ld:", key, (long)t);
    fprintf(stdout, fmt, a, b, c, d, e, f);

    fflush(stdout);
    fflush(stderr);             /* and why not ? */
}

