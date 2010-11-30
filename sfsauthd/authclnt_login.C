#include "sfsauthd.h"
#include "authgrps.h"
#include "auth_helper_prot.h"

#if HAVE_GETSPNAM
#include <shadow.h>
#endif /* HAVE_GETSPNAM */

extern "C" char *crypt (const char *, const char *);

str
unixpriv (const char *p)
{
  static const str unixeq ("unix=");
  static const str cunixeq (strbuf () << "," << unixeq);
  if (strncasecmp (p, unixeq, unixeq.len ())) {
    p = strstr (p, cunixeq);
    if (!p)
      return NULL;
    p++;
  }
  p += unixeq.len ();

  str val;
  if (char *e = strchr (p, ','))
    val = str (p, e - p);
  else
    val = p;
  if (val.len () == 0 || val.len () > 31)
    return NULL;
  for (u_int i = 0; i < val.len (); i++)
    if (!isalnum (val[i]) && val[i] != '_' && val[i] != '.' && val[i] != '-')
      return NULL;
  return val;
}

void
badauth (logincb_t cb, sfs_authtype atype, str msg, const sfsauth_dbrec *dbrp)
{
  static str badlogin ("bad login");
  sfsauth2_loginres res (SFSLOGIN_BAD);
  *res.errmsg = msg ? msg : badlogin;
  (*cb) (&res, atype, dbrp);
}
void
badauth (logincb_t cb, sfs_authtype atype, str msg, str user)
{
  if (user && user.len ()) {
    sfsauth_dbrec ae (SFSAUTH_USER);
    rpc_wipe (*ae.userinfo);
    ae.userinfo->name = user;
    ae.userinfo->id = badid;
    ae.userinfo->gid = badid;
    badauth (cb, atype, msg, &ae);
  }
  else
    badauth (cb, atype, msg, (sfsauth_dbrec *) NULL);
}
void
badauth (logincb_t cb, sfs_authtype atype, str msg, ref<sfspub> pub)
{
  sfsauth_dbrec ae (SFSAUTH_USER);
  rpc_wipe (*ae.userinfo);
  ae.userinfo->id = badid;
  ae.userinfo->gid = badid;
  if (pub->export_pubkey (&ae.userinfo->pubkey))
    badauth (cb, atype, msg, &ae);
  else
    badauth (cb, atype, msg, NULL);
}

struct passwd *
unix_user (str user, str pwd, str *errp)
{
  struct passwd *pw = getpwnam (user);
  if (!pw) {
    if (errp)
      *errp = "Invalid login";
    return NULL;
  }

  time_t now = time (NULL);
  bool expired = false;
#if HAVE_GETSPNAM
  struct spwd *spe = getspnam (user);
  if (spe && spe->sp_expire > 0
      && spe->sp_expire <= (now / (24 * 60 * 60)))
    expired = true;
#elif defined (HAVE_PASSWD_PW_EXPIRE)
  if (pw->pw_expire > 0 && pw->pw_expire <= now)
    expired = true;
#endif /* HAVE_PASSWD_PW_EXPIRE */
  if (expired) {
    if (errp)
      *errp = "Login expired";
    return NULL;
  }

  if (!pwd)
    return pw;
#if HAVE_GETSPNAM
  if (spe) {
    if (!strcmp (spe->sp_pwdp, crypt (pwd, spe->sp_pwdp)))
      return pw;
  }
  else
#endif /* HAVE_GETSPNAM */
    if (!strcmp (pw->pw_passwd, crypt (pwd, pw->pw_passwd)))
      return pw;

  if (errp)
    *errp = "Invalid password";
  /* Yes, some people believe you should just say invalid login here,
   * but I don't think the additional security through obscurity of
   * login names is worth the potential confusion to users. */
  return NULL;
}

