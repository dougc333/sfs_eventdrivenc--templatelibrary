
# 1 "test_tame.T"
// -*-c++-*-
/* $Id: test_tame.T,v 1.1 2006/05/31 16:30:37 max Exp $ */

#include "tame.h"
#include "arpc.h"
#include "parseopt.h"

/**
 * Get fastest Web page, where 'fast' is defined by how fast the DNS
 * lookup and TCP session establishment are.  Once connected, request
 * for "/" and dump the response to standard output.
 *
 * @param hosts the hosts to try
 * @param port the port to try on
 * @param done the callback to call when done
 */
# 17 "test_tame.T"
static void  get_web_page( str host,  int port,  strbuf *resp,  cbb done, ptr<closure_t> __cls_g = NULL); 
# 17 "test_tame.T"
static void __nonblock_cb_0_0 (ptr<closure_t> hold, ptr<joiner_t<> > j, 		value_set_t<> w) {   j->join (w); } 
# 17 "test_tame.T"
class get_web_page__closure_t : public closure_t { public:   get_web_page__closure_t ( str host,  int port,  strbuf *resp,  cbb done) : closure_t ("test_tame.T", "get_web_page"),  _stack (host, port, resp, done), _args (host, port, resp, done) {}   void reenter ()   {     get_web_page (_args.host, _args.port, _args.resp, _args.done, mkref (this));   }   struct stack_t {     stack_t ( str host,  int port,  strbuf *resp,  cbb done) {}      int fd;      int rc;      strbuf req;      coordgroup_t<> read_cg;      coordgroup_t<> write_cg;   };   struct args_t {     args_t ( str host,  int port,  strbuf *resp,  cbb done) : host (host), port (port), resp (resp), done (done) {}      str host;      int port;      strbuf *resp;      cbb done;   };   stack_t _stack;   args_t _args;   bool is_onstack (const void *p) const   {     return (static_cast<const void *> (&_stack) <= p &&             static_cast<const void *> (&_stack + 1) > p);   } }; 
# 17 "test_tame.T"
void 
get_web_page( str __tame_host,  int __tame_port,  strbuf *__tame_resp,  cbb __tame_done, ptr<closure_t> __cls_g)
{
  
# 20 "test_tame.T"
  get_web_page__closure_t *__cls;   ptr<get_web_page__closure_t > __cls_r;   if (!__cls_g) {     if (tame_check_leaks ()) start_join_group_collection ();     __cls_r = New refcounted<get_web_page__closure_t> (__tame_host, __tame_port, __tame_resp, __tame_done);     if (tame_check_leaks ()) __cls_r->collect_join_groups ();     __cls = __cls_r;     __cls_g = __cls_r;   } else {     __cls =     reinterpret_cast<get_web_page__closure_t *> (static_cast<closure_t *> (__cls_g));     __cls_r = mkref (__cls);   }    int &fd = __cls->_stack.fd;    int &rc = __cls->_stack.rc;    strbuf &req = __cls->_stack.req;    coordgroup_t<> &read_cg = __cls->_stack.read_cg;    coordgroup_t<> &write_cg = __cls->_stack.write_cg;    str &host = __cls->_args.host;    int &port = __cls->_args.port;    strbuf *&resp = __cls->_args.resp;    cbb &done = __cls->_args.done;    use_reference (host);     use_reference (port);     use_reference (resp);     use_reference (done);    switch (__cls->jumpto ()) {   case 0: break;   case 1:     goto get_web_page__label_1;     break;   case 2:     goto get_web_page__label_2;     break;   case 3:     goto get_web_page__label_3;     break;   default:     panic ("unexpected case.\n");     break;   }
# 24 "test_tame.T"


  //
  // get the fastest connection, and dump the result into 'fd'
  //
  
# 29 "test_tame.T"
  do {     __cls->init_block (1, 29);     __cls->set_jumpto (1); 
# 29 "test_tame.T"
 tcpconnect (host, port, wrap (__cls->make_wrapper (1, 29), &closure_wrapper_t::block_cb1<TTT(fd)>, refset_t<TTT(fd)> (fd))); 
# 29 "test_tame.T"
    if (!__cls->block_dec_count (29))       return;   } while (0);  get_web_page__label_1:     ;
# 29 "test_tame.T"

  if (fd < 0) {
    warn << host << ":" << port << ": connection failed\n";
    (*done) (false);
    
# 33 "test_tame.T"
  __cls->end_of_scope_checks (33);
# 33 "test_tame.T"
return ;
  }


  //
  // A dirt simple HTTP 1.0 request
  //
  req << "GET / HTTP/1.0\n\n";
  
  //
  // Call 'fdcb' to select on a file descriptor.  In this case we're
  // selecting on the TCP connection to the 'fastest' Web server, and
  // selecting for 'write'.  Once the socket is writeable, we'll be
  // callback.  Note that '@()' generates a callback of type 
  // callback<void> -- i.e., a callback that doesn't 'return' any
  // parameters.
  //
  // Also note that 'fdcb' is a bit different from the asychronous
  // functions we've seen so far, in that it can call us back multiple
  // times.  That is, fdcb will call us back every time the socket
  // 'fd' is available for writing.
  //
  fdcb (fd, selwrite, ((write_cg).launch_one (__cls_g), wrap (__nonblock_cb_0_0, __cls_g, (write_cg).make_joiner ("test_tame.T:55: in function get_web_page"), value_set_t<> ())));
  
  while (true) {
    
# 58 "test_tame.T"
get_web_page__label_2: do {    if (!(write_cg).next_var ()) {     __cls->set_jumpto (2);       (write_cg).set_join_cb (wrap (__cls_r, &get_web_page__closure_t::reenter));       return;   } } while (0);
# 58 "test_tame.T"


    //
    // Use this syntax to output the results of the string buffer
    // 'req' to the socket 'fd'. At this point, fdcb has returned,
    // signalling that the socket 'fd' is writable.  If for some
    // reason we were lied to, write() will return <0 inside of
    // suio::output() below, but with errno set to EAGAIN; then 
    // suio::output() will return 0, and we'll try the write again the 
    // next time through the loop.  A return from suio::output() that
    // is negative signals a non-retryable error, and we'll bail out.
    //
    if (req.tosuio ()->output (fd) < 0) {
      warn << "write failed...\n";
      fdcb (fd, selwrite, NULL);
      close (fd);
      (*done) (false);
      
# 75 "test_tame.T"
  __cls->end_of_scope_checks (75);
# 75 "test_tame.T"
return ;
    }
    
    //
    // suio::resid() returns the # of bytes left to write.  If there
    // are any bytes left to write, we'll need to be called back from
    // fdcb again, and we'll therefore need to wait again.  Use the
    // coordgroup_t::add_var call to schedule an additional coordination
    // variable for this coordination group.
    //
    if (req.tosuio ()->resid ()) {
      write_cg.add_var ();
    }
    
    // 
    // otherwise, no more writing left to do, and we are no longer
    // interested in writing to 'fd'
    //
    else {
      fdcb (fd, selwrite, NULL);
      break;
    }
  }

  // 
  // As before, but now we need to schedule a callback for reading
  //
  fdcb (fd, selread, ((read_cg).launch_one (__cls_g), wrap (__nonblock_cb_0_0, __cls_g, (read_cg).make_joiner ("test_tame.T:102: in function get_web_page"), value_set_t<> ())));
  
  //
  // The details of the reading loop are almost identical to the
  // writing loop above.
  //
  while (true) {
    
# 109 "test_tame.T"
get_web_page__label_3: do {    if (!(read_cg).next_var ()) {     __cls->set_jumpto (3);       (read_cg).set_join_cb (wrap (__cls_r, &get_web_page__closure_t::reenter));       return;   } } while (0);
# 109 "test_tame.T"

    if ((rc = resp->tosuio ()->input (fd)) < 0 && errno != EAGAIN) {
      warn << "read failed...\n";
      fdcb (fd, selread, NULL);
      close (fd);
      (*done) (false);
      
# 115 "test_tame.T"
  __cls->end_of_scope_checks (115);
# 115 "test_tame.T"
return ;
    }
    if (rc != 0) {
      read_cg.add_var ();
    } else {
      fdcb (fd, selread, NULL);
      break;
    }
  }

  close (fd);
  
  // 
  // success!
  //
  (*done) (true);
# 131 "test_tame.T"
  __cls->end_of_scope_checks (131);   return;
# 131 "test_tame.T"
}

