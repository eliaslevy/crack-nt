/*
 * raw_lanman.c by Aleph One <aleph1@underground.org>
 *
 * Based on crypt_nt.c by Bob Tinsley <R.Tinsley@rhbnc.ac.uk>
 * and des_enc.c & ecb_enc.c by Eric Young <eay@mincom.oz.au>
 *
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <des.h>
#include "global.h"
#include "md4.h"
#include "des_locl.h"
#include "crypt_nt.h"

#define PASSLEN_MAX (14)

#define DES_LONG unsigned long

/* Magic number - null ciphertext */
#define NULL_CT         868578240321282570LL

/*
define ll2ll(l1,l2,ll) (ll =((u_int64_t)(l1)) , \
                         ll =((u_int64_t)(((ll)<<32L)|l2)) )
*/

void des_raw_encrypt(DES_LONG *, des_key_schedule);


static void str_to_key(unsigned char *str, unsigned char *key)
{
  void des_set_odd_parity(des_cblock *);
  int i;

  key[0] = str[0]>>1;
  key[1] = ((str[0]&0x01)<<6) | (str[1]>>2);
  key[2] = ((str[1]&0x03)<<5) | (str[2]>>3);
  key[3] = ((str[2]&0x07)<<4) | (str[3]>>4);
  key[4] = ((str[3]&0x0F)<<3) | (str[4]>>5);
  key[5] = ((str[4]&0x1F)<<2) | (str[5]>>6);
  key[6] = ((str[5]&0x3F)<<1) | (str[6]>>7);
  key[7] = str[6]&0x7F;
  for (i=0;i<8;i++) {
    key[i] = (key[i]<<1);
  }
  des_set_odd_parity((des_cblock *)key);
}

void raw_lanman(char *key, u_int64_t *ll2) {
  unsigned char x1[] = {0x4B,0x47,0x53,0x21,0x40,0x23,0x24,0x25};
  unsigned char key2[PASSLEN_MAX + 1];
  des_cblock deskey;
  des_key_schedule ks;
  DES_LONG ll[2];
  register DES_LONG l;
  register unsigned char *in;
  int i;

  memset(key2, 0, PASSLEN_MAX + 1);

  /* Copy key into a zero-padded buffer, doing case conversion    */

#if 1   /* Convert to upper case */
  for (i = 0; i < PASSLEN_MAX; i++) {
    if (*(key+i) == '\0')
      break;
    *(key2+i) = toupper(*(key+i));
  }
#else   /* Don't do case conversion */
  strncpy((char *) key2, key, PASSLEN_MAX);
   i = strlen(key2);
#endif

  str_to_key(key2, (unsigned char *)deskey);
  des_set_key((des_cblock *)deskey, ks);

  in=(unsigned char *)x1;
  c2l(in,l); ll[0]=l;
  c2l(in,l); ll[1]=l;
  des_raw_encrypt(ll,ks);
  ll2ll(ll[0],ll[1],ll2[0]);
  l=ll[0]=ll[1]=0;

  if (i > 7) {
    str_to_key(key2+7, (unsigned char *)deskey);
    des_set_key((des_cblock *)deskey, ks);
    in=(unsigned char *)x1;
    c2l(in,l); ll[0]=l;
    c2l(in,l); ll[1]=l;
    des_raw_encrypt(ll,ks);
    ll2ll(ll[0],ll[1],ll2[1]);
    l=ll[0]=ll[1]=0;
  } else
    ll2[1] = NULL_CT;
}

void des_raw_encrypt(data, ks)
DES_LONG *data;
des_key_schedule ks;
        {
        register DES_LONG l,r,t,u;
        register DES_LONG *s;
#ifdef DES_PTR
        register unsigned char *des_SP=(unsigned char *)des_SPtrans;
#endif

        r=data[0];
        l=data[1];

        IP(r,l);
        /* Things have been modified so that the initial rotate is
         * done outside the loop.  This required the
         * des_SPtrans values in sp.h to be rotated 1 bit to the right.
         * One perl script later and things have a 5% speed up on a sparc2.
         * Thanks to Richard Outerbridge <71755.204@CompuServe.COM>
         * for pointing this out. */
        /* clear the top bits on machines with 8byte longs */
        /* shift left by 2 */
        r=ROTATE(r,29)&0xffffffffL;
        l=ROTATE(l,29)&0xffffffffL;
        s=(DES_LONG *)ks;
	D_ENCRYPT(l,r, 0); /*  1 */
	D_ENCRYPT(r,l, 2); /*  2 */
	D_ENCRYPT(l,r, 4); /*  3 */
	D_ENCRYPT(r,l, 6); /*  4 */
	D_ENCRYPT(l,r, 8); /*  5 */
	D_ENCRYPT(r,l,10); /*  6 */
	D_ENCRYPT(l,r,12); /*  7 */
	D_ENCRYPT(r,l,14); /*  8 */
	D_ENCRYPT(l,r,16); /*  9 */
	D_ENCRYPT(r,l,18); /*  10 */
	D_ENCRYPT(l,r,20); /*  11 */
	D_ENCRYPT(r,l,22); /*  12 */
	D_ENCRYPT(l,r,24); /*  13 */
	D_ENCRYPT(r,l,26); /*  14 */
	D_ENCRYPT(l,r,28); /*  15 */
	D_ENCRYPT(r,l,30); /*  16 */

	data[0]=l;
	data[1]=r;
	l=r=t=u=0;
	}