void
setuser_pkhash (sfsauth2_loginres *resp, ptr<sfspub> vrfy)
{
  str h;
  if (!(h = vrfy->get_pubkey_hash ())) {
    warn << "Error in sha1_hashxdr of user's public key\n";
    return;
  }

  vec<sfsauth_cred> v;
  size_t n = resp->resok->creds.size ();

  for (size_t i = 0; i < n; i++)
    v.push_back (resp->resok->creds[i]);

  v.push_back ();
  v[n].set_type (SFS_PKCRED);
  *v[n].pkhash = armor32 (h);

  resp->resok->creds.setsize (n + 1);
  for (size_t i = 0; i < n + 1; i++)
    resp->resok->creds[i] = v[i];
}

inline str
mkname (const dbfile *dbp, str name)
{
  str r;
  if (dbp->prefix)
    r = dbp->prefix << "/" << name;
  else
    r = name;
  return name;
}

inline bool
sourceok (str source)
{
  for (u_int i = 0; i < source.len (); i++)
    if (source[i] < 0x20 || source[i] >= 0x7f)
      return false;
  return true;
}

static str
method_str (sfs_authtype atype)
{
  switch (atype) {
  case SFS_AUTHREQ:
    return "old-style public key";
  case SFS_AUTHREQ2:
    return "public key";
  case SFS_UNIXPWAUTH:
    return "unix password";
  case SFS_SRPAUTH:
    return "SRP password";
  default:
    return "unknown auth method";
  }
}

void
authclnt::sfsauth_login (const sfsauth2_loginarg *lap,
			 logincb_t cb, bool self)
{
  sfs_autharg2 aa;
  sfsauth2_loginres res (SFSLOGIN_BAD);
  if (!sourceok (lap->source)) {
    *res.errmsg = "invalid source in login request";
    (*cb) (&res, SFS_NOAUTH, NULL);
    return;
  }
  if (!bytes2xdr (aa, lap->arg.certificate)) {
    *res.errmsg = "cannot unmarshal certificate";
    (*cb) (&res, SFS_NOAUTH, NULL);
    return;
  }

  if (authpending *ap = aptab[lap->authid]) {
    if (lap->arg.seqno == ap->seqno && aa.type == ap->atype) {
      ap->next (&aa, wrap (this, &authclnt::sfsauth_login_2, lap->source, cb));
      return;
    }
    warn << "canceling incomplete " << method_str (ap->atype)
	 << " login from " << lap->source << "\n";
    delete ap;
  }

  switch (aa.type) {
  case SFS_AUTHREQ:
  case SFS_AUTHREQ2:
    login_sigauth (lap, &aa, self, wrap (this, &authclnt::sfsauth_login_2,
					 lap->source, cb));
    return;
  case SFS_UNIXPWAUTH:
    login_unixpw (lap, &aa, self, wrap (this, &authclnt::sfsauth_login_2,
					lap->source, cb));
    return;
  case SFS_SRPAUTH:
    if (self || (uid && !*uid)) {
      login_srp (lap, &aa, self, wrap (this, &authclnt::sfsauth_login_2,
				       lap->source, cb));
      return;
    }
    else
      *res.errmsg = "SRP authentication of client to third party not allowed";
    break;
  case SFS_NOAUTH:
    *res.errmsg = "no authentication";
    (*cb) (&res, aa.type, NULL);
    return;
  default:
    *res.errmsg = strbuf ("unknown login type %d", aa.type);
    break;
  }

  sfsauth_login_2 (lap->source, cb, &res, aa.type, NULL);
}

