/* $Id: test_dsa.C,v 1.1 2006/02/20 04:43:40 kaminsky Exp $ */

#define USE_PCTR 0

#include "crypt.h"
#include "dsa.h"
#include "bench.h"

u_int64_t tst_vtime;
u_int64_t tst_stime;

void
test_key_sign (dsa_priv *sk)
{
  u_int64_t tmp1, tmp2, tmp3;

  for (int i = 0; i < 50; i++) {
    bigint r, s;

    size_t len = 512;
    wmstr wmsg (len);
    rnd.getbytes (wmsg, len);
    str msg = wmsg;

    tmp1 = get_time ();
    sk->sign (&r, &s, msg);
    tmp2 = get_time ();

    if (!sk->verify (msg, r, s))
      panic << "Verify failed\n"
	    << "  p = " << sk->p << "\n"
	    << "  q = " << sk->q << "\n"
	    << "  g = " << sk->g << "\n"
	    << "  x = " << sk->x << "\n"
	    << "  y = " << sk->y << "\n"
	    << "msg = " << hexdump (msg.cstr (), msg.len ()) << "\n"
	    << "sig.r = " << r << "\n"
	    << "sig.s = " << s << "\n";
    tmp3 = get_time ();

    tst_vtime += (tmp3 - tmp2);
    tst_stime += (tmp2 - tmp1);

    int bitno = rnd.getword () % mpz_sizeinbase2 (&r);
    r.setbit (bitno, !r.getbit (bitno));
    if (sk->verify (msg, r, s))
      panic << "Verify should have failed\n";

    bitno = rnd.getword () % mpz_sizeinbase2 (&s);
    s.setbit (bitno, !s.getbit (bitno));
    if (sk->verify (msg, s, s))
      panic << "Verify should have failed\n";
  }
}

int
main (int argc, char **argv)
{
  tst_vtime = tst_stime = 0;
  bool opt_v = false;
  int vsz = 1024;
  ptr<dsa_gen> dg;

  if (argc > 1 && !strcmp (argv[1], "-v")) {
    opt_v = true;
  }

  setprogname (argv[0]);
  random_update ();
  for (int i = 0; i < 1; i++) {
    dg = dsa_gen::rgen (opt_v ? vsz : 424 + rnd.getword () % 256);
    test_key_sign (dg->sk);
  }
  if (opt_v) {
    warn ("Signed 50 messages with %d bit key in %" U64F "u " 
	  TIME_LABEL " per signature\n", vsz, tst_stime / 50);
    warn ("Verified 50 messages with %d bit key in %" U64F "u " 
	  TIME_LABEL " per verify\n", vsz, tst_vtime / 50);
  }
  return 0;
}
