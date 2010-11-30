#include "sfsauthd.h"
#include "auth_helper_prot.h"

extern void badauth (logincb_t cb, sfs_authtype atype, str msg, const sfsauth_dbrec *dbrp);
extern void badauth (logincb_t cb, sfs_authtype atype, str msg, str user);
extern void badauth (logincb_t cb, sfs_authtype atype, str msg, ref<sfspub> pub);

authpending::authpending (authclnt *a, const sfsauth2_loginarg *largp)
    : ac (a), authid (largp->authid), seqno (largp->arg.seqno), tmo (NULL)
{
  if (!bytes2xdr (atype, largp->arg.certificate))
    panic ("authpending::authpending: could not decode type\n");
  ac->aptab.insert (this);
  refresh ();
}

authpending::~authpending ()
{
  timecb_remove (tmo);
  ac->aptab.remove (this);
}

void
authpending_srp::init (const sfs_autharg2 *aap, logincb_t cb)
{
  sfsauth_dbkey kname (SFSAUTH_DBKEY_NAME);
  *kname.name = aap->srpauth->req.user;
  if (!get_user_cursor (&srp_dbp, &srp_ac, NULL, kname)) {
    /* Note we are divulging what accounts exist and don't.  To avoid
     * doing so, we'd have to synthesize new but constant SRP
     * parameters and run part way through the protocol.
     */
    badauth (cb, atype, "bad user", *kname.name);
    delete this;
    return;
  }

  sfsauth2_loginres res (SFSLOGIN_MORE);
  switch (srp.init (res.resmore.addr (), &aap->srpauth->msg,
		    authid, srp_ac->ae.userinfo->name,
		    srp_ac->ae.userinfo->pwauth,
		    /* XXX - The following is for backwards compatibility,
		     * but could allow "two-for-one" guessing by active
		     * adversaries.  Should be phased out eventually. */
		    ac->client_release () < 8 ? 3 : 6)) {
  case SRP_NEXT:
    (*cb) (&res, SFS_SRPAUTH, NULL);
    break;
  default:
    badauth (cb, atype, "no password established", srp.user);
    delete this;
    break;
  }
}

void
authpending_srp::next (const sfs_autharg2 *aap, logincb_t cb)
{
  sfsauth2_loginres res;

  srpmsg msg;
  switch (srp.next (&msg, &aap->srpauth->msg)) {
  case SRP_NEXT:
    res.set_status (SFSLOGIN_MORE);
    swap (*res.resmore, msg);
    (*cb) (&res, atype, NULL);
    break;
  case SRP_LAST:
    if (ac->setuser (&res, srp_ac->ae, srp_dbp))
      swap (res.resok->resmore, msg);
    (*cb) (&res, atype, &srp_ac->ae); 
    delete this;
    return;
  default:
    badauth (cb, atype, "incorrect password", &srp_ac->ae);
    delete this;
    return;
  }
}

authpending_helper::~authpending_helper ()
{
  srv = NULL;
  if (getpassreq)
    getpassreq->reject (SYSTEM_ERR);
  if (pid > 0) {
    chldcb (pid, NULL);
    kill (pid, SIGKILL);
  }
  if (cb)
    badauth (cb, atype, "canceled or timed out", user);
}

void
authpending_helper::init (const sfs_autharg2 *aap, logincb_t c)
{
  vec<str> av;
  av.push_back (auth_helper);
  av.push_back ("-r");
  av.push_back (PACKAGE);
  if (aap->pwauth->req.user.len ()) {
    /* Problem is, a user might accidentally type his/her password
     * instead of a username.  So perhaps it's best not to have
     * usernames showing up in ps.  */
    user = aap->pwauth->req.user;
    pwds.push_back (user);
    if (aap->pwauth->password.len ())
      pwds.push_back (aap->pwauth->password);
  }

  ptr<axprt_unix> x (axprt_unix_aspawnv (av[0], av));
  if (!x) {
    badauth (c, atype, "could not spawn auth helper program", user);
    delete this;
    return;
  }
  pid = axprt_unix_spawn_pid;
  chldcb (pid, wrap (this, &authpending_helper::reap));
  x->allow_recvfd = false;

  cb = c;
  srv = asrv::alloc (x, authhelp_prog_1,
		     wrap (this, &authpending_helper::dispatch));
}

void
authpending_helper::next (const sfs_autharg2 *aap, logincb_t c)
{
  str2wstr (aap->pwauth->password);
  if (cb || !pwds.empty () || !getpassreq) {
    badauth (c, atype, "multiple concurrent login RPCs for same seqno", user);
    delete this;
    return;
  }
  cb = c;
  if (!user)
    user = aap->pwauth->password;
  authhelp_getpass_res res;
  res.response = aap->pwauth->password;
  svccb *sbp = getpassreq;
  getpassreq = NULL;
  sbp->reply (&res);
}

void
authpending_helper::reap (int)
{
  pid = -1;
}

void
authpending_helper::dispatch (svccb *sbp)
{
  if (!sbp) {
    badauth (cb, atype, NULL, user);
    cb = NULL;
    delete this;
    return;
  }

  switch (sbp->proc ()) {
  case AUTHHELPPROG_NULL:
    sbp->reply (NULL);
    break;
  case AUTHHELPPROG_GETPASS:
    if (getpassreq) {
      warn ("received concurrent RPCs from auth helper\n");
      delete this;
    }
    else if (!pwds.empty ()) {
      authhelp_getpass_res res;
      res.response = pwds.pop_front ();
      sbp->reply (&res);
    }
    else if (cb) {
      getpassreq = sbp;
      authhelp_getpass_arg *argp
	= sbp->Xtmpl getarg<authhelp_getpass_arg> ();
      if (!argp->echo && !strncasecmp (argp->prompt, "password:", 9))
	argp->prompt = strbuf () << "Unix " << argp->prompt;
      sfsauth2_loginres res (SFSLOGIN_MORE);
      xdr2bytes (*res.resmore, *argp);
      logincb_t c (cb);
      cb = NULL;
      (*c) (&res, atype, NULL);
    }
    else
      panic ("authpending_helper no cb and no getpassreq\n");
    break;
  case AUTHHELPPROG_SUCCEED:
    {
      str unixuser (sbp->Xtmpl getarg<authhelp_succeed_arg> ()->user);
      authclnt *ac_local (ac);
      ptr<authcursor> ah_ac_local (ah_ac);
      logincb_t::ptr c (cb);
      cb = NULL;
      delete this;
      sbp->reply (NULL);
      if (c)
	ac_local->login_unixpw_2 (ah_ac_local, unixuser, NULL, false, c);
      break;
    }
  default:
    sbp->reject (PROC_UNAVAIL);
    break;
  }
}

void
authpending_unix::init (const sfs_autharg2 *aap, logincb_t cb)
{
  authhelp_getpass_arg arg;
  arg.echo = false;
  arg.prompt = "UNIX password: ";

  sfsauth2_loginres res (SFSLOGIN_MORE);
  xdr2bytes (*res.resmore, arg);

  (*cb) (&res, atype, NULL);
}

void
authpending_unix::next (const sfs_autharg2 *aap, logincb_t cb)
{
  str2wstr (aap->pwauth->password);

  authclnt *ac_local (ac);
  ptr<authcursor> ah_ac_local (ah_ac);
  delete this;

  if (cb)
    ac_local->login_unixpw_2 (ah_ac_local, unixuser, aap->pwauth->password,
                              false, cb);
}
