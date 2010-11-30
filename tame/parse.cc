/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     T_ID = 258,
     T_NUM = 259,
     T_PASSTHROUGH = 260,
     T_CONST = 261,
     T_STRUCT = 262,
     T_TYPENAME = 263,
     T_VOID = 264,
     T_CHAR = 265,
     T_SHORT = 266,
     T_INT = 267,
     T_LONG = 268,
     T_LONG_LONG = 269,
     T_FLOAT = 270,
     T_DOUBLE = 271,
     T_SIGNED = 272,
     T_UNSIGNED = 273,
     T_STATIC = 274,
     T_TEMPLATE = 275,
     T_2COLON = 276,
     T_RETURN = 277,
     T_TAME = 278,
     T_TAMED = 279,
     T_VARS = 280,
     T_BLOCK = 281,
     T_NONBLOCK = 282,
     T_JOIN = 283,
     T_WAIT = 284,
     T_DEFAULT_RETURN = 285,
     T_2DOLLAR = 286
   };
#endif
/* Tokens.  */
#define T_ID 258
#define T_NUM 259
#define T_PASSTHROUGH 260
#define T_CONST 261
#define T_STRUCT 262
#define T_TYPENAME 263
#define T_VOID 264
#define T_CHAR 265
#define T_SHORT 266
#define T_INT 267
#define T_LONG 268
#define T_LONG_LONG 269
#define T_FLOAT 270
#define T_DOUBLE 271
#define T_SIGNED 272
#define T_UNSIGNED 273
#define T_STATIC 274
#define T_TEMPLATE 275
#define T_2COLON 276
#define T_RETURN 277
#define T_TAME 278
#define T_TAMED 279
#define T_VARS 280
#define T_BLOCK 281
#define T_NONBLOCK 282
#define T_JOIN 283
#define T_WAIT 284
#define T_DEFAULT_RETURN 285
#define T_2DOLLAR 286




/* Copy the first part of user declarations.  */
#line 25 "./parse.yy"

#include "tame.h"
#include "parseopt.h"
#define YYSTYPE YYSTYPE
int vars_lineno;


