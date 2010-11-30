// -*-c++-*-
/* $Id: async.h,v 1.32 2004/07/19 02:51:05 dm Exp $ */

/*
 *
 * Copyright (C) 1998 David Mazieres (dm@uun.org)
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

#ifndef _ASYNC_ASYNC_H_
#define _ASYNC_ASYNC_H_ 1

#include "amisc.h"
#include "init.h"

/* core.C */
struct timecb_t;
struct lazycb_t;
extern timespec tsnow;
extern const time_t &timenow;
INIT (async_init);
void amain () __attribute__ ((noreturn));
void acheck ();
void chldcb (pid_t, cbi::ptr);
cbv::ptr sigcb (int, cbv::ptr, int = 0);
void fdcb (int, selop, cbv::ptr);
timecb_t *timecb (const timespec &ts, cbv cb);
timecb_t *delaycb (time_t sec, u_int32_t nsec, cbv cb);
void timecb_remove (timecb_t *);
lazycb_t *lazycb (time_t min_interval, cbv cb);
void lazycb_remove (lazycb_t *lazy);

inline timecb_t *
timecb (time_t tm, cbv cb)
{
  timespec ts = { tm, 0 };
  return timecb (ts, cb);
}
inline timecb_t *
delaycb (time_t tm, cbv cb)
{
  return delaycb (tm, 0, cb);
}

/* aerr.C */
void err_init ();
void err_flush ();

/* tcpconnect.C */
struct srvlist;
struct tcpconnect_t;
tcpconnect_t *tcpconnect (in_addr addr, u_int16_t port, cbi cb);
tcpconnect_t *tcpconnect (str hostname, u_int16_t port, cbi cb,
			  bool dnssearch = true, str *namep = NULL);
tcpconnect_t *tcpconnect_srv (str hostname, str service, u_int16_t defport,
			      cbi cb, bool dnssearch = true,
			      ptr<srvlist> *srvlp = NULL, str *np = NULL);
tcpconnect_t *tcpconnect_srv_retry (ref<srvlist> srvl, cbi cb, str *np = NULL);
void tcpconnect_cancel (tcpconnect_t *tc);

/* ident.C */
void identptr (int fd, callback<void, str, ptr<hostent>, int>::ref);
void ident (int fd, callback<void, str, int>::ref);

/* pipe2str.C */
void pipe2str (int fd, cbs cb, int *fdp = NULL, strbuf *sb = NULL);
void chldrun (cbi chld, cbs cb);

/* ifchg.C */
struct ifchgcb_t;
extern vec<in_addr> ifchg_addrs;
extern u_int64_t ifchg_count;
ifchgcb_t *ifchgcb (cbv);
void ifchgcb_remove (ifchgcb_t *chg);

#endif /* !_ASYNC_ASYNC_H_ */
