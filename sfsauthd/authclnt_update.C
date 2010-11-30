#include "sfsauthd.h"
#include "authgrps.h"
#include "rxx.h"

inline str
group_prefix (str s, str prefix)
{
  if (s.len () <= prefix.len () || s[prefix.len ()] != '.'
      || memcmp (s.cstr (), prefix.cstr (), prefix.len ()))
    return NULL;
  return substr (s, prefix.len () + 1);
}

bool
authclnt::update_checksig (svccb *sbp, update_info &i, dbfile *cdbp)
{
  str reqxdr = xdr2str (i.argp->req);
  if (i.argp->newsig) {
    str e;
    if (!sfscrypt.verify (i.argp->req.rec.userinfo->pubkey, *(i.argp->newsig), 
			  reqxdr, &e)) {
      *i.res.errmsg = str (strbuf ("new signature: " << e));
      sbp->replyref (i.res);
      return false;
    }
  }
  else if (!(i.opts & SFSUP_KPPK) && !i.admin) {
    *i.res.errmsg = "Missing signature with new public key.";
    sbp->replyref (i.res);
    return false;
  }

  if (i.argp->authsig) {
    str e;
    if (!sfscrypt.verify (i.cdbr.userinfo->pubkey, *(i.argp->authsig), 
			  reqxdr, &e)) {
      *i.res.errmsg = str (strbuf ("old signature: " << e));
      sbp->replyref (i.res);
      return false;
    }
  }
  else if (!cdbp->allow_unix_pwd || i.ur->authtype != SFS_UNIXPWAUTH) {
    *i.res.errmsg = "digital signature required";
    sbp->replyref (i.res);
    return false;
  }
  else
    i.admin = false;

  return true;
}

void
authclnt::update_user (svccb *sbp, update_info &i)
{
  dbfile *udbp;
  ptr<authcursor> uac;
  sfsauth_dbrec udbr;

  *i.kname.name = i.argp->req.rec.userinfo->name;
  if (!get_user_cursor (&udbp, &uac, &udbr, i.kname, true)) {
    *i.res.errmsg = "could not find or update user's record";
    return;
  }
  if (!i.admin && (udbr.userinfo->name != i.cdbr.userinfo->name
                 || udbr.userinfo->id != i.cdbr.userinfo->id)) {
    /* XXX - ignoring owner field for now */
    *i.res.errmsg = "access denied";
    return;
  }
  if (i.argp->req.rec.userinfo->vers < 1) {
    *i.res.errmsg = "version number of record must be greater than 0";
    return;
  }
  if (i.argp->req.rec.userinfo->vers != udbr.userinfo->vers + 1) {
    *i.res.errmsg = "version mismatch";
    return;
  }
  uac->ae.userinfo->vers = i.argp->req.rec.userinfo->vers;
  if (!(i.opts & SFSUP_KPPK))
    uac->ae.userinfo->pubkey = i.argp->req.rec.userinfo->pubkey;
  if (!(i.opts & SFSUP_KPSRP))
    uac->ae.userinfo->pwauth = i.argp->req.rec.userinfo->pwauth;
  if (!(i.opts & SFSUP_KPESK))
    uac->ae.userinfo->privkey = i.argp->req.rec.userinfo->privkey;

  str err = update_srv_keyhalf (i.argp->req.rec.userinfo->srvprivkey,
                                uac->ae.userinfo->srvprivkey, 
                                udbr.userinfo->srvprivkey, true, i.ur);
  if (err) {
    *i.res.errmsg = err;
    return;
  }

  strbuf sb;
  sb << "Last modified " << timestr () << " by " ;
  if (uid && !*uid)
    sb << "*superuser*";
  else
    sb << i.cp->unixcred->username;
  sb << "@" << client_name;
  uac->ae.userinfo->audit = sb;

  if (i.admin) {
    u_int32_t gid = i.argp->req.rec.userinfo->gid;
    if (udbp->gidmap && gid != udbp->gidmap->map (uac->ae.userinfo->gid)) {
      gid = udbp->gidmap->unmap (gid);
      if (gid == badid
          || udbp->gidmap->map (gid) != i.argp->req.rec.userinfo->gid) {
        *i.res.errmsg = "bad gid";
        return;
      }
    }
    uac->ae.userinfo->gid = gid;
    uac->ae.userinfo->privs = i.argp->req.rec.userinfo->privs;
  }

  if (!uac->update ()) {
    *i.res.errmsg = "database refused update";
    return;
  }
  i.res.set_ok (true);
  udbp->mkpub ();
}