void
authclnt::sfsauth_login_2 (str source, logincb_t cb,
			   sfsauth2_loginres *resp, sfs_authtype atype,
			   const sfsauth_dbrec *dbrp)
{
  str method = method_str (atype);
  if (resp->status == SFSLOGIN_OK && !resp->resok->creds.size ())
    resp->set_status (SFSLOGIN_BAD);
  if (resp->status == SFSLOGIN_OK) {
    if (resp->resok->creds[0].type == SFS_UNIXCRED)
      warn << "accepted user " << resp->resok->creds[0].unixcred->username
	   << " from " << source
	   << " using " << method << "\n";
    else if (resp->resok->creds[0].type == SFS_PKCRED)
      warn << "accepted pubkey " << *resp->resok->creds[0].pkhash
	   << " from " << source
	   << " using " << method << "\n";
  }
  else if (resp->status == SFSLOGIN_BAD) {
    str msg;
    if (resp->errmsg->len ())
      msg = strbuf () << " (" << *resp->errmsg << ")";
    else
      msg = "";

    str foruser = "";
    if (dbrp && dbrp->type == SFSAUTH_USER) {
      if (dbrp->userinfo->name.len ())
	foruser = strbuf () << " for " << dbrp->userinfo->name;
      else if (ptr<sfspub> pk = sfscrypt.alloc (dbrp->userinfo->pubkey))
	foruser = strbuf () << " for keyhash "
			    << armor32 (pk->get_pubkey_hash ());
    }

    warn << "BAD login" << foruser << " from " << source
	 << " using " << method << msg << "\n";
  }
  (*cb) (resp, atype, dbrp);
}

bool
authclnt::authreq_validate (sfsauth2_loginres *resp,
			    const sfsauth2_loginarg *lap,
			    const sfs_authreq2 &areq, bool nocred)
{
  if (areq.type != SFS_SIGNED_AUTHREQ
      && (!nocred || areq.type == SFS_SIGNED_AUTHREQ_NOCRED)) {
    resp->set_status (SFSLOGIN_BAD);
    *resp->errmsg = "malformed authentication request (bad type)";
    return false;
  }
  if (areq.authid != lap->authid) {
    resp->set_status (SFSLOGIN_BAD);
    *resp->errmsg = "bad authid in authentication request";
    return false;
  }
  if (areq.seqno != lap->arg.seqno) {
    resp->set_status (SFSLOGIN_BAD);
    *resp->errmsg = "sequence number mismatch in authentication request";
    return false;
  }
  return true;
}

void
authclnt::login_sigauth (const sfsauth2_loginarg *lap, const sfs_autharg2 *aap,
			 bool self, logincb_t cb)
{
  sfsauth2_loginres res (SFSLOGIN_BAD);
  ptr<sfspub> vrfy;
  sfs_msgtype mtype;
  str logname;

  if (aap->type == SFS_AUTHREQ) {
    const sfs_pubkey &kp = aap->authreq1->usrkey;
    if (!(vrfy = sfscrypt.alloc (kp, SFS_VERIFY))) {
      badauth (cb, aap->type, "cannot load public Rabin key", NULL);
      return;
    }
    sfs_signed_authreq authreq;
    str msg;
    if (!vrfy->verify_r (aap->authreq1->signed_req, sizeof (authreq), msg)
	|| !str2xdr (authreq, msg)
	|| (authreq.type != SFS_SIGNED_AUTHREQ && 
	    authreq.type != SFS_SIGNED_AUTHREQ_NOCRED)
	|| authreq.seqno != lap->arg.seqno
	|| authreq.authid != lap->authid) {
      badauth (cb, aap->type, "bad signature", vrfy);
      return;
    }
    mtype = authreq.type;
    if (authreq.usrinfo[0]) {
      if (memchr (authreq.usrinfo.base (), 0, authreq.usrinfo.size ()))
	logname = authreq.usrinfo.base ();
      else
	logname.setbuf (authreq.usrinfo.base (), authreq.usrinfo.size ());
    }
  }
  else {
    if (aap->sigauth->req.user.len ())
      logname = aap->sigauth->req.user;
    mtype = aap->sigauth->req.type;
    str e;
    if (!(vrfy = sfscrypt.alloc (aap->sigauth->key, SFS_VERIFY))) {
      badauth (cb, aap->type, "cannot load public key", NULL);
      return; 
    }
    if (!vrfy->verify (aap->sigauth->sig,
			    xdr2str (aap->sigauth->req), &e)) {
      badauth (cb, aap->type, e, vrfy);
      return;
    }
    if (!authreq_validate (&res, lap, aap->sigauth->req, true)) {
      (*cb) (&res, aap->type, NULL);
      return;
    }
  }

  for (dbfile *dbp = dbfiles.base (); dbp < dbfiles.lim (); dbp++) {
    ptr<authcursor> ac = dbp->db->open (dbp->dbflags);
    // XXX - in long form for aiding in debugging
    if (!ac)
      continue;
    if (!ac->find_user_pubkey (*vrfy))
      continue;
    if (ac->ae.type != SFSAUTH_USER)
      continue;
    if (logname) {
      if (dbp->prefix) {
	if (logname != dbp->prefix << "/" << ac->ae.userinfo->name) 
	  continue;
      } else {
	if (logname != ac->ae.userinfo->name)
	  continue;
      }
    }

    if (!(*vrfy == ac->ae.userinfo->pubkey))
      continue;

    if (mtype == SFS_SIGNED_AUTHREQ_NOCRED) {
      res.set_status (SFSLOGIN_OK);
      res.resok->creds.setsize (1);
      res.resok->creds[0].set_type (SFS_NOCRED);
    }
    else {
      if (!setuser (&res, ac->ae, dbp))
	continue;
      setuser_pkhash (&res, vrfy);
      setuser_groups (&res, &ac->ae, dbp, vrfy);
    }
    (*cb) (&res, aap->type, &ac->ae);
    return;
  }

  if (mtype != SFS_SIGNED_AUTHREQ_NOCRED) {
    res.set_status (SFSLOGIN_OK);
    setuser_pkhash (&res, vrfy);
    setuser_groups (&res, NULL, NULL, vrfy);
    (*cb) (&res, aap->type, NULL);
  }
  else
    badauth (cb, aap->type, "signed login of type AUTHREQ_NOCRED", vrfy);
}