/**
 * Given a vector of N hosts, connect to all of them on the given port.
 * When the first connection is established, return controle via 'done',
 * and close the remaining stragglers.
 *
 * @param hosts the hosts to try
 * @param port the port to try on
 * @param done the callback to call when the first has returned.
 */
# 142 "test_tame.T"
static void  get_fastest_web_page( vec< str > hosts,  int port,  cbb done, ptr<closure_t> __cls_g = NULL); 
# 142 "test_tame.T"
template<class P1, class W1, class W2> static void __nonblock_cb_2_1 (ptr<closure_t> hold, ptr<joiner_t<W1, W2> > j, 		refset_t<P1> rs, 		value_set_t<W1, W2> w, 		P1 v1) {   rs.assign (v1);   j->join (w); } 
# 142 "test_tame.T"
class get_fastest_web_page__closure_t : public closure_t { public:   get_fastest_web_page__closure_t ( vec< str > hosts,  int port,  cbb done) : closure_t ("test_tame.T", "get_fastest_web_page"),  _stack (hosts, port, done), _args (hosts, port, done) {}   void reenter ()   {     get_fastest_web_page (_args.hosts, _args.port, _args.done, mkref (this));   }   struct stack_t {     stack_t ( vec< str > hosts,  int port,  cbb done) : got_one (false)  {}      u_int i;      coordgroup_t< u_int  ,  ptr< bool > > CG;      bool got_one;      vec< strbuf > responses;      ptr< bool > res;   };   struct args_t {     args_t ( vec< str > hosts,  int port,  cbb done) : hosts (hosts), port (port), done (done) {}      vec< str > hosts;      int port;      cbb done;   };   stack_t _stack;   args_t _args;   bool is_onstack (const void *p) const   {     return (static_cast<const void *> (&_stack) <= p &&             static_cast<const void *> (&_stack + 1) > p);   } }; 
# 142 "test_tame.T"
void 
get_fastest_web_page( vec< str > __tame_hosts,  int __tame_port,  cbb __tame_done, ptr<closure_t> __cls_g)
{
  
# 145 "test_tame.T"
  get_fastest_web_page__closure_t *__cls;   ptr<get_fastest_web_page__closure_t > __cls_r;   if (!__cls_g) {     if (tame_check_leaks ()) start_join_group_collection ();     __cls_r = New refcounted<get_fastest_web_page__closure_t> (__tame_hosts, __tame_port, __tame_done);     if (tame_check_leaks ()) __cls_r->collect_join_groups ();     __cls = __cls_r;     __cls_g = __cls_r;   } else {     __cls =     reinterpret_cast<get_fastest_web_page__closure_t *> (static_cast<closure_t *> (__cls_g));     __cls_r = mkref (__cls);   }    u_int &i = __cls->_stack.i;    coordgroup_t< u_int  ,  ptr< bool > > &CG = __cls->_stack.CG;    bool &got_one = __cls->_stack.got_one;    vec< strbuf > &responses = __cls->_stack.responses;    ptr< bool > &res = __cls->_stack.res;    vec< str > &hosts = __cls->_args.hosts;    int &port = __cls->_args.port;    cbb &done = __cls->_args.done;    use_reference (hosts);     use_reference (port);     use_reference (done);    switch (__cls->jumpto ()) {   case 0: break;   case 1:     goto get_fastest_web_page__label_1;     break;   default:     panic ("unexpected case.\n");     break;   }
# 151 "test_tame.T"

  
  responses.setsize (hosts.size ());
  
  for (i = 0; i < hosts.size (); i++) {
    res = New refcounted<bool> ();
    get_web_page (hosts[i], port, &responses[i], ((CG).launch_one (__cls_g), wrap (__nonblock_cb_2_1 <typeof (*res), typeof (i), typeof (res)>, __cls_g, (CG).make_joiner ("test_tame.T:157: in function get_fastest_web_page"), refset_t<TTT (*res)> (*res), value_set_t<typeof (i), typeof (res)> (i, res))));
  }
  
  while (CG.need_wait ()) {
    
# 161 "test_tame.T"
get_fastest_web_page__label_1: do {    if (!(CG).next_var (i, res)) {     __cls->set_jumpto (1);       (CG).set_join_cb (wrap (__cls_r, &get_fastest_web_page__closure_t::reenter));       return;   } } while (0);
# 161 "test_tame.T"

    warn << hosts[i]  << ":" << port << ": ";
    if (*res) {
      warnx << "connection succeeded";
      if (!got_one) {
	(*done) (true);
	responses[i].tosuio ()->output (1);
	got_one = true;
      } else {
	warnx << "... but too late!";
      }
      warnx << "\n";
    } else {
      warnx << "connection failed\n";
    }
  }
  if (!got_one)
    (*done) (false);
# 179 "test_tame.T"
  __cls->end_of_scope_checks (179);   return;
# 179 "test_tame.T"
}

