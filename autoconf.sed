# $Id: autoconf.sed,v 1.21 2006/04/17 23:09:42 dm Exp $

1i\
#ifndef _ARPCCONF_H_\
#define _ARPCCONF_H_ 1
$a\
\
#endif /* !_ARPCCONF_H_ */

/^#define inline/b skip
/^#define xdr_ops_t/b skip
/^#define [a-z]/b pdefine

/ DMALLOC/b pdefine
/ HAVE_ARPA_NAMESER_COMPAT_H/b pdefine
/ HAVE_CLOCK_GETTIME/b pdefine
/ HAVE_DEV_XFS/b pdefine
/ HAVE_PEEREID/b pdefine
/ HAVE_GETRUSAGE/b pdefine
/ HAVE_GMP_CXX_OPS/b pdefine
/ HAVE_INET_ATON/b pdefine
/ HAVE_INT32_T/b pdefine
/ HAVE_INT64_T/b pdefine
/ HAVE_MEMCPY/b pdefine
/ HAVE_MEMORY_H/b pdefine
/ HAVE_MODE_T/b pdefine
/ HAVE_OFF_T_64/b pdefine
/ HAVE_PREAD/b pdefine
/ HAVE_PREAD_DECL/b pdefine
/ HAVE_PWRITE/b pdefine
/ HAVE_PWRITE_DECL/b pdefine
/ HAVE_RLIM_T/b pdefine
/ HAVE_SOCKLEN_T/b pdefine
/ HAVE_SS_LEN_UNDERSCORES/b pdefine
/ HAVE_SSIZE_T/b pdefine
/ HAVE_ST_MTIM/b pdefine
/ HAVE_ST_MTIMESPEC/b pdefine
/ HAVE_STRCASECMP/b pdefine
/ HAVE_STRCHR/b pdefine
/ HAVE_STRERROR/b pdefine
/ HAVE_STRUCT_SOCKADDR_STORAGE/b pdefine
/ HAVE_SYS_FILE_H/b pdefine
/ HAVE_SYS_FILIO_H/b pdefine
/ HAVE_SYS_RUSAGE_H/b pdefine
/ HAVE_SYS_SOCKIO_H/b pdefine
/ HAVE_SYS_TIME_H/b pdefine
/ HAVE_SYS_WAIT_H/b pdefine
/ HAVE_TIMES/b pdefine
/ HAVE_TIMESPEC/b pdefine
/ HAVE_U_CHAR/b pdefine
/ HAVE_U_INT/b pdefine
/ HAVE_U_INT8_T/b pdefine
/ HAVE_U_INT16_T/b pdefine
/ HAVE_U_INT32_T/b pdefine
/ HAVE_U_INT64_T/b pdefine
/ HAVE_U_LONG/b pdefine
/ NEED_FCHDIR_DECL/b pdefine
/ NEED_GETRUSAGE_DECL/b pdefine
/ NEED_MKSTEMP_DECL/b pdefine
/ NEED_RES_INIT_DECL/b pdefine
/ NEED_RES_MKQUERY_DECL/b pdefine
/ NEED_XDR_CALLMSG_DECL/b pdefine
/ SETGROUPS_NEEDS_GRP_H/b pdefine
/ SIZEOF_LONG/b pdefine
/ SIZEOF_LONG_LONG/b pdefine
/ STDC_HEADERS/b pdefine
/ TIME_WITH_SYS_TIME/b pdefine
/ U_INT64_T_IS_LONG_LONG/b pdefine
/ WORDS_BIGENDIAN/b pdefine
/ PYMALLOC/b pdefine

:skip
d
n

:pdefine
s/#define \([^ ]*\).*/\
#ifndef \1\
&\
#endif/p
d
