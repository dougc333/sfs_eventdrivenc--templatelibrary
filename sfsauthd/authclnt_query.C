#include "sfsauthd.h"
#include "authgrps.h"

/* XXX - this is needed for backwards compatibility with SFS 0.7,
 * because there was an extra hyper in the protocol we needed to get
 * rid of (but old clients still expect the hyper). */
static BOOL
xdr_dbrec_plus_hyper (XDR *xdrs, void *dbrec)
{
  if (!xdr_sfsauth_dbrec (xdrs, dbrec))
    return FALSE;
  if (xdrs->x_op == XDR_ENCODE && !xdr_puthyper (xdrs, 0))
    return FALSE;
  return TRUE;
}

void
authclnt::query_user (svccb *sbp)
{
  sfsauth2_query_arg *arg = sbp->Xtmpl getarg<sfsauth2_query_arg> ();
  ptr<authcursor> ac;
  sfsauth2_query_res res;

  if (!get_user_cursor (NULL, &ac, &res, arg->key)) {
    if (res.type != SFSAUTH_ERROR) {
      res.set_type (SFSAUTH_ERROR);
      *res.errmsg = "bad user";
    }
  }
  else if (sbp->getaui () < credtab.size ()) {
    const sfsauth_cred &c = credtab[sbp->getaui ()];
    const urec_t *ur = utab[sbp->getaui ()];
    if (ur && c.type == SFS_UNIXCRED
	&& c.unixcred->username == res.userinfo->name
	&& c.unixcred->uid == res.userinfo->id
	&& ur->authtype == SFS_SRPAUTH) {
      aesanitize (&res, AE_USER);
      res.userinfo->privkey = ac->ae.userinfo->privkey;
    }
    else if (c.type == SFS_UNIXCRED)
      aesanitize (&res, AE_PUBFILE);
    else
      aesanitize (&res, AE_QUERY);
  }
  else
    aesanitize (&res, AE_QUERY);
  
  sbp->reply (&res, xdr_dbrec_plus_hyper);
}

void
authclnt::query_srpparms (svccb *sbp)
{
  sfsauth2_query_res res (SFSAUTH_SRPPARMS);
  if (!srpparms) {
    res.set_type (SFSAUTH_ERROR);
    *res.errmsg = "No SRP information available";
  }
  else
    res.srpparms->parms = srpparms;
  sbp->replyref (res);
}

void
authclnt::query_certinfo (svccb *sbp)
{
  sfsauth2_query_res res (SFSAUTH_CERTINFO);
  if (sfsauthrealm.len () > 0) {
    res.certinfo->name = sfsauthrealm;
    res.certinfo->info.set_status (SFSAUTH_CERT_REALM);
    res.certinfo->info.certpaths->set (sfsauthcertpaths.base (), 
				       sfsauthcertpaths.size ());
  }
  else {
    res.certinfo->name = myservinfo.cr7->host.hostname;
    res.certinfo->info.set_status (SFSAUTH_CERT_SELF);
  }
  sbp->replyref (res);
  return ;

}

void
authclnt::query_group (svccb *sbp)
{
  sfsauth2_query_arg *arg = sbp->Xtmpl getarg<sfsauth2_query_arg> ();
  sfsauth2_query_res res;
  dbfile *dbp;
  sfsauth_dbkey key = arg->key;
  unsigned int start = 0;
  bool more = false;

  if (arg->key.type == SFSAUTH_DBKEY_NAMEVERS) {
    key.set_type (SFSAUTH_DBKEY_NAME);
    *key.name = arg->key.namevers->name;
    start = arg->key.namevers->vers;
  }

  if (get_group_cursor (&dbp, NULL, &res, key)) {
    // Send back chunks of 250 members that can fit in an RPC
    while (start && res.groupinfo->owners.size () > 0) {
      res.groupinfo->owners.pop_front ();
      start--;
    }
    while (start && res.groupinfo->members.size () > 0) {
      res.groupinfo->members.pop_front ();
      start--;
    }
    while (res.groupinfo->members.size () + res.groupinfo->owners.size ()
	   > 250) {
      if (res.groupinfo->members.size () > 0)
	res.groupinfo->members.pop_back ();
      else
	res.groupinfo->owners.pop_back ();
      more = true;
    }
    if (more)
      res.groupinfo->members.push_back ("...");

    if (dbp->hide_users && !is_authenticated (sbp)) {
      obfuscate_group (res.groupinfo->members);
      obfuscate_group (res.groupinfo->owners);
    }
  }
  sbp->replyref (res);
}

