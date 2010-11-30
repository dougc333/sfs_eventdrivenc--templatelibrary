/* $Id: test_elgamal.C,v 1.2 2006/05/30 00:00:20 dm Exp $ */

/*
 *
 * Copyright (C) 2006 Michael J. Freedman (mfreedman at alum.mit.edu)
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2, or (at
 * your option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
 * USA
 *
 */

#define USE_PCTR 0

#include "crypt_prot.h"
#include "crypt.h"
#include "elgamal.h"
#include "bench.h"

u_int64_t etime;
u_int64_t dtime;
u_int64_t htime;

static const size_t repeat = 1;
static const size_t cnt    = 20;

void
do_elgamal (elgamal_priv &sk, crypt_ctext &ctext, str &ptext, bool recover)
{
  u_int64_t tmp1, tmp2, tmp3;

#if 0
  u_int len = rnd.getword () % (mpz_sizeinbase2 (&sk.p)/8 - 1);
  len = max (len, (u_int) 20);
#else
  u_int len = 10;
#endif

  wmstr wmsg (len);
  rnd.getbytes (wmsg, len);
  ptext = wmsg;

  tmp1 = get_time ();
  
  if (!sk.encrypt (&ctext, ptext, recover)) {
    strbuf sb;
    sb << "Encryption failed\n"
       << "  p  = " << sk.p << "\n"
       << "  g  = " << sk.g << "\n"
       << "msg1 = " << hexdump (ptext.cstr (), ptext.len ()) << "\n";
    panic << sb;
  }

  tmp2 = get_time ();
  str ptext2 = sk.decrypt (ctext, len, recover);
  tmp3 = get_time ();
  
  if (!ptext2 || (recover && ptext != ptext2)) {
    elgamal_ctext &ec = *ctext.elgamal;
    str ctextr = sk.post_decrypt (ec.r, sk.mod_size ()/8);
    str ctextm = sk.post_decrypt (ec.m, sk.mod_size ()/8);

    strbuf sb;
    sb << "Decryption failed\n"
       << "  p  = "   << sk.p << "\n"
       << "  g  = "   << sk.g << "\n"
       << "  cr = "   << hexdump (ctextr.cstr (), ctextr.len ()) << "\n"
       << "  cm = "   << hexdump (ctextm.cstr (), ctextm.len ()) << "\n"
       << "msg1 = "   << hexdump (ptext.cstr (),  ptext.len ())  << "\n";
    if (ptext2)
      sb << "msg2 = " << hexdump (ptext2.cstr (), ptext2.len ()) << "\n";
    panic << sb;
  }

  etime += (tmp2 - tmp1);  
  dtime += (tmp3 - tmp2);
}


void
test_elgamal (elgamal_priv &sk)
{
  u_int64_t tmp1, tmp2;
  for (size_t i = 0; i < cnt; i++) {

    crypt_ctext ctext1 (sk.ctext_type ());
    crypt_ctext ctext2 (sk.ctext_type ());
    crypt_ctext ctext3 (sk.ctext_type ());
    str ptext1, ptext2, ptext3;

    do_elgamal (sk, ctext1, ptext1, true);
    do_elgamal (sk, ctext2, ptext2, false);
    do_elgamal (sk, ctext3, ptext3, false);

    bigint ptextc = sk.pre_encrypt (ptext2) + sk.pre_encrypt (ptext3);
    ptextc = powm (sk.g, ptextc, sk.p);

    // Test homomorphic encryption

    tmp1 = get_time ();
    crypt_ctext ctextc (sk.ctext_type ());
    sk.add (&ctextc, ctext2, ctext3);
    tmp2 = get_time ();

    u_int len = max (ptext2.len (), ptext3.len ()) + 1;
    str cres  = sk.decrypt (ctextc, len, false);
    str pres  = sk.post_decrypt (ptextc, len);

    if (cres != pres)
      panic << "Homomorphic multiplication failed"
	    << "\n        msg1 = " << hexdump (ptext2.cstr (), ptext2.len ())
	    << "\n        msg2 = " << hexdump (ptext3.cstr (), ptext3.len ())
	    << "\n cipher comb = " << hexdump (cres.cstr (), cres.len ())
	    << "\n  plain comb = " << hexdump (pres.cstr (), pres.len ()) 
	    << "\n";

    htime += (tmp2 - tmp1);
  }
}

void
do_test (int vsz, int asz, bool opt_v)
{
  etime = dtime = htime = 0;

  if (!opt_v) {
    vsz = 424 + rnd.getword () % 256;
    asz = 160 + rnd.getword () % 256;
  }

  for (size_t i = 0; i < repeat; i++) {
    elgamal_priv sk = elgamal_keygen (vsz, asz);
    test_elgamal (sk);
  }
  
  if (opt_v) {
    size_t tot  = repeat * cnt;
    size_t ttot = tot * 3;
    warn ("Elgamal cryptosystem with %d bit key [%d]\n", vsz, asz);
    warn ("   Encrypted  %u messages in %" U64F "u " 
	  TIME_LABEL " per message\n", (u_int) ttot, (etime / ttot));
    warn ("   Decrypted  %u messages in %" U64F "u " 
	  TIME_LABEL " per message\n", (u_int) ttot, (dtime / ttot));
    warn ("   Homo-added %u messages in %" U64F "u " 
	  TIME_LABEL " per message\n", (u_int) tot, (htime / tot));
  }
}


int
main (int argc, char **argv)
{
  bool opt_v  = false;
  int  vsz    = 1024;
  int  asz    = 160;

  for (int i=1; i < argc; i++) {
    if (!strcmp (argv[i], "-v"))
      opt_v = true;
    else if (!strcmp (argv[i], "-b")) {
      assert (argc > i+1);
      vsz = atoi (argv[i+1]);
      assert (vsz > 0);
    }
    else if (!strcmp (argv[i], "-a")) {
      assert (argc > i+1);
      asz = atoi (argv[i+1]);
      assert (asz > 0);
    }
  }

  setprogname (argv[0]);
  random_update ();

  do_test (vsz, asz, opt_v);
  
  return 0;
}