inline dbfile *
pwdb ()
{
  static bool initialized;
  static dbfile *unix_dbp;
  if (!initialized) {
    for (dbfile *dbp = dbfiles.base ();
	 !unix_dbp && dbp < dbfiles.lim (); dbp++)
      if (dbp->allow_unix_pwd)
	unix_dbp = dbp;
    initialized = true;
  }
  return unix_dbp;
}

void
authclnt::login_unixpw (const sfsauth2_loginarg *lap, const sfs_autharg2 *aap,
			bool self, logincb_t cb)
{
  str2wstr (aap->pwauth->password);
  sfsauth2_loginres res;
  if (!authreq_validate (&res, lap, aap->pwauth->req)) {
    (*cb) (&res, aap->type, NULL);
    return;
  }
  if (!self || !uid) {	// This is safest, but debatable
    badauth (cb, aap->type, "remote unix-style login disallowed",
	     aap->pwauth->req.user);
    return;
  }
  dbfile *dbp = pwdb ();
  if (!dbp) {
    badauth (cb, aap->type, "Unix password authentication not allowed",
	     aap->pwauth->req.user);
    return;
  }

  str pwd = aap->pwauth->password;
  if (!pwd.len () && self && uid && !*uid)
    pwd = NULL;

  ptr<authcursor> ac = dbp->db->open (dbp->dbflags);
  if (!ac) {
    badauth (cb, aap->type, "authentication database error",
	     aap->pwauth->req.user);
    return;
  }

  str unixname;
  if (ac->find_user_name (aap->pwauth->req.user)) {
    unixname = unixpriv (ac->ae.userinfo->privs);
    if (!unixname) // For compatibility before unix= priv
      unixname = aap->pwauth->req.user;
    if (unixname && unixname != aap->pwauth->req.user) {
      /*  Note:  This policy is arguable, but the idea is that if you
       *  have multiple public keys mapped to the same Unix account,
       *  you don't necessarily want different users to be able to
       *  muck with each other's keys through sfskey register, even if
       *  they are all accessing the file system with the same UID.
       */ 
      badauth (cb, aap->type,
	       strbuf ("password login to %s rejected for"
		       " mismatched unix=%s priv\n",
		       aap->pwauth->req.user.cstr (), unixname.cstr ()),
	       &ac->ae);
      return;
    }
  }
  else {
    unixname = aap->pwauth->req.user;
    ac->ae.set_type (SFSAUTH_ERROR);
  }

  if (pwd && auth_helper) {
    authpending_helper *ahp (New authpending_helper (this, lap));
    ahp->ah_ac = ac;
    ahp->init (aap, cb);
  }
  else if (pwd) {
    authpending_unix *ahp (New authpending_unix (this, lap));
    ahp->ah_ac = ac;
    ahp->unixuser = unixname;
    ahp->init (aap, cb);
  }
  else {
    fatal ("Should not be reached\n");
    //login_unixpw_2 (ac, unixname, pwd, self, cb);
  }
}