static void finish (bool rc)
{
  delaycb (10, 0, wrap (exit, rc ? 0 : -1));
}

# 186 "test_tame.T"
static void  simple_test(ptr<closure_t> __cls_g = NULL); 
# 186 "test_tame.T"
class simple_test__closure_t : public closure_t { public:   simple_test__closure_t () : closure_t ("test_tame.T", "simple_test"),  _stack (), _args () {}   void reenter ()   {     simple_test (mkref (this));   }   struct stack_t {     stack_t () {}   };   struct args_t {     args_t () {}   };   stack_t _stack;   args_t _args;   bool is_onstack (const void *p) const   {     return (static_cast<const void *> (&_stack) <= p &&             static_cast<const void *> (&_stack + 1) > p);   } }; 
# 186 "test_tame.T"
void 
simple_test(ptr<closure_t> __cls_g)
{
# 188 "test_tame.T"
  simple_test__closure_t *__cls;   ptr<simple_test__closure_t > __cls_r;   if (!__cls_g) {     if (tame_check_leaks ()) start_join_group_collection ();     __cls_r = New refcounted<simple_test__closure_t> ();     if (tame_check_leaks ()) __cls_r->collect_join_groups ();     __cls = __cls_r;     __cls_g = __cls_r;   } else {     __cls =     reinterpret_cast<simple_test__closure_t *> (static_cast<closure_t *> (__cls_g));     __cls_r = mkref (__cls);   }   switch (__cls->jumpto ()) {   case 0: break;   case 1:     goto simple_test__label_1;     break;   default:     panic ("unexpected case.\n");     break;   }
# 188 "test_tame.T"

  
# 189 "test_tame.T"
  do {     __cls->init_block (1, 189);     __cls->set_jumpto (1); 
# 189 "test_tame.T"
 delaycb (1, 0, wrap (__cls->make_wrapper (1, 189), &closure_wrapper_t::block_cb0)); 
# 189 "test_tame.T"
    if (!__cls->block_dec_count (189))       return;   } while (0);  simple_test__label_1:     ;
# 189 "test_tame.T"

  exit (0);
# 191 "test_tame.T"
  __cls->end_of_scope_checks (191);   return;
# 191 "test_tame.T"
}

int
main (int argc, char *argv[])
{
  vec<str> hosts;
  int port;

  // for 'make check' just exit cleanly
  if (argc == 1) {
    simple_test ();
  } else {
    if (argc < 3 || !convertint (argv[1], &port))
      fatal << "usage: ex2 <port> <host1> <host2> ...\n";

    for (int i = 2; i < argc; i++) 
      hosts.push_back (argv[i]);
    get_fastest_web_page (hosts, port, wrap (finish));
  }
  amain ();
}
