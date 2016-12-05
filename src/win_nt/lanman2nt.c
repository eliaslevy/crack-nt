#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "crypt_nt.h"

int permute(char *passwd, char *hash, int index) {
  char *h;

  if(!passwd[index]) {
    h = crypt_nt(passwd, NULL);
    if (!strncmp(h, hash, 16)) {
      printf("%s\n", passwd);
      return 1;
    } else {
      return 0;
    }
  }

  passwd[index] = tolower(passwd[index]);
  if (permute(passwd, hash, index + 1))
    return 1;

  passwd[index] = toupper(passwd[index]);
  return (permute(passwd, hash, index + 1));
}

int main(int argc, char **argv) {
  char *lanman, *nt_hash;


  if ((argc != 3) || (strlen(argv[2]) != 32)) {
    fprintf(stderr, "usage: lanman2nt <lanman password> <nt hash>\n");
    exit(1);
  }

  lanman = argv[1];
  nt_hash = argv[2];

  return(permute(lanman, nt_hash, 0));
}