/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 175 "y.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
struct yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (struct yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   167

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  44
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  68
/* YYNRULES -- Number of rules.  */
#define YYNRULES  118
/* YYNRULES -- Number of states.  */
#define YYNSTATES  178

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   286

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      32,    33,    43,     2,    39,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    38,
      36,     2,    37,     2,    42,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    40,     2,    41,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    34,     2,    35,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     9,    10,    13,    15,    18,    23,
      26,    27,    33,    35,    37,    38,    44,    46,    47,    52,
      53,    55,    57,    61,    63,    65,    67,    69,    71,    73,
      75,    77,    79,    84,    85,    91,    95,    96,   102,   103,
     105,   108,   112,   115,   116,   118,   120,   124,   125,   134,
     140,   141,   150,   152,   156,   160,   161,   163,   169,   171,
     172,   174,   176,   179,   180,   182,   184,   186,   190,   193,
     194,   199,   200,   202,   204,   208,   211,   214,   217,   218,
     222,   224,   226,   231,   234,   236,   238,   240,   241,   244,
     246,   248,   250,   252,   254,   256,   258,   260,   262,   264,
     266,   268,   271,   272,   274,   276,   280,   283,   284,   286,
     290,   291,   293,   295,   299,   302,   303,   305,   307
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      45,     0,    -1,    46,    -1,    45,    49,    46,    -1,    -1,
      46,     5,    -1,     5,    -1,    47,     5,    -1,    23,    32,
      54,    33,    -1,    24,    54,    -1,    -1,    48,    34,    50,
      56,    35,    -1,    52,    -1,    19,    -1,    -1,    20,    36,
      46,    37,    53,    -1,    19,    -1,    -1,    51,    96,    91,
      55,    -1,    -1,     6,    -1,    46,    -1,    56,    57,    46,
      -1,    60,    -1,    63,    -1,    70,    -1,    73,    -1,    72,
      -1,    62,    -1,    59,    -1,    58,    -1,    78,    -1,    30,
      34,    46,    35,    -1,    -1,    25,    61,    34,    80,    35,
      -1,    22,    46,    38,    -1,    -1,    26,    34,    64,    75,
      35,    -1,    -1,    66,    -1,    39,    79,    -1,    66,    39,
      79,    -1,    46,    65,    -1,    -1,    69,    -1,    47,    -1,
      69,    39,    47,    -1,    -1,    27,    32,    69,    33,    34,
      71,    75,    35,    -1,    29,    32,    67,    33,    38,    -1,
      -1,    28,    32,    67,    33,    34,    74,    56,    35,    -1,
      46,    -1,    75,    78,    46,    -1,    40,    69,    41,    -1,
      -1,    76,    -1,    42,    77,    32,    68,    33,    -1,     3,
      -1,    -1,    81,    -1,    86,    -1,    81,    86,    -1,    -1,
      83,    -1,    84,    -1,    85,    -1,    84,    39,    85,    -1,
      96,    91,    -1,    -1,    96,    87,    88,    38,    -1,    -1,
      89,    -1,    90,    -1,    89,    39,    90,    -1,    92,    93,
      -1,   110,    95,    -1,   110,    94,    -1,    -1,    32,    46,
      33,    -1,    79,    -1,   103,    -1,   103,    32,    82,    33,
      -1,    98,    99,    -1,   100,    -1,    17,    -1,    18,    -1,
      -1,    98,    97,    -1,     9,    -1,    10,    -1,    11,    -1,
      12,    -1,    13,    -1,    14,    -1,    15,    -1,    16,    -1,
     103,    -1,     6,    -1,     7,    -1,   100,    -1,   101,   100,
      -1,    -1,   101,    -1,   104,    -1,   103,    21,   104,    -1,
      79,   105,    -1,    -1,   106,    -1,    36,   107,    37,    -1,
      -1,   108,    -1,   109,    -1,   108,    39,   109,    -1,    96,
     110,    -1,    -1,   111,    -1,    43,    -1,    43,   102,   111,
      -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   100,   100,   101,   104,   105,   113,   114,   122,   123,
     127,   126,   145,   146,   147,   150,   156,   157,   161,   168,
     169,   172,   176,   183,   184,   185,   186,   187,   188,   189,
     190,   193,   202,   215,   214,   235,   247,   246,   263,   264,
     267,   273,   280,   292,   293,   296,   302,   310,   309,   326,
     336,   335,   352,   356,   363,   369,   370,   373,   398,   411,
     412,   415,   416,   419,   420,   425,   428,   432,   446,   456,
     455,   462,   463,   466,   467,   472,   494,   502,   510,   511,
     517,   528,   532,   546,   555,   556,   557,   560,   561,   572,
     573,   574,   575,   576,   577,   578,   579,   580,   586,   587,
     590,   591,   597,   598,   605,   606,   612,   618,   619,   622,
     628,   629,   632,   633,   639,   645,   646,   649,   650
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "T_ID", "T_NUM", "T_PASSTHROUGH",
  "T_CONST", "T_STRUCT", "T_TYPENAME", "T_VOID", "T_CHAR", "T_SHORT",
  "T_INT", "T_LONG", "T_LONG_LONG", "T_FLOAT", "T_DOUBLE", "T_SIGNED",
  "T_UNSIGNED", "T_STATIC", "T_TEMPLATE", "T_2COLON", "T_RETURN", "T_TAME",
  "T_TAMED", "T_VARS", "T_BLOCK", "T_NONBLOCK", "T_JOIN", "T_WAIT",
  "T_DEFAULT_RETURN", "T_2DOLLAR", "'('", "')'", "'{'", "'}'", "'<'",
  "'>'", "';'", "','", "'['", "']'", "'@'", "'*'", "$accept", "file",
  "passthrough", "expr", "tame_decl", "fn", "@1", "fn_specifiers",
  "template_decl", "static_opt", "fn_declaration", "const_opt",
  "fn_statements", "fn_tame", "floating_callback", "default_return",
  "vars", "@2", "return_statement", "block", "@3", "id_list_opt",
  "id_list", "join_list", "expr_list_opt", "expr_list", "nonblock", "@4",
  "wait", "join", "@5", "callbacks_and_passthrough", "bracket_list",
  "bracket_list_opt", "callback", "identifier", "declaration_list_opt",
  "declaration_list", "parameter_type_list_opt", "parameter_type_list",
  "parameter_list", "parameter_declaration", "declaration", "@6",
  "init_declarator_list_opt", "init_declarator_list", "init_declarator",
  "declarator", "declarator_cpp", "cpp_initializer_opt",
  "direct_declarator_cpp", "direct_declarator", "declaration_specifiers",
  "type_modifier", "type_modifier_list", "type_specifier",
  "type_qualifier", "type_qualifier_list", "type_qualifier_list_opt",
  "typedef_name", "typedef_name_single", "template_instantiation_opt",
  "template_instantiation", "template_instantiation_list_opt",
  "template_instantiation_list", "template_instantiation_arg",
  "pointer_opt", "pointer", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,    40,    41,   123,   125,    60,    62,    59,    44,
      91,    93,    64,    42
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    44,    45,    45,    46,    46,    47,    47,    48,    48,
      50,    49,    51,    51,    51,    52,    53,    53,    54,    55,
      55,    56,    56,    57,    57,    57,    57,    57,    57,    57,
      57,    58,    59,    61,    60,    62,    64,    63,    65,    65,
      66,    66,    67,    68,    68,    69,    69,    71,    70,    72,
      74,    73,    75,    75,    76,    77,    77,    78,    79,    80,
      80,    81,    81,    82,    82,    83,    84,    84,    85,    87,
      86,    88,    88,    89,    89,    90,    91,    92,    93,    93,
      94,    95,    95,    96,    97,    97,    97,    98,    98,    99,
      99,    99,    99,    99,    99,    99,    99,    99,   100,   100,
     101,   101,   102,   102,   103,   103,   104,   105,   105,   106,
     107,   107,   108,   108,   109,   110,   110,   111,   111
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     3,     0,     2,     1,     2,     4,     2,
       0,     5,     1,     1,     0,     5,     1,     0,     4,     0,
       1,     1,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     4,     0,     5,     3,     0,     5,     0,     1,
       2,     3,     2,     0,     1,     1,     3,     0,     8,     5,
       0,     8,     1,     3,     3,     0,     1,     5,     1,     0,
       1,     1,     2,     0,     1,     1,     1,     3,     2,     0,
       4,     0,     1,     1,     3,     2,     2,     2,     0,     3,
       1,     1,     4,     2,     1,     1,     1,     0,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     2,     0,     1,     1,     3,     2,     0,     1,     3,
       0,     1,     1,     3,     2,     0,     1,     1,     3
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       4,     0,     2,     1,     0,    14,     0,     4,     5,    14,
      13,     0,    87,    12,     9,    10,     3,     0,     4,   115,
       0,     4,     8,     0,   117,    19,     0,   116,    58,    98,
      99,    89,    90,    91,    92,    93,    94,    95,    96,    85,
      86,   107,    88,    83,    84,    97,   104,    21,     0,    17,
     100,   103,     0,    20,    18,    76,    81,    87,   106,   108,
       0,     4,    33,     0,     0,     0,     0,     0,    11,    55,
       4,    30,    29,    23,    28,    24,    25,    27,    26,    31,
      16,    15,   101,   118,    87,   115,     0,   111,   112,   105,
       0,     0,    36,     0,     4,     4,     4,     0,    56,     0,
      22,     0,    64,    65,    66,   115,   114,   109,    87,    35,
      87,     4,     6,    45,     0,    38,     0,     0,     0,     0,
      43,    82,    87,    68,   113,     0,    87,    61,    69,    52,
       0,     7,     0,     0,     0,    42,    39,     0,     0,    32,
      54,     0,    44,    67,    34,    62,    71,    37,     4,    47,
      46,    40,     0,    50,    49,    57,     0,    72,    73,    78,
       0,    53,     4,    41,     4,    70,   115,     4,    75,    80,
      77,     0,     0,    74,     0,    48,    51,    79
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    47,   113,     6,     7,    21,    12,    13,    81,
      14,    54,    48,    70,    71,    72,    73,    91,    74,    75,
     111,   135,   136,   116,   141,   114,    76,   162,    77,    78,
     164,   130,    98,    99,    79,    41,   125,   126,   101,   102,
     103,   104,   127,   146,   156,   157,   158,    25,   159,   168,
     170,    55,    85,    42,    20,    43,    44,    51,    52,    45,
      46,    58,    59,    86,    87,    88,    26,    27
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -128
static const yytype_int8 yypact[] =
{
    -128,    24,    15,  -128,    -9,    10,    16,  -128,  -128,    10,
    -128,    17,  -128,  -128,  -128,  -128,    15,     4,  -128,    -7,
     111,  -128,  -128,     6,     3,    46,    57,  -128,  -128,  -128,
    -128,  -128,  -128,  -128,  -128,  -128,  -128,  -128,  -128,  -128,
    -128,    30,  -128,  -128,  -128,    42,  -128,    15,    29,    48,
    -128,    27,    -7,  -128,  -128,  -128,    -5,    28,  -128,  -128,
      57,  -128,  -128,    34,    37,    41,    49,    52,  -128,    43,
    -128,  -128,  -128,  -128,  -128,  -128,  -128,  -128,  -128,  -128,
    -128,  -128,  -128,  -128,    51,    -7,    53,    54,  -128,  -128,
       1,    55,  -128,    82,  -128,  -128,  -128,    82,  -128,    56,
      15,    58,  -128,    59,  -128,    -7,  -128,  -128,  -128,  -128,
      62,  -128,  -128,    94,    -8,    -1,    68,    69,     7,   -22,
      82,  -128,  -128,  -128,  -128,    70,    71,  -128,  -128,    15,
     -20,  -128,    73,    82,    57,  -128,    64,    74,    66,  -128,
    -128,    76,    80,  -128,  -128,  -128,     2,  -128,  -128,  -128,
      94,  -128,    57,  -128,  -128,  -128,    72,    91,  -128,    81,
      57,    15,  -128,  -128,  -128,  -128,    -7,  -128,  -128,  -128,
    -128,   -14,    50,  -128,     8,  -128,  -128,  -128
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
    -128,  -128,     0,   -18,  -128,  -128,  -128,  -128,  -128,  -128,
     122,  -128,   -32,  -128,  -128,  -128,  -128,  -128,  -128,  -128,
    -128,  -128,  -128,    38,  -128,   -71,  -128,  -128,  -128,  -128,
    -128,   -28,  -128,  -128,  -127,  -120,  -128,  -128,  -128,  -128,
    -128,    13,    11,  -128,  -128,  -128,   -30,    33,  -128,  -128,
    -128,  -128,   -10,  -128,  -128,  -128,   -16,  -128,  -128,   113,
      83,  -128,  -128,  -128,  -128,    32,   -84,    89
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -116
static const yytype_int16 yytable[] =
{
       2,   106,    19,   148,     8,  -115,     8,    16,    50,    29,
      30,     8,     8,     8,   151,   147,    60,   133,    23,   140,
       8,   175,    69,     9,     3,   132,   119,    84,    69,    10,
      11,   133,   163,    29,    30,    82,    24,    22,   134,   109,
     169,   177,   139,    49,   148,    24,  -102,     4,     5,   142,
      15,    61,    53,    18,    62,    63,    64,    65,    66,    67,
      28,    90,   160,    60,    68,  -110,    57,    80,    92,    93,
     100,    69,    61,    94,   105,    62,    63,    64,    65,    66,
      67,    95,   160,    97,   -63,   176,    96,   112,   120,   110,
     107,   121,    69,   108,   115,   115,   118,   -59,   122,   131,
     128,   137,   138,   152,   154,   144,   -60,   149,   153,   155,
     165,   129,   105,   167,    28,   150,   128,    29,    30,   133,
      31,    32,    33,    34,    35,    36,    37,    38,    39,    40,
     166,    17,   172,   117,   171,   143,   173,   145,   123,    56,
     124,    83,     0,    89,     0,     0,     0,     0,   161,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   129,     0,     0,     0,     0,   174
};

static const yytype_int16 yycheck[] =
{
       0,    85,    12,   130,     5,     3,     5,     7,    24,     6,
       7,     5,     5,     5,   134,    35,    21,    39,    18,    41,
       5,    35,    42,    32,     0,    33,    97,    32,    42,    19,
      20,    39,   152,     6,     7,    51,    43,    33,    39,    38,
     160,    33,    35,    37,   171,    43,    43,    23,    24,   120,
      34,    22,     6,    36,    25,    26,    27,    28,    29,    30,
       3,    61,   146,    21,    35,    37,    36,    19,    34,    32,
      70,    42,    22,    32,    84,    25,    26,    27,    28,    29,
      30,    32,   166,    40,    33,    35,    34,     5,    32,    34,
      37,    33,    42,    39,    94,    95,    96,    35,    39,     5,
     110,    33,    33,    39,    38,    35,    35,    34,    34,    33,
      38,   111,   122,    32,     3,   133,   126,     6,     7,    39,
       9,    10,    11,    12,    13,    14,    15,    16,    17,    18,
      39,     9,   164,    95,   162,   122,   166,   126,   105,    26,
     108,    52,    -1,    60,    -1,    -1,    -1,    -1,   148,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,   162,    -1,    -1,    -1,    -1,   167
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    45,    46,     0,    23,    24,    48,    49,     5,    32,
      19,    20,    51,    52,    54,    34,    46,    54,    36,    96,
      98,    50,    33,    46,    43,    91,   110,   111,     3,     6,
       7,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    79,    97,    99,   100,   103,   104,    46,    56,    37,
     100,   101,   102,     6,    55,    95,   103,    36,   105,   106,
      21,    22,    25,    26,    27,    28,    29,    30,    35,    42,
      57,    58,    59,    60,    62,    63,    70,    72,    73,    78,
      19,    53,   100,   111,    32,    96,   107,   108,   109,   104,
      46,    61,    34,    32,    32,    32,    34,    40,    76,    77,
      46,    82,    83,    84,    85,    96,   110,    37,    39,    38,
      34,    64,     5,    47,    69,    46,    67,    67,    46,    69,
      32,    33,    39,    91,   109,    80,    81,    86,    96,    46,
      75,     5,    33,    39,    39,    65,    66,    33,    33,    35,
      41,    68,    69,    85,    35,    86,    87,    35,    78,    34,
      47,    79,    39,    34,    38,    33,    88,    89,    90,    92,
     110,    46,    71,    79,    74,    38,    39,    32,    93,    79,
      94,    75,    56,    90,    46,    35,    35,    33
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */



/* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  
  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	struct yyalloc *yyptr =
	  (struct yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 100 "./parse.yy"
    { state.passthrough ((yyvsp[(1) - (1)].str)); }
    break;

  case 3:
#line 101 "./parse.yy"
    { state.passthrough ((yyvsp[(3) - (3)].str)); }
    break;

  case 4:
#line 104 "./parse.yy"
    { (yyval.str) = lstr (get_yy_lineno ()); }
    break;

  case 5:
#line 106 "./parse.yy"
    {
	   strbuf b ((yyvsp[(1) - (2)].str));
	   b << (yyvsp[(2) - (2)].str);
	   (yyval.str) = lstr ((yyvsp[(1) - (2)].str).lineno (), b);
	}
    break;

  case 6:
#line 113 "./parse.yy"
    { (yyval.str) = lstr (get_yy_lineno (), (yyvsp[(1) - (1)].str)); }
    break;

  case 7:
#line 115 "./parse.yy"
    {
	   strbuf b ((yyvsp[(1) - (2)].str));
	   b << (yyvsp[(2) - (2)].str);
	   (yyval.str) = lstr ((yyvsp[(1) - (2)].str).lineno (), b);
	}
    break;

  case 8:
#line 122 "./parse.yy"
    { (yyval.fn) = (yyvsp[(3) - (4)].fn); }
    break;

  case 9:
#line 123 "./parse.yy"
    { (yyval.fn) = (yyvsp[(2) - (2)].fn); }
    break;

  case 10:
#line 127 "./parse.yy"
    {
	  state.new_fn ((yyvsp[(1) - (2)].fn));
	  state.push_list ((yyvsp[(1) - (2)].fn));
	  (yyvsp[(1) - (2)].fn)->set_lbrace_lineno (get_yy_lineno ());
	}
    break;

  case 11:
#line 133 "./parse.yy"
    {
	  if (!state.function ()->check_return_type ()) {
	    yyerror ("Function has non-void return type but no "
	    	     "DEFAULT_RETURN specified");
 	  }
	  state.push (New tame_fn_return_t (get_yy_lineno (), 
				            state.function ()));
	  state.passthrough (lstr (get_yy_lineno (), "}"));
	  state.pop_list ();
	}
    break;

  case 12:
#line 145 "./parse.yy"
    { (yyval.fn_spc) = (yyvsp[(1) - (1)].fn_spc); }
    break;

  case 13:
#line 146 "./parse.yy"
    { (yyval.fn_spc) = fn_specifier_t (STATIC_DECL); }
    break;

  case 14:
#line 147 "./parse.yy"
    { (yyval.fn_spc) = fn_specifier_t (0); }
    break;

  case 15:
#line 151 "./parse.yy"
    {
	   (yyval.fn_spc) = fn_specifier_t ((yyvsp[(5) - (5)].opts), (yyvsp[(3) - (5)].str));
	}
    break;

  case 16:
#line 156 "./parse.yy"
    { (yyval.opts) = STATIC_DECL; }
    break;

  case 17:
#line 157 "./parse.yy"
    { (yyval.opts) = 0; }
    break;

  case 18:
#line 162 "./parse.yy"
    {
	   (yyval.fn) = New tame_fn_t ((yyvsp[(1) - (4)].fn_spc), (yyvsp[(2) - (4)].str), (yyvsp[(3) - (4)].decl), (yyvsp[(4) - (4)].opt), get_yy_lineno (), 
	                       get_yy_loc ());
	}
    break;

  case 19:
#line 168 "./parse.yy"
    { (yyval.opt) = false; }
    break;

  case 20:
#line 169 "./parse.yy"
    { (yyval.opt) = true; }
    break;

  case 21:
#line 173 "./parse.yy"
    {
	  state.passthrough ((yyvsp[(1) - (1)].str));
	}
    break;

  case 22:
#line 177 "./parse.yy"
    {
   	  if ((yyvsp[(2) - (3)].el)) state.push ((yyvsp[(2) - (3)].el));
	  state.passthrough ((yyvsp[(3) - (3)].str));
	}
    break;

  case 31:
#line 194 "./parse.yy"
    {
	  tame_nonblock_t *c = New tame_nonblock_t (NULL);
	  (yyvsp[(1) - (1)].cb)->set_nonblock (c);
	  c->push ((yyvsp[(1) - (1)].cb));
	  (yyval.el) = c;
	}
    break;

  case 32:
#line 203 "./parse.yy"
    {
	  // this thing will not be output anywhere near where
	  // it's being input, so don't associate it in the 
	  // element list as usual.
	  if (!state.function ()->set_default_return ((yyvsp[(3) - (4)].str))) {
	    yyerror ("DEFAULT_RETURN specified more than once");
	  }
	  (yyval.el) = NULL;
	}
    break;

  case 33:
#line 215 "./parse.yy"
    {
	  vars_lineno = get_yy_lineno ();
	}
    break;

  case 34:
#line 219 "./parse.yy"
    {
	  tame_vars_t *v = New tame_vars_t (state.function (), vars_lineno);
	  if (state.function ()->get_vars ()) {
	    strbuf b;
	    b << "VARS{} section specified twice (before on line " 
	      << state.function ()->get_vars ()->lineno () << ")\n";
	    yyerror (b);
	  }
	  if (!state.function ()->set_vars (v)) {
	    yyerror ("The VARS{} section must come before any BLOCK, "
	             " WAIT or NONBLOCK section");
	  }
	  (yyval.el) = v;
	}
    break;

  case 35:
#line 236 "./parse.yy"
    {
	   tame_ret_t *r = New tame_ret_t (get_yy_lineno (), 
			  	    state.function ());	
	   if ((yyvsp[(2) - (3)].str))
	     r->add_params ((yyvsp[(2) - (3)].str));
 	   r->passthrough (lstr (get_yy_lineno (), ";"));
	   (yyval.el) = r;
	}
    break;

  case 36:
#line 247 "./parse.yy"
    {
	  tame_fn_t *fn = state.function ();
 	  tame_block_t *bl = New tame_block_t (fn, get_yy_lineno ());
	  state.new_block (bl);
	  fn->add_env (bl);
	  fn->hit_tame_block ();
	  state.push_list (bl);
	}
    break;

  case 37:
#line 256 "./parse.yy"
    {
 	  state.pop_list ();
	  (yyval.el) = state.block ();
	  state.clear_block ();
	}
    break;

  case 38:
#line 263 "./parse.yy"
    { (yyval.exprs) = NULL; }
    break;

  case 40:
#line 268 "./parse.yy"
    {
	  (yyval.exprs) = New refcounted<expr_list_t> ();
	  (yyval.exprs)->push_back (var_t ()); // reserve 1 empty spot!
	  (yyval.exprs)->push_back (var_t ((yyvsp[(2) - (2)].str), STACK));
	}
    break;

  case 41:
#line 274 "./parse.yy"
    {
	  (yyvsp[(1) - (3)].exprs)->push_back (var_t ((yyvsp[(3) - (3)].str), STACK));
	  (yyval.exprs) = (yyvsp[(1) - (3)].exprs);
	}
    break;

  case 42:
#line 281 "./parse.yy"
    {
	  if ((yyvsp[(2) - (2)].exprs)) {
	    (*(yyvsp[(2) - (2)].exprs))[0] = var_t ((yyvsp[(1) - (2)].str), EXPR);
	    (yyval.exprs) = (yyvsp[(2) - (2)].exprs);
	  } else {
	    (yyval.exprs) = New refcounted<expr_list_t> ();
	    (yyval.exprs)->push_back (var_t ((yyvsp[(1) - (2)].str), EXPR));
	  }
	}
    break;

  case 43:
#line 292 "./parse.yy"
    { (yyval.exprs) = New refcounted<expr_list_t> (); }
    break;

  case 45:
#line 297 "./parse.yy"
    {
	  (yyval.exprs) = New refcounted<expr_list_t> ();
	  (yyval.exprs)->push_back (var_t ((yyvsp[(1) - (1)].str), EXPR));
	}
    break;

  case 46:
#line 303 "./parse.yy"
    {
	  (yyvsp[(1) - (3)].exprs)->push_back (var_t ((yyvsp[(3) - (3)].str), EXPR));
	  (yyval.exprs) = (yyvsp[(1) - (3)].exprs);
	}
    break;

  case 47:
#line 310 "./parse.yy"
    {
	  tame_fn_t *fn = state.function ();
 	  tame_nonblock_t *c = New tame_nonblock_t ((yyvsp[(3) - (5)].exprs));
	  state.new_nonblock (c);
	  fn->add_env (c);
	  state.push_list (c);
	  state.passthrough (lstr (get_yy_lineno (), "{"));
	}
    break;

  case 48:
#line 319 "./parse.yy"
    {
	  state.passthrough (lstr (get_yy_lineno (), "}"));
	  state.pop_list ();
	  (yyval.el) = state.nonblock ();
	}
    break;

  case 49:
#line 327 "./parse.yy"
    {
	  tame_fn_t *fn = state.function ();
	  tame_wait_t *w = New tame_wait_t (fn, (yyvsp[(3) - (5)].exprs), get_yy_lineno ());
	  fn->add_env (w);
	  (yyval.el) = w;
	}
    break;

  case 50:
#line 336 "./parse.yy"
    {
	  tame_fn_t *fn = state.function ();
	  tame_join_t *jn = New tame_join_t (fn, (yyvsp[(3) - (5)].exprs));
	  state.new_join (jn);
	  fn->add_env (jn);
	  state.passthrough (lstr (get_yy_lineno (), "{"));
	  state.push_list (jn);
	}
    break;

  case 51:
#line 345 "./parse.yy"
    {
	  state.pop_list ();
	  state.passthrough (lstr (get_yy_lineno (), "}"));
	  (yyval.el) = state.join ();
	}
    break;

  case 52:
#line 353 "./parse.yy"
    { 
	  state.passthrough ((yyvsp[(1) - (1)].str)); 
	}
    break;

  case 53:
#line 357 "./parse.yy"
    {
	  state.push ((yyvsp[(2) - (3)].cb));
	  state.passthrough ((yyvsp[(3) - (3)].str));
	}
    break;

  case 54:
#line 364 "./parse.yy"
    {
	  (yyval.exprs) = (yyvsp[(2) - (3)].exprs);
	}
    break;

  case 55:
#line 369 "./parse.yy"
    { (yyval.exprs) = NULL; }
    break;

  case 56:
#line 370 "./parse.yy"
    { (yyval.exprs) = (yyvsp[(1) - (1)].exprs); }
    break;

  case 57:
#line 374 "./parse.yy"
    {
  	  tame_fn_t *fn = state.function ();
	  if (state.block ()) {
 	    // Callbacks are labeled serially within a function; the 
	    // constructor sets this ID.
	    if ((yyvsp[(2) - (5)].exprs)) {
	      yyerror ("Cannot give '[..]' args to callback within BLOCK");
	    }
	    (yyval.cb) = New tame_block_callback_t (get_yy_lineno (), 
				            fn, state.block (), (yyvsp[(4) - (5)].exprs));
	  } else {
	    if (!state.nonblock () && ! (yyvsp[(2) - (5)].exprs)) {
	      yyerror ("Nonblocking callbacks must be associated with a "
	 	       "join_group_t");
	    }
	    tame_nonblock_callback_t *cb = 
              New tame_nonblock_callback_t (get_yy_lineno (), 
				            state.nonblock (), (yyvsp[(4) - (5)].exprs), (yyvsp[(2) - (5)].exprs));
	    fn->add_nonblock_callback (cb);
	    (yyval.cb) = cb;
	  }
	}
    break;

  case 63:
#line 419 "./parse.yy"
    { (yyval.vars) = NULL; }
    break;

  case 66:
#line 429 "./parse.yy"
    {
	  (yyval.vars) = New refcounted<vartab_t> ((yyvsp[(1) - (1)].var));
	}
    break;

  case 67:
#line 433 "./parse.yy"
    {
	  if (! ((yyvsp[(1) - (3)].vars))->add ((yyvsp[(3) - (3)].var)) ) {
	    strbuf b;
	    b << "duplicated parameter in param list: " << (yyvsp[(3) - (3)].var).name ();
	    yyerror (b);
          } else {
 	    (yyval.vars) = (yyvsp[(1) - (3)].vars);
          }
	}
    break;

  case 68:
#line 447 "./parse.yy"
    {
	  if ((yyvsp[(2) - (2)].decl)->params ()) {
	    warn << "parameters found when not expected\n";
	  }
	  (yyval.var) = var_t ((yyvsp[(1) - (2)].str), (yyvsp[(2) - (2)].decl), ARG);
	}
    break;

  case 69:
#line 456 "./parse.yy"
    {
	  state.set_decl_specifier ((yyvsp[(1) - (1)].str));
	}
    break;

  case 73:
#line 466 "./parse.yy"
    {}
    break;

  case 74:
#line 467 "./parse.yy"
    {}
    break;

  case 75:
#line 473 "./parse.yy"
    {
	  if ((yyvsp[(2) - (2)].str) && (yyvsp[(2) - (2)].str).len () > 0) 
	    (yyvsp[(1) - (2)].decl)->set_initializer ((yyvsp[(2) - (2)].str));

	  vartab_t *t = state.stack_vars ();

	  var_t v (state.decl_specifier (), (yyvsp[(1) - (2)].decl), STACK);
	  if (state.args () &&
              state.args ()->exists (v.name ())) {
	    strbuf b;
	    b << "stack variable '" << v.name () << "' shadows a parameter";
	    yyerror (b);
	  }
	  if (!t->add (v)) {
	    strbuf b;
	    b << "redefinition of stack variable: " << v.name () ;
 	    yyerror (b);
          }
	}
    break;

  case 76:
#line 495 "./parse.yy"
    {
	  if ((yyvsp[(1) - (2)].str).len () > 0) 
	    (yyvsp[(2) - (2)].decl)->set_pointer ((yyvsp[(1) - (2)].str));
  	  (yyval.decl) = (yyvsp[(2) - (2)].decl);
	}
    break;

  case 77:
#line 503 "./parse.yy"
    {
	  if ((yyvsp[(1) - (2)].str).len () > 0) 
	    (yyvsp[(2) - (2)].decl)->set_pointer ((yyvsp[(1) - (2)].str));
  	  (yyval.decl) = (yyvsp[(2) - (2)].decl);
	}
    break;

  case 78:
#line 510 "./parse.yy"
    { (yyval.str) = lstr (get_yy_lineno (), NULL); }
    break;

  case 79:
#line 512 "./parse.yy"
    {
	  (yyval.str) = (yyvsp[(2) - (3)].str);
	}
    break;

  case 80:
#line 518 "./parse.yy"
    {
	  (yyval.decl) = New refcounted<declarator_t> ((yyvsp[(1) - (1)].str));
	}
    break;

  case 81:
#line 529 "./parse.yy"
    {
	   (yyval.decl) = New refcounted<declarator_t> ((yyvsp[(1) - (1)].str));
	}
    break;

  case 82:
#line 533 "./parse.yy"
    {
	   (yyval.decl) = New refcounted<declarator_t> ((yyvsp[(1) - (4)].str), (yyvsp[(3) - (4)].vars));
	}
    break;

  case 83:
#line 547 "./parse.yy"
    {
	   CONCAT((yyvsp[(1) - (2)].str).lineno (), (yyvsp[(1) - (2)].str) << " " << (yyvsp[(2) - (2)].str), (yyval.str));
	}
    break;

  case 85:
#line 556 "./parse.yy"
    { (yyval.str) = "signed"; }
    break;

  case 86:
#line 557 "./parse.yy"
    { (yyval.str) = "unsigned"; }
    break;

  case 87:
#line 560 "./parse.yy"
    { (yyval.str) = ""; }
    break;

  case 88:
#line 562 "./parse.yy"
    {
	  CONCAT ((yyvsp[(1) - (2)].str).lineno (), (yyvsp[(1) - (2)].str) << " " << (yyvsp[(2) - (2)].str), (yyval.str));
	}
    break;

  case 89:
#line 572 "./parse.yy"
    { (yyval.str) = "void" ; }
    break;

  case 90:
#line 573 "./parse.yy"
    { (yyval.str) = "char";  }
    break;

  case 91:
#line 574 "./parse.yy"
    { (yyval.str) = "short"; }
    break;

  case 92:
#line 575 "./parse.yy"
    { (yyval.str) = "int" ; }
    break;

  case 93:
#line 576 "./parse.yy"
    { (yyval.str) = "long" ; }
    break;

  case 94:
#line 577 "./parse.yy"
    { (yyval.str) = "long long"; }
    break;

  case 95:
#line 578 "./parse.yy"
    { (yyval.str) = "float"; }
    break;

  case 96:
#line 579 "./parse.yy"
    { (yyval.str) = "double" ; }
    break;

  case 98:
#line 586 "./parse.yy"
    { (yyval.str) = "const"; }
    break;

  case 99:
#line 587 "./parse.yy"
    { (yyval.str) = "struct"; }
    break;

  case 101:
#line 592 "./parse.yy"
    {
	  CONCAT((yyvsp[(1) - (2)].str).lineno (), (yyvsp[(1) - (2)].str) << " " << (yyvsp[(2) - (2)].str), (yyval.str));
	}
    break;

  case 102:
#line 597 "./parse.yy"
    { (yyval.str) = ""; }
    break;

  case 105:
#line 607 "./parse.yy"
    {
	   CONCAT((yyvsp[(1) - (3)].str).lineno (), (yyvsp[(1) - (3)].str) << "::" << (yyvsp[(3) - (3)].str), (yyval.str));
	}
    break;

  case 106:
#line 613 "./parse.yy"
    {
          CONCAT((yyvsp[(1) - (2)].str).lineno (), (yyvsp[(1) - (2)].str) << (yyvsp[(2) - (2)].str), (yyval.str));
	}
    break;

  case 107:
#line 618 "./parse.yy"
    { (yyval.str) = ""; }
    break;

  case 109:
#line 623 "./parse.yy"
    {
	  CONCAT((yyvsp[(2) - (3)].str).lineno (), "<" << (yyvsp[(2) - (3)].str) << ">", (yyval.str));
	}
    break;

  case 110:
#line 628 "./parse.yy"
    { (yyval.str) = "" ; }
    break;

  case 113:
#line 634 "./parse.yy"
    {
	  CONCAT((yyvsp[(1) - (3)].str).lineno (), (yyvsp[(1) - (3)].str) << " , " << (yyvsp[(3) - (3)].str), (yyval.str));
	}
    break;

  case 114:
#line 640 "./parse.yy"
    {
	  CONCAT((yyvsp[(1) - (2)].str).lineno (), (yyvsp[(1) - (2)].str) << " " << (yyvsp[(2) - (2)].str), (yyval.str));
	}
    break;

  case 115:
#line 645 "./parse.yy"
    { (yyval.str) = ""; }
    break;

  case 117:
#line 649 "./parse.yy"
    { (yyval.str) = "*"; }
    break;

  case 118:
#line 651 "./parse.yy"
    {
	  CONCAT((yyvsp[(2) - (3)].str).lineno (), " * " << (yyvsp[(2) - (3)].str) << (yyvsp[(3) - (3)].str), (yyval.str));
	}
    break;


/* Line 1267 of yacc.c.  */
#line 2216 "y.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 656 "./parse.yy"



