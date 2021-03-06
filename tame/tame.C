
/* $Id: tame.C,v 1.3 2006/05/31 16:30:36 max Exp $ */

#include "tame.h"
#include "rxx.h"

parse_state_t state;

static void
usage ()
{
  warnx << "usage: " << progname << " [-Lchnv] "
	<< "[-o <outfile>] [<infile>]\n"
	<< "\n"
	<< "  Flags:\n"
	<< "    -n  turn on newlines in autogenerated code\n"
	<< "    -L  disable line number translation\n"
	<< "    -h  show this screen\n"
	<< "    -v  show version number and exit\n"
	<< "\n"
	<< "  Options:\n"
	<< "    -o  specify output file\n"
	<< "    -c  compile mode; infer output file name from input file "
	<< "name\n"
	<< "\n"
	<< "  If no input or output files are specified, then standard in\n"
	<< "  and out are assumed, respectively.\n"
	<< "\n"
    << "  Environment Variables:\n"
	<< "    TAME_NO_LINE_NUMBERS  equivalent to -L\n"
	<< "    TAME_ADD_NEWLINES     equivalent to -n\n"
	<< "    TAME_DEBUG_SOURCE     equivalent to -Ln\n"
	  ;
    
  exit (1);
}

static str
ifn2ofn (const str &s)
{
  static rxx x ("(.*)\\.T");
  if (!x.match (s))
    return NULL;
  strbuf b (x[1]);
  b << ".C";
  return b;
}


int
main (int argc, char *argv[])
{
  setprogname (argv[0]);
  FILE *ifh = NULL;
  int ch;
  str outfile;
  bool debug = false;
  bool no_line_numbers = false;
  bool horiz_mode = true;
  str ifn;
  outputter_t *o;
  
  make_sync (0);
  make_sync (1);
  make_sync (2);

  while ((ch = getopt (argc, argv, "hnLvdo:c:")) != -1)
    switch (ch) {
    case 'h':
      usage ();
      break;
    case 'n':
      horiz_mode = false;
      break;
    case 'c':
      ifn = optarg;
      if (!(outfile = ifn2ofn (ifn))) {
	warn << "-c expects an input file with the .T suffix\n";
	usage ();
      }
      break;
    case 'L':
      no_line_numbers = true;
      break;
    case 'd':
      debug = true;
      break;
    case 'o':
      outfile = optarg;
      break;
    case 'v':
      warnx << "tame\n"
	    << "sfslite version " << VERSION << "\n"
	    << "compiled " __DATE__ " " __TIME__ "\n" ;
      exit (0);
      break;
    default:
      usage ();
      break;
    }

  if (getenv ("TAME_DEBUG_SOURCE")) {
    no_line_numbers = true;
	horiz_mode = false;
  }

  if (getenv ("TAME_NO_LINE_NUMBERS"))
    no_line_numbers = true;

  if (getenv ("TAME_ADD_NEWLINES"))
    horiz_mode = false;

  argc -= optind;
  argv += optind;

  if (argc == 1) {
    if (ifn) {
      warn << "input filename double-specified!\n";
      usage ();
    }
    ifn = argv[0];
  }

  if (ifn && ifn != "-") {
    if (!(ifh = fopen (ifn.cstr (), "r"))) {
      warn << "cannot open file: " << ifn << "\n";
      usage ();
    }

    // the filename variable is local to scan.ll, which will need
    // it to output message messages. It defaults to '(stdin)'
    filename = ifn;
    
    yyin = ifh;
  }

  state.set_infile_name (ifn);
  bool fl = (ifn && ifn != "-" && !no_line_numbers);
  if (horiz_mode) {
    o = New outputter_H_t (ifn, outfile, fl);
  } else {
    o = New outputter_t (ifn, outfile, fl);
  }
  if (!o->init ())
    exit (1);

  // only on if YYDEBUG is on :(
  //yydebug = 1;

  yyparse ();

  if (ifh) {
    fclose (ifh);
  }

  state.output (o);

  // calls close on the outputter fd
  delete o;
}
