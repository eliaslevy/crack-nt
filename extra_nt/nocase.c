/*
 * nocase.c (c) 1997, Bob Tinsley <R.Tinsley@rhbnc.ac.uk>
 *
 * Convert a Crack 5.0 ruleset to uppercase.
 * This program is designed to be run in a pipe-line...
 *      rules_u < rules_file | sort | uniq > rules_file.uc
 * Later versions will probably do the uniq'ing internally,
 * which will avoid having to reorder the rules...
 *
 */

#ifndef MAX_RULE
#define MAX_RULE 1024
#endif

#include <stdio.h>

int main(int argc, char **argv)
{
    FILE *rules = NULL;
    char rule_in[MAX_RULE];
    char rule_out[MAX_RULE];
    int i, err, offset, r = 0;
    char ch;

    if (argc != 1) {
	fprintf(stderr, "usage : nocase\n");
	exit(1);
    }
    while (gets(rule_in) != NULL) {
	if (*rule_in != '#') {
	    err = 0;
	    offset = 0;
	    for (i = 0; *(rule_in + i) != '\0' && *(rule_in + i) != '\n';) {
		if (err)
		    *(rule_out + offset++) = *(rule_in + i++);
		else
		    switch (*(rule_in + i)) {
		    case '=':	/* binary / trinary ops */
		    case '%':
		    case 's':
			*(rule_out + offset++) = *(rule_in + i++);
		    case '/':	/* unary / binary ops */
		    case '!':
		    case '(':
		    case ')':
		    case '@':
			if (*(rule_in + i + 1) == '?')
			    *(rule_out + offset++) = *(rule_in + i++);
			*(rule_out + offset++) = *(rule_in + i++);
			*(rule_out + offset++) = *(rule_in + i++);
			break;

		    case 'x':	/* binary ops */
		    case 'o':
		    case 'i':
			*(rule_out + offset++) = *(rule_in + i++);
		    case '^':	/* unary ops */
		    case '$':
		    case '<':
		    case '>':
		    case '\'':
			*(rule_out + offset++) = *(rule_in + i++);
		    case ' ':	/* nonary ops */
		    case ':':
		    case '*':
		    case '[':
		    case ']':
		    case 'r':
		    case 'd':
		    case 'f':
		    case 'p':
			*(rule_out + offset++) = *(rule_in + i++);
			break;

		    case 'u':
		    case 'l':
		    case 'c':
		    case 'C':
		    case 't':
			i++;
			break;

		    default:
			fprintf(stderr, "%s : unable to parse '%c' at position %d in " "%s" "\n",
				argv[0], *(rule_in + i), i, rule_in);
			*(rule_out + offset++) = *(rule_in + i++);
			err = 1;
			r = 1;
		    }
	    }
	    *(rule_out + offset) = '\0';

	    for (i = 0; i < offset; i++)
		putchar(*(rule_out + i));
	    putchar('\n');
	}
    }

    exit(r);
}
