/*
 * speed_nt.c -- (c) 1997, Bob Tinsley <R.Tinsley@rhbnc.ac.uk>
 *
 * Whooo, my first program using signals.
 *
 */

#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include "crypt_nt.h"

#define TIME_HASH (10)

static char *key = "aB3dE6gH9!";
static int running;

void time_up()
{
  running = 0;
}

int main(int argc, char **argv)
{
    struct timeval start, finish;
    float diff;
    int count;
    u_int64_t ll[2];

    if (argc > 1)
      key = argv[1];

    printf("Please wait %d seconds...\n", 2*TIME_HASH);
    fflush(stdout);

    running=1;
    gettimeofday(&start, NULL);
    signal(SIGALRM, time_up);
    alarm(TIME_HASH);
    for (count = 0; running; count++)
	(void) crypt_nt(key, NULL);
    gettimeofday(&finish, NULL);

    diff = (float) (finish.tv_sec - start.tv_sec);
    diff += ((float) (finish.tv_usec - start.tv_usec)) / 1000000;
    printf("MD4-hash a %d-character string %8d times in %5.2fs\t= %6d/s\n",
	   strlen(key), count, diff, (int) (count / diff));

    running=1;
    gettimeofday(&start, NULL);
    signal(SIGALRM, time_up);
    alarm(TIME_HASH);
    for (count = 0; running; count++)
	(void) crypt_lanman(key, NULL);
    gettimeofday(&finish, NULL);

    diff = (float) (finish.tv_sec - start.tv_sec);
    diff += ((float) (finish.tv_usec - start.tv_usec)) / 1000000;
    printf("DES-hash a %d-character string %8d times in %5.2fs\t= %6d/s\n",
	   strlen(key), count, diff, (int) (count / diff));

    running=1;
    gettimeofday(&start, NULL);
    signal(SIGALRM, time_up);
    alarm(TIME_HASH);
    for (count = 0; running; count++)
        (void) raw_lanman(key, ll);
    gettimeofday(&finish, NULL);

    diff = (float) (finish.tv_sec - start.tv_sec);
    diff += ((float) (finish.tv_usec - start.tv_usec)) / 1000000;
    printf("RAW-LANMAN-hash a %d-character string %8d times in %5.2fs\t= %6d/s\n",
           strlen(key), count, diff, (int) (count / diff));

    exit(0);
}