void
authclnt::update_group (svccb *sbp, update_info &i)
{
  dbfile *udbp;
  ptr<authcursor> uac;
  sfsauth_dbrec udbr;

  *i.kname.name = i.argp->req.rec.groupinfo->name;
  bool create = i.argp->req.rec.groupinfo->id == 0
    && i.argp->req.rec.groupinfo->vers == 1;
  bool exists = get_group_cursor (&udbp, &uac, &udbr, i.kname, true, create);

  if (exists && create && udbr.groupinfo->id > udbp->grprange->id_max) {
    *i.res.errmsg = strbuf () << "all group IDs in the allowed range ("
                              << udbp->grprange->id_min << "-"
                              << udbp->grprange->id_max << ") are in use";
    return;
  }

  if (!exists) {
    if (!create) {
      *i.res.errmsg = "perhaps record is read-only "
                      "or database doesn't accept group updates";
      return;
    }
    else {
      *i.res.errmsg = "no writable databases that accept group updates";
      return;
    }
  }
  if (create && udbr.groupinfo->vers != 0) {
    *i.res.errmsg = strbuf () << "group `" << udbr.groupinfo->name
                              << "'already exists";
    return;
  }

  if (!i.admin) {
    if (create) {
      str gname;
      if (!(gname = group_prefix (udbr.groupinfo->name, i.cdbr.userinfo->name))
          || gname.len () < 1) {
        *i.res.errmsg = strbuf () << "group name must be of the form `"
                                << i.cdbr.userinfo->name << ".groupname'";
        return;
      }
      static rxx groupquotarx ("(\\A|,)groupquota=([0-9]+)(\\Z|,)");
      if (groupquotarx.search (i.cdbr.userinfo->privs)
          || udbp->default_groupquota >= 0) {
        u_int32_t max_groups;
        u_int32_t cur_groups;
        
        if (groupquotarx.success ())
          convertint (groupquotarx[2], &max_groups);
        else
          max_groups = udbp->default_groupquota;

        // XXX - open could fail
        ptr<authcursor> gac = udbp->db->open (udbp->dbflags);
        cur_groups = gac->count_group_prefix (strbuf ()
                                              << i.cdbr.userinfo->name << ".");
        if (cur_groups + 1 > max_groups) {
          *i.res.errmsg = strbuf () << "group quota exceeded (current="
                                    << cur_groups << "/quota=" << max_groups << ")";
          return;
        }
      }
    }
    else {
      ptr<sfspub> pk = sfscrypt.alloc (i.cdbr.userinfo->pubkey);
      str h = armor32 (pk->get_pubkey_hash ());
      sfs_groupmembers list;
      unsigned int n = udbr.groupinfo->owners.size ();
      for (unsigned int j = 0; j < n; j++)
        list.push_back (udbr.groupinfo->owners[j]);
      
      if (!group_prefix (udbr.groupinfo->name, i.cdbr.userinfo->name)
          && !is_a_member (i.cdbr.userinfo->name, h, list)) {
        *i.res.errmsg = "access denied";
        return;
      }
    }
  }
  if (i.argp->req.rec.groupinfo->vers < 1) {
    *i.res.errmsg = "version number of record must be greater than 0";
    return;
  }
  if (i.argp->req.rec.groupinfo->vers != udbr.groupinfo->vers + 1) {
    *i.res.errmsg = "version mismatch";
    return;
  }
  uac->ae.groupinfo->vers = i.argp->req.rec.groupinfo->vers;

  strbuf sb;
  sb << "Last modified " << timestr () << " by " ;
  if (uid && !*uid)
    sb << "*superuser*";
  else
    sb << i.cp->unixcred->username;
  sb << "@" << client_name;
  uac->ae.groupinfo->audit = sb;

  // XXX: checking to make sure that the owners/groups are well-formed happens in update
  process_group_updates (udbr.groupinfo->owners, i.argp->req.rec.groupinfo->owners);
  process_group_updates (udbr.groupinfo->members, i.argp->req.rec.groupinfo->members);
  uac->ae.groupinfo->owners = udbr.groupinfo->owners;
  uac->ae.groupinfo->members = udbr.groupinfo->members;

  bool chlogok = write_group_changelog (i.argp->req.rec.groupinfo->name,
                                        i.argp->req.rec.groupinfo->vers,
                                        i.argp->req.rec.groupinfo->members,
                                        sb);
  if (!chlogok) {
    *i.res.errmsg = "could not write changelog; database unmodified";
    return;
  }

  if (!uac->update ()) {
    // XXX: remove changelog entry
    *i.res.errmsg = "database refused update; see SFS documentation for correct syntax";
    return;
  }
  i.res.set_ok (true);
  udbp->mkpub ();
  uac->find_group_name (udbr.groupinfo->name);
  if (global_dbcache)
    update_dbcache (uac);
}

