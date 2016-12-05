/*
 * crypt_nt.c by Bob Tinsley <R.Tinsley@rhbnc.ac.uk>
 *
 * Based on ntcrack.c (v2.0) by Jonathan Wilkins <jwilkins@secnet.com>
 *                          and Jesse Burns      <burnsj@cuug.ab.ca>
 *                          with thanks to Oliver Freidrichs, Alfred Huger
 *                          and Aaron Spangler.
 *      and smbencrypt.c    (C) Andrew Tridgell 1992-1995
 *                          Modified by Jeremy Allison 1995.
 *                          [ released under GPL version 2 ]
 *                          Trimmed by Jonathan Wilkins (and later by Bob)
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <des.h>
#include "global.h"
#include "md4.h"

#define PASSLEN_MAX (14)

static void str_to_key(unsigned char *str, unsigned char *key);
static void E1(unsigned char *k, unsigned char *d, unsigned char *out);

static char digest_hex[32];

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

static void E1(unsigned char *k, unsigned char *d, unsigned char *out)
{
  des_key_schedule ks;
  des_cblock deskey;

  str_to_key(k,(unsigned char *)deskey);
#ifdef __FreeBSD__
  des_set_key(&deskey,ks);
#else /* __FreeBsd__ */
  des_set_key((des_cblock *)deskey,ks);
#endif /* __FreeBsd__ */
  des_ecb_encrypt((des_cblock *)d,(des_cblock *)out, ks, DES_ENCRYPT);
}

static void E_P16fast(unsigned char *p14, int len, unsigned char *p16)
{
  unsigned char x1[] = {0x4B,0x47,0x53,0x21,0x40,0x23,0x24,0x25};
  unsigned char null[] = {0xAA,0xD3,0xB4,0x35,0xB5,0x14,0x04,0xEE};

  E1(p14,   x1, p16);

  if (len > 7)
    E1(p14+7, x1, p16+8);
  else
    memcpy(p16+8, null, 8);
}

/* The NT LanMan 2.1 hashing algorithm, with a UNIX crypt(3) interface */
char* crypt_lanman(const char* key, const char* salt)
/* Note that the salt is not used -- NT doesn't have them		*/
/* salt must be NULL -- it might be used to pass a version in the future*/
{
  int i;
  unsigned char digest[16];
  unsigned char key2[PASSLEN_MAX + 1];

  memset(key2, 0, PASSLEN_MAX + 1);

	/* Copy key into a zero-padded buffer, doing case conversion	*/

#if 1	/* Convert to upper case */
  for (i = 0; i < PASSLEN_MAX; i++) {
    if (*(key+i) == '\0')
      break;
    *(key2+i) = toupper(*(key+i));
  }
#else	/* Don't do case conversion */
  strncpy((char *) key2, key, PASSLEN_MAX);
#endif

	/* Hash it! */

  E_P16fast(key2, i, digest);

	/* crypt(3) returns printable output, pwdump uses this format	*/

  sprintf(digest_hex,
          "%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X\n", 
          digest[ 0], digest[ 1], digest[ 2], digest[ 3], 
          digest[ 4], digest[ 5], digest[ 6], digest[ 7], 
          digest[ 8], digest[ 9], digest[10], digest[11], 
          digest[12], digest[13], digest[14], digest[15]);
  return(digest_hex);
}

/* The NT password hashing algorithm, with a UNIX crypt(3) interface */
char* crypt_nt(const char* key, const char* salt)
/* Note that the salt is not used -- NT doesn't have them		*/
/* salt must be NULL -- it might be used to pass a version in the future*/
{
  MD4_CTX context;
  unsigned char digest[16];
  unsigned char key2[2*PASSLEN_MAX + 1];
  unsigned int len2 = 2*strlen(key);
  int i;

  memset(key2, 0, 2*PASSLEN_MAX + 1);

	/* Check for internal buffer overflow and do something sensible	*/

  if (len2 > 2*PASSLEN_MAX)
    len2 = 2*PASSLEN_MAX;

	/* Copy key into a zero-padded buffer, converting to Unicode	*/

  for (i = 0; i < len2; i += 2) {
    key2[i+0] = *(key+(i/2));		/* We assume ISO 8859-1		*/
    key2[i+1] = 0;
  }

	/* Hash it! */

  MD4Init(&context);			/* The MD4 code is untouched.	*/
  MD4Update(&context, key2, len2);	/* If you want it optimised,	*/
  MD4Final(digest, &context);		/* then do it yourself...	*/

	/* crypt(3) returns printable output, pwdump uses this format	*/

  sprintf(digest_hex,
          "%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X%02X\n", 
          digest[ 0], digest[ 1], digest[ 2], digest[ 3], 
          digest[ 4], digest[ 5], digest[ 6], digest[ 7], 
          digest[ 8], digest[ 9], digest[10], digest[11], 
          digest[12], digest[13], digest[14], digest[15]);
  return(digest_hex);
}