void
authclnt::login_unixpw_2 (ref<authcursor> ac, str unixname,
			  str pwd, bool self, logincb_t cb)
{
  dbfile *dbp = pwdb ();
  assert (dbp);

  str err;
  struct passwd *pe = unix_user (unixname, pwd, &err);
  if (!pe) {
    if (ac->ae.type == SFSAUTH_USER)
      badauth (cb, SFS_UNIXPWAUTH, err, &ac->ae);
    else
      badauth (cb, SFS_UNIXPWAUTH, err, unixname);
    return;
  }
  /* The following is debatable, but safer to deny than allow... */
  if (uid && *uid && *uid != pe->pw_uid) {
    badauth (cb, SFS_UNIXPWAUTH, strbuf ("user %s does not match uid %d",
					 pe->pw_name, *uid), &ac->ae);
    return;
  }

  if (ac->ae.type != SFSAUTH_USER) {
    if ((!self || !uid || *uid) && !validshell (pe->pw_shell)) {
      badauth (cb, SFS_UNIXPWAUTH, "bad shell", unixname);
      return;
    }
    ac->ae.set_type (SFSAUTH_USER);
    ac->ae.userinfo->name = pe->pw_name;
    ac->ae.userinfo->id = pe->pw_uid;
    ac->ae.userinfo->vers = 0;
    ac->ae.userinfo->gid = pe->pw_gid;
    ac->ae.userinfo->privs = strbuf ("unix=%s,refresh=%d,timeout=%d",
				     pe->pw_name, dbp->default_refresh,
				     dbp->default_timeout);
  }

  sfsauth2_loginres res;
  bool ok = setuser (&res, ac->ae, dbp);
  if (ok)
    res.resok->resmore = mkname (dbp, ac->ae.userinfo->name);
  (*cb) (&res, SFS_UNIXPWAUTH, ok ? &ac->ae : NULL);
}


void
authclnt::login_srp (const sfsauth2_loginarg *lap, const sfs_autharg2 *aap,
		   bool self, logincb_t cb)
{
  sfsauth2_loginres res;
  if (!authreq_validate (&res, lap, aap->srpauth->req)) {
    (*cb) (&res, aap->type, NULL);
    return;
  }
  if ((!uid || *uid) && lap->authid != authid) {
    // SRP auth is mutual, so only root daemons
    badauth (cb, aap->type, "third-party SRP authentication refused",
	     aap->srpauth->req.user);
    return;
  }
  (New authpending_srp (this, lap))->init (aap, cb);
}

