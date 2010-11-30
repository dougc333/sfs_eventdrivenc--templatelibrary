/* $Id: authclnt.C,v 1.125 2005/06/17 20:12:16 kaminsky Exp $ */

/*
 *
 * Copyright (C) 2002-2004 David Mazieres (dm@uun.org)
 * Copyright (C) 2003 Michael Kaminsky (kaminsky@lcs.mit.edu)
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

#include "sfscrypt.h"
#include "sfsschnorr.h"
#include "sfsauthd.h"
#include "parseopt.h"
#include "sfskeymisc.h"
#include "authgrps.h"
#include "auth_helper_prot.h"

sprivk_tab_t sprivk_tab;

const str refresh_eq (",refresh=");
const str timeout_eq (",timeout=");

extern str unixpriv (const char *p);

static str
hash_sprivk (const sfs_2schnorr_priv_xdr &k)
{
  sfs_hash h;
  if (!sha1_hashxdr (&h, k))
    return NULL;
  return str (h.base (), h.size ());
}

bool
sprivk_tab_t::is_valid (const str &hv)
{
  assert (hv);
  bool ret;
  sprivk_t *s = keys[hv];
  if (!s)
    ret = false;
  else 
    ret = s->valid;
  return ret;
}

bool
sprivk_tab_t::invalidate (const str &hv)
{
  assert (hv);
  sprivk_t *s = keys[hv];
  if (!s)
    return false;
  s->valid = false;
  release (hv, s);
  return true;
}

void
sprivk_tab_t::bind (const str &hv)
{
  
  assert (hv);
  sprivk_t *s = keys[hv];
  if (s)
    s->refs++;
  else {
    nentries ++;
    keys.insert (hv);
  }
}

void
sprivk_tab_t::release (const str &hv, sprivk_t *s)
{
  assert (hv);
  if (!s)
    s = keys[hv];
  if (s && --s->refs == 0) {
    nentries --;
    keys.remove (hv);
  }
}

bool
authclnt::is_authenticated (svccb *sbp)
{
  const sfsauth_cred *cp = NULL;
  const urec_t *ur = NULL;

  return (sbp->getaui () < credtab.size ()
      && (cp = &credtab[sbp->getaui ()]) 
      && cp->type == SFS_UNIXCRED 
      && (ur = utab[sbp->getaui ()])
      && ur->authtype != SFS_NOAUTH);
}

void
authclnt::urecfree (urec_t *u)
{
  utab.remove (u);
  ulist.remove (u);
  delete u;
}

authclnt::urec_t::~urec_t ()
{
  if (kh.type == SFSAUTH_KEYHALF_PRIV) 
    for (u_int i = 0; i < kh.priv->size (); i++) 
      sprivk_tab.release (hash_sprivk ((*kh.priv) [i]));
}

authclnt::urec_t::urec_t (u_int32_t a, sfs_authtype t, 
			  const sfsauth_dbrec &dbr)
  : authno (a), authtype (t)
{
  if (dbr.type == SFSAUTH_USER) {
    uname = dbr.userinfo->name;
    kh = dbr.userinfo->srvprivkey;
    if (kh.type == SFSAUTH_KEYHALF_PRIV)
      for (u_int i = 0; i < kh.priv->size (); i++)
	sprivk_tab.bind (hash_sprivk ((*kh.priv) [i]));
  }
}

authclnt::authclnt (ref<axprt_crypt> x, const authunix_parms *aup)
  : sfsserv (x),
    authsrv (asrv::alloc (x, sfsauth_prog_2,
			  wrap (this, &authclnt::dispatch)))
{
  if (aup) {
    uid.alloc ();
    *uid = aup->aup_uid;
    if (!client_name || !client_name.len ()) {
      if (!*uid)
	client_name = "LOCAL";
      else
	client_name = strbuf ("LOCAL(uid=%d)", *uid);
    }
  }
}

authclnt::~authclnt ()
{
  ulist.traverse (wrap (this, &authclnt::urecfree));
  aptab.deleteall ();
}

ptr<sfspriv>
authclnt::doconnect (const sfs_connectarg *ci,
		     sfs_servinfo *si)
{
  *si = myservinfo;
  return myprivkey;
}

void
authclnt::dispatch (svccb *sbp)
{
  if (!sbp) {
    delete this;
    return;
  }
  switch (sbp->proc ()) {
  case SFSAUTH2_NULL:
    sbp->reply (NULL);
    break;
  case SFSAUTH2_LOGIN:
    sfsauth_login (sbp->Xtmpl getarg<sfsauth2_loginarg> (),
		   wrap (&authclnt::dispatch_2, sbp));
    break;
  case SFSAUTH2_QUERY:
    sfsauth_query (sbp);
    break;
  case SFSAUTH2_UPDATE:
    sfsauth_update (sbp);
    break;
  case SFSAUTH2_SIGN:
    sfsauth_sign (sbp);
    break;
  default:
    sbp->reject (PROC_UNAVAIL);
    break;
  }
}

void
authclnt::sfsauth_sign (svccb *sbp)
{
  sfsauth2_sign_arg *arg = sbp->Xtmpl getarg<sfsauth2_sign_arg> ();
  sfsauth2_sign_res res (true);
  u_int32_t authno = sbp->getaui ();
  sfsauth_dbrec db;
  bool restricted_sign = false;
  urec_t *ur = NULL;
  sfsauth_keyhalf *kh = NULL;
  sfs_idname uname;

  if (authno && (ur = utab[authno])) {
    kh = &ur->kh;
    uname = ur->uname;
  }

  if (!kh && arg->req.type == SFS_SIGNED_AUTHREQ
      && arg->req.authreq->type == SFS_SIGNED_AUTHREQ_NOCRED
      && arg->authinfo.service == SFS_AUTHSERV
      && authid == arg->req.authreq->authid) {
    sfsauth_dbkey key (SFSAUTH_DBKEY_NAME);
    if ((*key.name = arg->user) && get_user_cursor (NULL, NULL, &db, key)) {
      kh = &db.userinfo->srvprivkey;
      uname = db.userinfo->name;
      restricted_sign = true;
    }
  } 

  if (!kh || kh->type != SFSAUTH_KEYHALF_PRIV) {
    res.set_ok (false);
    *res.errmsg = "No valid server private keyhalf for user";
    sbp->replyref (res);
    return;
  }
  if (arg->presig.type != SFS_2SCHNORR) {
    res.set_ok (false);
    *res.errmsg = "Can only answer 2-Schnorr requests";
    sbp->replyref (res);
    return;
  }
  res.sig->set_type (SFS_SCHNORR);
  int i = sfs_schnorr_pub::find (*kh, arg->pubkeyhash);
  if (i < 0) {
    res.set_ok (false);
    *res.errmsg = "No matching keyhalf found on server.";
    sbp->replyref (res);
    return;
  }
  const sfs_2schnorr_priv_xdr &spriv = (*kh->priv)[i];
  if (ur && !sprivk_tab.is_valid (hash_sprivk (spriv))) {
    res.set_ok (false);
    *res.errmsg = "Server keyhalf is no longer valid.";
    sbp->replyref (res);
    return;
  }

  ref<schnorr_srv_priv> srv_key = New refcounted<schnorr_srv_priv> 
    (spriv.p, spriv.q, spriv.g, spriv.y, spriv.x);

  str msg = sigreq2str (arg->req);
  if (!msg) {
    res.set_ok (false);
    *res.errmsg = "Cannot marshal signature request";
    sbp->replyref (res);
    return ;
  }

  sfs_hash aid_tmp;
  if (arg->req.type != SFS_NULL && 
      (!sha1_hashxdr (aid_tmp.base (), arg->authinfo) || 
      !sigreq_authid_cmp (arg->req, aid_tmp))) {
    res.set_ok (false);
    *res.errmsg = "Incorrect authid in request";
    sbp->replyref (res);
    return ;
  }

  if (!siglog (siglogline (*arg, uname))) {
    res.set_ok (false);
    *res.errmsg = "Refusing to sign: could not log signature";
    sbp->replyref (res);
    return;
  }
    
  srv_key->endorse_signature (&res.sig->schnorr->r, &res.sig->schnorr->s, 
			      msg, arg->presig.schnorr->r);
  sbp->replyref (res);
}

str
authclnt::siglogline (const sfsauth2_sign_arg &arg, const str &uname)
{
  str req = xdr2str (arg);
  if (!req) return NULL;
  req = armor64 (req);
  str tm = single_char_sub (timestr (), ':', ".");
  strbuf line;
  line << "SIGN:" << uname << ":" << client_name << ":" << tm << ":" 
       << req << "\n";
  return line;
}

bool
siglog (const str &line)
{
  if (!line) return false;
  int n = write (logfd, line.cstr (), line.len ());
  if (n < int (line.len ())) 
    return false;
  return true;
}

str 
siglog_startup_msg ()
{
  strbuf msg;
  str tm = single_char_sub (timestr (), ':', ".");
  msg << "sfsauthd restarted: " << tm << "\n";
  return msg;
}

void
siglogv ()
{
  if (!siglog (siglog_startup_msg ()))
    fatal << "Cannot generate startup message for signature log\n";
}

void
authclnt::sfs_login (svccb *sbp)
{
  if (!authid_valid) {
    sbp->replyref (sfs_loginres (SFSLOGIN_ALLBAD));
    return;
  }
  sfsauth2_loginarg la;
  la.arg = *sbp->Xtmpl getarg<sfs_loginarg> ();
  la.authid = authid;
  la.source = client_name << "!" << progname;

  sfsauth_login (&la, wrap (this, &authclnt::sfs_login_2, sbp), true);
}

void
authclnt::sfs_login_2 (svccb *sbp, sfsauth2_loginres *resp,
		       sfs_authtype atype, const sfsauth_dbrec *dbrp)
{
  sfs_loginarg *lap = sbp->Xtmpl getarg<sfs_loginarg> ();
  sfsauth2_loginres &lr = *resp;

  sfs_loginres res (lr.status);
  switch (lr.status) {
  case SFSLOGIN_OK:
    if (!seqstate.check (lap->seqno) || lr.resok->creds.size () < 1)
      res.set_status (SFSLOGIN_BAD);
    else {
      u_int32_t authno;
      authno = authalloc (lr.resok->creds.base (), 
			  lr.resok->creds.size ());
      if (!authno) {
	warn << "ran out of authnos (or bad cred type)\n";
	res.set_status (SFSLOGIN_BAD);
      }
      else {
	utab_insert (authno, atype, *dbrp);
	res.resok->authno = authno;
	res.resok->resmore = resp->resok->resmore;
	res.resok->hello = resp->resok->hello;
      }
    }
    break;
  case SFSLOGIN_MORE:
    *res.resmore = *lr.resmore;
    break;
  default:
    break;
  }

  sbp->replyref (res);
}

void
authclnt::utab_insert (u_int32_t authno, sfs_authtype at,
		       const sfsauth_dbrec &dbr)
{
  urec_t *u = utab[authno];
  if (u) 
    urecfree (u);
  urec_t *ur = New urec_t (authno, at, dbr);
  utab.insert (ur);
  ulist.insert_head (ur);
}

void
authclnt::sfs_logout (svccb *sbp)
{
  u_int32_t authno = *sbp->Xtmpl getarg<u_int32_t> ();
  urec_t *u = utab[authno];
  if (u) 
    urecfree (u);
  sfsserv::sfs_logout (sbp);
}

// XXX - this function doesn't make sense for multi-round authentication
#if 0
inline str
arg2user (const sfs_autharg2 &aa)
{
  str user;
  switch (aa.type) {
  case SFS_AUTHREQ2:
    user = aa.sigauth->req.user;
    break;
  case SFS_UNIXPWAUTH:
    user = aa.pwauth->req.user;
    break;
  case SFS_SRPAUTH:
    user = aa.srpauth->req.user;
    break;
  default:
    user = "";
    break;
  }

  for (const char *p = user; *p; p++)
    if (*p <= ' ' || *p >= 127) {
      user = "";
      break;
    }
  if (user.len ())
    return user;

  switch (aa.type) {
  case SFS_AUTHREQ:
    if (ptr<sfspub> pk = sfscrypt.alloc (aa.authreq1->usrkey))
      user = strbuf ("keyhash ") << armor32 (pk->get_pubkey_hash ());
    break;
  case SFS_AUTHREQ2:
    if (ptr<sfspub> pk = sfscrypt.alloc (aa.sigauth->key))
      user = strbuf ("keyhash ") << armor32 (pk->get_pubkey_hash ());
    break;
  default:
    break;
  }

  if (!user || !user.len ())
    user = "<unknown user>";
  return user;
}
#endif

str
authclnt::update_srv_keyhalf (const sfsauth_keyhalf &updkh,
			      sfsauth_keyhalf &newkh,
			      const sfsauth_keyhalf &oldkh,
			      bool canclear, urec_t *ur)
{
  bool hasoldkh = false;
  const sfsauth_keyhalf_type &kht = updkh.type;
  if (kht == SFSAUTH_KEYHALF_NONE)
    return NULL;

  newkh.set_type (SFSAUTH_KEYHALF_PRIV);
  u_int okeys = 0;
  if (oldkh.type == SFSAUTH_KEYHALF_PRIV)
    okeys =  oldkh.priv->size ();
  u_int nkeys;
  if (oldkh.type == SFSAUTH_KEYHALF_PRIV && okeys >= 1) {
    hasoldkh = true;
    if (kht == SFSAUTH_KEYHALF_DELTA) {
      nkeys = okeys;
      for (u_int i = 1; i < okeys; i++) 
	(*newkh.priv)[i] = (*oldkh.priv)[i];
    } else {
      nkeys = (okeys == SPRIVK_HISTORY_LEN) ? okeys : okeys + 1;
      newkh.priv->setsize (nkeys);
      for (u_int i = 1; i < nkeys; i++)
	(*newkh.priv)[i] = (*oldkh.priv)[i-1];
    }
  } else {
    nkeys = 1;
    newkh.priv->setsize (1);
  }

  if (kht == SFSAUTH_KEYHALF_DELTA) {
    if (!hasoldkh) 
      return "Cannot apply key delta: no key currently exists!";
    (*newkh.priv)[0] = (*oldkh.priv)[0];
    (*newkh.priv)[0].x += *updkh.delta;
    (*newkh.priv)[0].x %= (*newkh.priv)[0].q;
    sprivk_tab.invalidate (hash_sprivk ((*oldkh.priv)[0]));
    sprivk_tab.bind (hash_sprivk ((*newkh.priv)[0]));
  } else if (kht == SFSAUTH_KEYHALF_PRIV) {
    if (!canclear)
      return "Can only explicitly set server keyhalf on register or signed "
	     "update.";
    if (nkeys == okeys) 
      sprivk_tab.invalidate (hash_sprivk ((*oldkh.priv)[okeys - 1]));
    (*newkh.priv)[0] = (*updkh.priv)[0];
    sprivk_tab.bind (hash_sprivk ((*newkh.priv)[0]));
  }
  ur->kh = newkh;
  
  return NULL;
}

bool
get_user_cursor (dbfile **dbpp, ptr<authcursor> *acp,
		 sfsauth_dbrec *dbrp, const sfsauth_dbkey &key,
		 bool writable)
{
  if (key.type != SFSAUTH_DBKEY_NAME && key.type != SFSAUTH_DBKEY_ID
      && key.type != SFSAUTH_DBKEY_PUBKEY) {
    if (dbrp) {
      dbrp->set_type (SFSAUTH_ERROR);
      *dbrp->errmsg = strbuf ("unsupported key type %d", key.type);
    }
    return false;
  }
  ptr<sfspub> pk;
  for (dbfile *dbp = dbfiles.base (); dbp < dbfiles.lim (); dbp++) {
    if (writable && !dbp->allow_update)
      continue;
    u_int flags = dbp->dbflags;
    if (writable)
      flags |= authdb::AUDB_WRITE;
    ptr<authcursor> ac = dbp->db->open (flags);
    if (!ac)
      continue;
    switch (key.type) {
    case SFSAUTH_DBKEY_NAME:
      {
	struct passwd *pw;
	str name = dbp->strip_prefix (*key.name);
	if (!name)
	  continue;
	else if (ac->find_user_name (name))
	  break;
	else if (dbp->allow_unix_pwd && (pw = getpwnam (name))) {
	  sfsauth_dbrec rec (SFSAUTH_USER);
	  rec.userinfo->name = *key.name;
	  rec.userinfo->id = pw->pw_uid;
	  rec.userinfo->vers = 0;
	  rec.userinfo->gid = pw->pw_gid;
	  rec.userinfo->privs = strbuf ("unix=%s,refresh=%d,timeout=%d",
					pw->pw_name, dbp->default_refresh,
					dbp->default_timeout);
	  ac->ae = rec;
	  break;
	} else 
	  continue;
      }
    case SFSAUTH_DBKEY_ID:
      {
	u_int32_t id = dbp->uidmap ? dbp->uidmap->unmap (*key.id) : *key.id;
	if (id == badid || !ac->find_user_uid (id)) {
	  struct passwd *pw;
	  if (dbp->allow_unix_pwd && (pw = getpwuid (*key.id))) {
	    sfsauth_dbrec rec (SFSAUTH_USER);
	    rec.userinfo->name = pw->pw_name;
	    rec.userinfo->id = pw->pw_uid;
	    rec.userinfo->vers = 0;
	    rec.userinfo->gid = pw->pw_gid;
	    rec.userinfo->privs = strbuf ("unix=%s,refresh=%d,timeout=%d",
					  pw->pw_name, dbp->default_refresh,
					  dbp->default_timeout);
	    ac->ae = rec;
	    break;
	  }
	  continue;
	}
	break;
      }
    case SFSAUTH_DBKEY_PUBKEY:
      if (!pk) {
	if (!(pk = sfscrypt.alloc (*key.key))) {
	  warn << "Cannot import user public key.\n";
          if (dbrp) {
            dbrp->set_type (SFSAUTH_ERROR);
            *dbrp->errmsg = "cannot import user public key";
          }
	  return false;
	}
      }
      if (!(ac->find_user_pubkey (*pk) && *pk == ac->ae.userinfo->pubkey))
	continue;
      break;
    default:
      panic ("unreachable\n");
    }
    if (dbp->allow_unix_pwd)
      if (str u = unixpriv (ac->ae.userinfo->privs))
	if (struct passwd *pw = getpwnam (u)) {
	  if (ac->ae.userinfo->id != pw->pw_uid) {
	    warn << "overriding uid " << ac->ae.userinfo->id << " with "
		 << pw->pw_uid << " for unix privs " << u << "\n";
	    ac->ae.userinfo->id = pw->pw_uid;
	  }
	  if (ac->ae.userinfo->gid != pw->pw_gid) {
	    warn << "overriding gid " << ac->ae.userinfo->gid << " with "
		 << pw->pw_gid << " for unix privs " << u << "\n";
	    ac->ae.userinfo->gid = pw->pw_gid;
	  }
	}
    if (dbpp)
      *dbpp = dbp;
    if (acp)
      *acp = ac;
    if (dbrp) {
      *dbrp = ac->ae;
      aesanitize (dbrp, AE_QUERY);
      if (dbp->prefix)
	dbrp->userinfo->name = dbp->prefix << "/" << dbrp->userinfo->name;
      if (dbp->uidmap)
	dbrp->userinfo->id = dbp->uidmap->map (dbrp->userinfo->id);
      if (dbrp->userinfo->id == badid)
	continue;
      if (dbp->gidmap)
	dbrp->userinfo->gid = dbp->gidmap->map (dbrp->userinfo->gid);
    }
    return true;
  }
  if (dbrp) {
    dbrp->set_type (SFSAUTH_ERROR);
    *dbrp->errmsg = "user not found";
  }
  return false;
}

bool
get_group_cursor (dbfile **dbpp, ptr<authcursor> *acp,
                  sfsauth_dbrec *dbrp, const sfsauth_dbkey &key,
                  bool writable, bool create)
{
  if (key.type != SFSAUTH_DBKEY_NAME && key.type != SFSAUTH_DBKEY_ID) {
    if (dbrp) {
      dbrp->set_type (SFSAUTH_ERROR);
      *dbrp->errmsg = strbuf ("unsupported key type %d", key.type);
    }
    return false;
  }
  for (dbfile *dbp = dbfiles.base (); dbp < dbfiles.lim (); dbp++) {
    if (writable && (!dbp->allow_update || !dbp->grprange))
      continue;
    u_int flags = dbp->dbflags;
    if (writable)
      flags |= authdb::AUDB_WRITE;
    ptr<authcursor> ac = dbp->db->open (flags);
    if (!ac)
      continue;
    switch (key.type) {
    case SFSAUTH_DBKEY_NAME:
      {
	str name = dbp->strip_prefix (*key.name);
	if (!name)
	  continue;
	else if (ac->find_group_name (name))
	  break;
	else if (create) {
	  u_int32_t gid = ac->alloc_gid (dbp->grprange->id_min,
	                                 dbp->grprange->id_max);
	  sfsauth_dbrec rec (SFSAUTH_GROUP);
	  rec.groupinfo->name = *key.name;
	  rec.groupinfo->id = gid;
	  rec.groupinfo->vers = 0;
	  //rec.groupinfo->refresh = dbp->default_refresh;
	  //rec.groupinfo->timeout = dbp->default_timeout;
	  ac->ae = rec;
	  break;
	} else 
	  continue;
      }
    case SFSAUTH_DBKEY_ID:
      {
	u_int32_t id = dbp->gidmap ? dbp->gidmap->unmap (*key.id) : *key.id;
	if (id == badid || !ac->find_group_gid (id))
	  continue;
	break;
      }
    default:
      panic ("unreachable\n");
    }
    if (dbpp)
      *dbpp = dbp;
    if (acp)
      *acp = ac;
    if (dbrp) {
      *dbrp = ac->ae;
      if (dbp->prefix)
	dbrp->groupinfo->name = dbp->prefix << "/" << dbrp->groupinfo->name;
      if (dbp->gidmap)
	dbrp->groupinfo->id = dbp->gidmap->map (dbrp->groupinfo->id);
      if (dbrp->groupinfo->id == badid)
	continue;
    }
    return true;
  }
  if (dbrp) {
    dbrp->set_type (SFSAUTH_ERROR);
    *dbrp->errmsg = "group not found";  // WARNING: Do not change the text of
                                        // this string; sfsgroupmgr.C uses it

  }
  return false;
}