void
authclnt::sfsauth_update (svccb *sbp)
{
  update_info i;

  i.cp = NULL;
  i.ur = NULL;
  if (sbp->getaui () >= credtab.size ()
      || !(i.cp = &credtab[sbp->getaui ()]) 
      || i.cp->type != SFS_UNIXCRED 
      || !(i.ur = utab[sbp->getaui ()])
      || i.ur->authtype == SFS_NOAUTH) {
    sbp->reject (AUTH_REJECTEDCRED);
    return;
  }

  i.res.set_ok (false);
  i.kname.set_type (SFSAUTH_DBKEY_NAME);
  *i.kname.name = i.cp->unixcred->username;

  dbfile *cdbp;
  if (!get_user_cursor (&cdbp, NULL, &i.cdbr, i.kname)
      || i.cp->unixcred->username != i.cdbr.userinfo->name) {
    *i.res.errmsg = "could not load credential db record";
    sbp->replyref (i.res);
    return;
  }

  if (i.cp->unixcred->uid != i.cdbr.userinfo->id) {
    *i.res.errmsg = "invalid uid";
    warn << i.cp->unixcred->username << " authenticated with uid "
	 << i.cp->unixcred->uid << " while DB record has uid "
	 << i.cdbr.userinfo->id << "\n";
    warn << "could user " << i.cp->unixcred->username << " have"
	 << " wrong UID in sfs_users file?\n";
    sbp->replyref (i.res);
    return;
  }

  i.argp = sbp->Xtmpl getarg<sfsauth2_update_arg> ();
  if (i.argp->req.type != SFS_UPDATEREQ
      || (i.argp->req.rec.type != SFSAUTH_USER
	  && i.argp->req.rec.type != SFSAUTH_GROUP)) {
    *i.res.errmsg = "invalid request";
    sbp->replyref (i.res);
    return;
  }
  i.opts = i.argp->req.opts;
  if (i.argp->req.authid != authid) {
    *i.res.errmsg = "invalid authid";
    sbp->replyref (i.res);
    return ;
  }

  static rxx adminrx ("(\\A|,)admin(\\Z|,)");
  i.admin = cdbp->allow_admin && adminrx.search (i.cdbr.userinfo->privs);

  if (!update_checksig (sbp, i, cdbp))
    return;

  if (i.argp->req.rec.type == SFSAUTH_USER)
    update_user (sbp, i);
  else
    update_group (sbp, i);

  sbp->replyref (i.res);
}