bool
authclnt::setuser (sfsauth2_loginres *resp, const sfsauth_dbrec &ae,
		   const dbfile *dbp)
{
  assert (ae.type == SFSAUTH_USER);

  resp->set_status (SFSLOGIN_OK);
  resp->resok->creds.setsize (1);
  resp->resok->creds[0].set_type (SFS_UNIXCRED);

  str name = mkname (dbp, ae.userinfo->name);

  bool require_unix = false;
  str unixname;
  if (dbp->allow_unix_pwd) {
    if ((unixname = unixpriv (ae.userinfo->privs)))
      require_unix = true;
    else // For compatibility before unix= priv
      unixname = ae.userinfo->name;
  }
      
  if (unixname) {
    str err;
    if (struct passwd *pw = unix_user (unixname, NULL, &err)) {
      resp->resok->creds[0].unixcred->username = unixname;
      resp->resok->creds[0].unixcred->homedir = pw->pw_dir;
      resp->resok->creds[0].unixcred->shell = pw->pw_shell;
      resp->resok->creds[0].unixcred->uid = pw->pw_uid;
      resp->resok->creds[0].unixcred->gid = pw->pw_gid;
    }
    else if (require_unix) {
      resp->set_status (SFSLOGIN_BAD);
      *resp->errmsg = err;
      return false;
    }
    else
      unixname = NULL;
  }

  if (!unixname) {
    resp->resok->creds[0].unixcred->username = name;
    resp->resok->creds[0].unixcred->homedir = "/dev/null";
    resp->resok->creds[0].unixcred->shell = "/dev/null";
    resp->resok->creds[0].unixcred->uid = dbp->uidmap->map (ae.userinfo->id);
    resp->resok->creds[0].unixcred->gid = dbp->gidmap->map (ae.userinfo->gid);
  }

  if (resp->resok->creds[0].unixcred->uid == badid) {
    resp->set_status (SFSLOGIN_BAD);
    *resp->errmsg = "uid out of range";
    return false;
  }

  vec<u_int32_t> groups;
  findgroups_unix (&groups, unixname ? unixname : name);
  resp->resok->creds[0].unixcred->groups.setsize (groups.size ());
  memcpy (resp->resok->creds[0].unixcred->groups.base (),
	  groups.base (), groups.size () * sizeof (groups[0]));
  return true;
}

void
authclnt::setuser_groups (sfsauth2_loginres *resp, const sfsauth_dbrec *ae,
                          const dbfile *dbp, ptr<sfspub> vrfy)
{
  vec<str> groups;
  str h = vrfy->get_pubkey_hash ();
  if (h)
    h = armor32 (h);
  if (dbp && ae)
    findgroups_symbolic (&groups, mkname (dbp, ae->userinfo->name),
                         &ae->userinfo->gid, h);
  else
    findgroups_symbolic (&groups, NULL, NULL, h);

  if (!groups.size ())
    return;

  vec<sfsauth_cred> v;
  size_t n = resp->resok->creds.size ();
  for (size_t i = 0; i < n; i++)
    v.push_back (resp->resok->creds[i]);

  v.push_back ();
  v[n].set_type (SFS_GROUPSCRED);
  v[n].groups->setsize (groups.size ());
  for (unsigned int i = 0; i < groups.size (); i++)
    (*v[n].groups)[i] = groups[i];

  resp->resok->creds.setsize (n + 1);
  for (size_t i = 0; i < n + 1; i++)
    resp->resok->creds[i] = v[i];
}

void
authclnt::findgroups_unix (vec<u_int32_t> *groups, str name)
{
  groups->clear ();
  bhash<u_int32_t> seen;
  for (dbfile *dbp = dbfiles.base (); dbp < dbfiles.lim (); dbp++) {
    str suffix = dbp->strip_prefix (name);
    if (!suffix)
      continue;

    if (ptr<authcursor> ac = dbp->db->open (dbp->dbflags)) {
      vec<u_int32_t> gv;
      ac->find_gids_user (&gv, suffix);
      while (!gv.empty ()) {
	u_int32_t gid = dbp->gidmap->map (gv.pop_front ());
	if (gid != badid && seen.insert (gid))
	  groups->push_back (gid);
      }
    }
  }
}

void
authclnt::findgroups_symbolic (vec<str> *groups, str name,
                               const u_int32_t *gidp, str pkhash)
{
  groups->clear ();
  if (!global_dbcache)  // symbolic groups are disabled
    return;

  if (name)
    groups->push_back (add_member_type (name, "u"));
  if (pkhash)
    groups->push_back (add_member_type (pkhash, "p"));
  global_dbcache->find_groups (groups);
}
