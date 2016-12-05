#include <stdio.h>
#include <unistd.h>
#include "crypt_nt.h"

static const char *usage = "usage: hash_nt [-ln] password...\n";

int main(int argc, char *argv[])
{
    int i, argnn, argn = 1, lanman = 0;
    char *p;

    /* The flag-parsing code is over-kill, but what the hell... */

    while (argn < argc && argv[argn][0] == '-' && argv[argn][1] != '\0') {
	/* '--' on its own ends flags */
	if (argv[argn][1] == '-' && argv[argn][2] == '\0') {
	    argn++;
	    break;
	}
	argnn = 1;
	while (argv[argn][argnn] != '\0') {
	    switch (argv[argn][argnn]) {
	    case 'l':
		lanman = 1;
		break;
	    case 'n':
		lanman = 0;
		break;
            case '-':           /* silently ignored */
                break;
            default:
                fprintf(stderr, "%s", usage);
                return 1;
	    }
	    argnn++;
	}
	argn++;
    }

    if (argc - argn < 1) {
	fprintf(stderr, "%s", usage);
	return 1;
    }
    for (; argn < argc; argn++) {
#if 0
	printf("%14s : ", argv[argn]);
#endif
	if (lanman)
	    p = crypt_lanman(argv[argn], NULL);
	else
	    p = crypt_nt(argv[argn], NULL);
	for (i = 0; i < 32; i++)
	    putchar(*(p + i));
	putchar('\n');
    }

    return 0;
}