void
authclnt::query_expandedgroup (svccb *sbp)
{
  sfsauth2_query_arg *arg = sbp->Xtmpl getarg<sfsauth2_query_arg> ();
  sfsauth2_query_res res;
  dbfile *dbp;
  sfsauth_dbkey key = arg->key;
  unsigned int start = 0;
  bool more = false;

  // This query is disabled if the authentication cache is off because the
  // results could be misleading.  Specifically, the "members" list will
  // always be empty because it is generated from the (non-existent) cache.
  if (!global_dbcache) {
    res.set_type (SFSAUTH_ERROR);
    *res.errmsg = strbuf ("Expanded queries are disabled because this "
                          "server's authentication cache is disabled.");
    sbp->replyref (res);
    return;
  }

  if (arg->key.type == SFSAUTH_DBKEY_NAMEVERS) {
    key.set_type (SFSAUTH_DBKEY_NAME);
    *key.name = arg->key.namevers->name;
    start = arg->key.namevers->vers;
  }

  if (get_group_cursor (&dbp, NULL, &res, key)) {
    sfs_groupmembers list;
    sfs_groupmembers closure;

    list.push_back (add_member_type (res.groupinfo->name, "g"));
    transitive_closure (list, closure);
    res.groupinfo->members = closure;

    closure.clear ();
    transitive_closure (res.groupinfo->owners, closure);
    res.groupinfo->owners = closure;

    // Send back chunks of 250 members that can fit in an RPC
    while (start && res.groupinfo->owners.size () > 0) {
      res.groupinfo->owners.pop_front ();
      start--;
    }
    while (start && res.groupinfo->members.size () > 0) {
      res.groupinfo->members.pop_front ();
      start--;
    }
    while (res.groupinfo->members.size () + res.groupinfo->owners.size () > 250) {
      if (res.groupinfo->members.size () > 0)
	res.groupinfo->members.pop_back ();
      else
	res.groupinfo->owners.pop_back ();
      more = true;
    }
    if (more)
      res.groupinfo->members.push_back ("...");

    if (dbp->hide_users && !is_authenticated (sbp)) {
      obfuscate_group (res.groupinfo->members);
      obfuscate_group (res.groupinfo->owners);
    }
  }
  sbp->replyref (res);
}

void
authclnt::query_changelog (svccb *sbp)
{
  sfsauth2_query_arg *arg = sbp->Xtmpl getarg<sfsauth2_query_arg> ();
  sfsauth2_query_res res;
  dbfile *dbp;

  if (arg->key.type != SFSAUTH_DBKEY_NAMEVERS || arg->type != SFSAUTH_LOGENTRY) {
    res.set_type (SFSAUTH_ERROR);
    *res.errmsg = strbuf ("unsupported DB (%d) or key type (%d)",
	                  arg->type, arg->key.type);
    sbp->replyref (res);
    return;
  }

  sfs_namevers *nv = arg->key.namevers;
  sfsauth_dbkey key (SFSAUTH_DBKEY_NAME);
  *key.name = nv->name;

  if (get_group_cursor (&dbp, NULL, &res, key)) {
    sfs_groupmembers updates;
    unsigned int latest;

    if (nv->vers
	&& (latest = read_group_changelog (nv->name, nv->vers, updates))) {
      bool more = res.groupinfo->vers > latest;
      // sfs_time refresh = res.groupinfo->refresh;
      // sfs_time timeout = res.groupinfo->timeout;
      sfs_time refresh = extract_u_int_default (refresh_eq,
						res.groupinfo->properties,
						dbp->default_refresh);
      sfs_time timeout = extract_u_int_default (timeout_eq,
						res.groupinfo->properties,
						dbp->default_timeout);

      if (dbp->hide_users && !is_authenticated (sbp))
	obfuscate_group (updates, true);

      res.set_type (SFSAUTH_LOGENTRY);
      res.logentry->vers = latest;
      res.logentry->members = updates;
      res.logentry->more = more;
      res.logentry->refresh = refresh;
      res.logentry->timeout = timeout;
      res.logentry->audit = strbuf () << "Changes for group `" << nv->name
	                    << "' from version " << nv->vers << " to " << latest;
    }
    else {
      // if read_group_changelog fails, we return the full group record
      query_group (sbp);
      return;
    }
  }
  sbp->replyref (res);
}

void
authclnt::sfsauth_query (svccb *sbp)
{
  sfsauth2_query_arg *arg = sbp->Xtmpl getarg<sfsauth2_query_arg> ();
  switch (arg->type) {
  case SFSAUTH_USER:
    query_user (sbp);
    break;
  case SFSAUTH_GROUP:
    query_group (sbp);
    break;
  case SFSAUTH_CERTINFO:
    query_certinfo (sbp);
    break;
  case SFSAUTH_SRPPARMS:
    query_srpparms (sbp);
    break;
  case SFSAUTH_EXPANDEDGROUP:
    query_expandedgroup (sbp);
    break;
  case SFSAUTH_LOGENTRY:
    query_changelog (sbp);
    break;
  default:
    sfsauth2_query_res res;
    res.set_type (SFSAUTH_ERROR);
    *res.errmsg = strbuf ("unsupported query type %d", arg->type);
    sbp->replyref (res);
    break;
  }
}
