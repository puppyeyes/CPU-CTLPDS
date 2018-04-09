/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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
#define YYBISON_VERSION "3.0.4"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1


/* Substitute the variable and function names.  */
#define yyparse         pdsparse
#define yylex           pdslex
#define yyerror         pdserror
#define yydebug         pdsdebug
#define yynerrs         pdsnerrs

#define yylval          pdslval
#define yychar          pdschar

/* Copy the first part of user declarations.  */
#line 1 "pdsparse.y" /* yacc.c:339  */


#include <string.h>

#include "pds.h"
#include "abpds.h"
#include "common.h"
#include "variables.h"
#include "reorder.h"
#include "ftree.h"
#include "expr.h"
#include "bdd.h"


//int pdslineno = 0;
int pds_p1, pds_y1, pds_p2, pds_y2, pds_y3;
int pp_current_label;
int length=0;
int mc_pds_line_num=0;
extern int pdslex();
extern FormulaTree ctlTree;

void pdserror (char*);

extern PushdownSystem mc_pds;
extern AlternatingBuchiPushdownSystem mc_abpds;
extern int mc_verbose;
extern int mc_restrict;
extern int mc_logic_lang_CTL;
extern NameTreeList mc_atoms;
extern Name sharp;
extern ValList abpdsValList;
extern int abpdsValNum;
extern int mc_CTPL;


#line 111 "pdsparse.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif


/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int pdsdebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    LABEL = 258,
    IDENT = 259,
    NUMBER = 260,
    QUANT = 261,
    PDS_OR = 262,
    PDS_AND = 263,
    PDS_NOT = 264,
    PDS_EQV = 265,
    PDS_XOR = 266,
    PDS_PLUS = 267,
    PDS_MINUS = 268,
    PDS_MULT = 269,
    PDS_DIV = 270,
    PDS_SHL = 271,
    PDS_LE = 272,
    PDS_EQ = 273,
    PDS_NE = 274,
    PDS_GE = 275,
    PDS_FORALL = 276,
    PDS_EXISTS = 277,
    ARROW = 278,
    PRIME = 279,
    GLOBAL = 280,
    LOCAL = 281,
    BOOL = 282,
    INT = 283,
    DEFINE = 284,
    ATOMS = 285,
    RANGE = 286,
    BEGIN_RULE = 287,
    BEGIN_PROPOSITION = 288,
    CSQ = 289,
    CSEND = 290,
    CSINIT = 291
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 39 "pdsparse.y" /* yacc.c:355  */

	char *string;
	int integer;
	NameStack stack;
	NameSet set;
	Name name;
	DdNode *bdd;
	DdNode **varptr;
	ident_t ident;
	identref_t identref;
	FormulaTree etree;
	block_t block;
	const_t quant;
	ValList valList;

#line 201 "pdsparse.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE pdslval;

int pdsparse (void);



/* Copy the second part of user declarations.  */

#line 218 "pdsparse.c" /* yacc.c:358  */

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
#else
typedef signed char yytype_int8;
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
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
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
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  10
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   343

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  48
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  57
/* YYNRULES -- Number of rules.  */
#define YYNRULES  144
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  281

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   291

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,    39,     2,     2,     2,     2,
      41,    37,     2,     2,    40,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    38,
      44,     2,    45,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    42,     2,    43,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    46,     2,    47,     2,     2,     2,     2,
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
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   104,   104,   105,   106,   107,   120,   122,   122,   124,
     124,   125,   125,   126,   126,   127,   128,   129,   130,   131,
     132,   134,   159,   187,   212,   242,   244,   244,   246,   260,
     288,   298,   309,   310,   311,   312,   314,   322,   323,   325,
     326,   328,   335,   351,   356,   362,   366,   374,   417,   418,
     420,   421,   435,   436,   438,   441,   447,   453,   454,   461,
     462,   464,   465,   467,   476,   485,   488,   494,   501,   502,
     506,   518,   527,   539,   553,   559,   562,   565,   570,   577,
     584,   594,   600,   606,   614,   615,   617,   617,   619,   620,
     622,   644,   674,   677,   680,   692,   707,   716,   730,   742,
     752,   753,   755,   756,   757,   758,   759,   760,   761,   762,
     763,   770,   778,   787,   793,   799,   809,   815,   822,   823,
     824,   825,   826,   827,   834,   845,   846,   847,   849,   850,
     851,   852,   853,   854,   856,   864,   870,   876,   878,   879,
     880,   881,   882,   883,   884
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "LABEL", "IDENT", "NUMBER", "QUANT",
  "PDS_OR", "PDS_AND", "PDS_NOT", "PDS_EQV", "PDS_XOR", "PDS_PLUS",
  "PDS_MINUS", "PDS_MULT", "PDS_DIV", "PDS_SHL", "PDS_LE", "PDS_EQ",
  "PDS_NE", "PDS_GE", "PDS_FORALL", "PDS_EXISTS", "ARROW", "PRIME",
  "GLOBAL", "LOCAL", "BOOL", "INT", "DEFINE", "ATOMS", "RANGE",
  "BEGIN_RULE", "BEGIN_PROPOSITION", "CSQ", "CSEND", "CSINIT", "')'",
  "';'", "'#'", "','", "'('", "'['", "']'", "'<'", "'>'", "'{'", "'}'",
  "$accept", "automaton", "propositions", "props", "prop", "$@1", "$@2",
  "$@3", "IDENTS", "rules", "rule_list", "rule", "ctrl_state", "defines",
  "universal", "vallist", "valdecl", "value", "vardecl", "globals",
  "locals", "optproc", "symblist", "symbols", "varlist", "decl",
  "boollist", "boolident", "intident", "optdim", "intlist", "startconfig",
  "startstate", "startident", "stackident", "stacklist", "atoms", "atom",
  "$@4", "proplist", "propident", "transitionlist", "transition",
  "simp_trans_wrap", "simple_transition", "label", "expr", "quant",
  "expression", "boolvar", "indexvar", "intexpr", "intterm", "comp_op",
  "variable", "varident", "constexp", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,    41,    59,    35,
      44,    40,    91,    93,    60,    62,   123,   125
};
# endif

#define YYPACT_NINF -123

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-123)))

#define YYTABLE_NINF -116

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -12,   150,    26,    90,     4,  -123,    87,    99,   150,  -123,
    -123,   139,   170,   156,  -123,   162,   173,   187,    73,  -123,
     187,  -123,    92,  -123,    23,    23,   190,  -123,   191,    70,
    -123,  -123,   230,    23,  -123,  -123,    87,   194,  -123,    99,
    -123,    23,     9,     9,   170,   106,  -123,   -26,  -123,  -123,
    -123,  -123,  -123,    68,   -24,  -123,   195,   144,  -123,    23,
    -123,   201,    23,    23,    23,    23,    23,  -123,  -123,  -123,
    -123,  -123,   200,   196,   215,   253,  -123,  -123,   254,   208,
     236,    69,    77,    79,  -123,   256,   150,    23,  -123,   209,
    -123,   123,   123,   245,   245,  -123,    83,    71,   254,  -123,
     254,   218,  -123,   260,  -123,    70,    70,   231,   232,   233,
     234,   237,   238,  -123,    -7,   150,   149,  -123,  -123,  -123,
      43,   222,  -123,  -123,  -123,    71,   266,   266,    71,    13,
     116,   239,  -123,   175,  -123,   135,    78,   247,   271,   273,
    -123,   241,   274,    86,   275,   242,   276,   244,   280,   246,
    -123,   282,  -123,    71,  -123,   250,  -123,  -123,   248,   249,
     183,  -123,    82,   251,    71,    71,    71,    71,  -123,    13,
      13,    13,    13,    13,  -123,  -123,  -123,  -123,  -123,  -123,
      13,    13,   264,   255,   252,  -123,   257,    71,    25,   258,
     261,   262,   263,   265,   267,   268,   269,  -123,   272,  -123,
      23,    23,  -123,  -123,   283,  -123,    24,   181,   157,   157,
     278,   278,  -123,   240,   154,  -123,   206,   271,   281,   277,
     270,  -123,   279,  -123,  -123,   291,  -123,  -123,   291,  -123,
    -123,   291,  -123,    83,   188,   193,  -123,  -123,  -123,   284,
     285,  -123,   294,  -123,  -123,  -123,   286,    95,    96,    97,
    -123,    23,    23,  -123,   206,   287,   307,   288,  -123,  -123,
    -123,   227,   235,  -123,     1,   289,   311,    71,    71,     3,
    -123,   312,   295,   116,   116,   290,  -123,  -123,   314,  -123,
    -123
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
      38,     0,     0,    39,     0,    50,     0,     0,    48,    57,
       1,     0,     0,    49,    26,     0,    47,    65,     0,    61,
      65,    68,     0,    58,     0,     0,    41,    40,    71,    25,
       7,     5,    53,     0,    63,    59,     0,     0,    60,     0,
     144,     0,    36,    37,     0,     0,    84,    71,    32,    33,
      35,    34,    27,     0,     6,    52,     0,     0,    62,     0,
      69,     0,     0,     0,     0,     0,     0,    42,    74,    76,
      77,    75,     0,    72,    92,     0,    92,    85,     4,     0,
       0,     0,     0,     0,     8,     0,     0,     0,    66,     0,
     143,   138,   139,   140,   141,   142,     0,     0,     2,    86,
       3,     0,    93,   100,    96,     0,     0,     0,     0,     0,
       0,     0,     0,    55,     0,    51,     0,    64,    78,    79,
      80,     0,   137,   126,   125,     0,     0,     0,     0,     0,
     113,     0,   102,   124,   103,     0,   123,   134,     0,     0,
     101,    94,     0,     0,     0,     0,     0,     0,     0,     0,
      54,     0,    67,     0,    82,     0,   109,   112,     0,     0,
       0,   124,     0,   123,     0,     0,     0,     0,    73,     0,
       0,     0,     0,     0,   129,   130,   131,   132,   128,   133,
       0,     0,   135,    90,    87,    88,     0,     0,     0,     0,
       0,     9,     0,    11,     0,    13,     0,    56,     0,    70,
       0,     0,   104,   127,   105,   106,   107,   108,   118,   119,
     120,   121,   122,   117,     0,   136,     0,     0,     0,     0,
       0,    29,     0,    30,    15,     0,    18,    16,     0,    19,
      17,     0,    20,    81,     0,     0,   116,    45,    46,     0,
      43,    89,     0,    95,    28,    31,    21,     0,     0,     0,
      83,     0,     0,    91,     0,     0,     0,    23,    10,    12,
      14,     0,     0,    44,     0,     0,     0,     0,     0,     0,
      99,     0,     0,   110,   111,     0,    98,    22,     0,    97,
      24
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -123,  -123,  -123,  -123,  -123,  -123,  -123,  -123,  -100,  -123,
    -123,  -123,   113,  -123,  -123,   292,    66,  -123,   309,  -123,
    -123,  -123,  -123,  -123,   243,    -6,  -123,   296,   298,   303,
    -123,   293,  -123,  -123,  -123,  -119,   297,  -123,  -123,  -123,
     107,   122,  -123,  -123,  -123,  -123,  -122,   199,   -51,  -123,
    -120,  -123,  -115,  -123,  -102,  -123,   -25
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,    31,    54,    84,   225,   228,   231,   247,    15,
      29,    52,    53,     3,    13,    27,   239,   240,     4,     5,
      16,    56,    86,   114,     8,     9,    18,    19,    21,    34,
      22,    46,    72,    73,   120,   121,    47,    77,   138,   184,
     185,    78,   102,   103,   104,   141,   130,   158,   131,   132,
     133,   134,   135,   180,   136,   137,    42
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      43,   154,    23,   156,    75,   269,   160,   275,    57,   161,
      81,    82,    83,     1,   162,    45,    61,   122,   123,   124,
     -49,    62,    63,    64,    65,    66,    10,   163,    40,   220,
     150,   164,   165,   151,    89,   167,    14,    91,    92,    93,
      94,    95,   204,   205,   206,   207,   270,   118,   276,   161,
     161,   161,   161,   161,   208,   209,   210,   211,   212,   129,
     161,   161,   116,   221,    41,   213,   214,   163,   163,   163,
     163,   163,    79,   107,    48,   122,   123,   124,   163,   163,
     125,   109,   119,   111,   153,  -114,  -114,   118,  -114,  -114,
     189,    17,   126,   127,   169,   170,   171,   172,   173,   257,
     257,   257,   198,    20,    49,    50,    51,    80,   108,    23,
      68,    35,   128,    36,   250,  -114,   110,   129,   112,    11,
     181,    12,   119,   164,   165,   190,   166,   167,   248,   203,
      38,   249,    39,   258,   259,   260,   219,    64,    65,    66,
      69,    70,    71,    24,    25,   273,   274,   169,   170,   171,
     172,   173,   174,   175,   176,   177,    62,    63,    64,    65,
      66,    62,    63,    64,    65,    66,   169,   170,   171,   172,
     173,   171,   172,   173,    26,   234,   235,     6,     7,   178,
     179,     1,  -115,  -115,    87,  -115,  -115,    88,   164,   165,
     164,   165,   152,   166,   167,    30,    98,   236,   100,    32,
      62,    63,    64,    65,    66,    62,    63,    64,    65,    66,
     237,   238,  -115,    62,    63,    64,    65,    66,   142,   143,
     202,    62,    63,    64,    65,    66,   261,   262,   251,    33,
      44,   105,    45,   252,    55,    59,    85,    97,    90,    62,
      63,    64,    65,    66,    96,    75,   117,    62,    63,    64,
      65,    66,   169,   170,   171,   172,   173,    99,   101,   106,
     113,    66,   139,   140,   267,   144,   145,   155,   146,   147,
     157,   182,   268,   148,   149,   183,   168,   186,   188,   191,
     193,   192,   187,   194,   195,   196,   197,   199,   215,   200,
     201,   165,   217,   181,   173,   246,   216,   222,   255,   223,
     224,   226,   218,   227,   242,   229,   230,   232,   244,   233,
     256,   265,   266,   271,   243,   272,   277,   245,   280,   278,
     263,   253,    28,    37,   241,   254,   159,     0,     0,   115,
       0,   264,    58,     0,     0,   279,    67,    60,     0,     0,
      76,     0,     0,    74
};

static const yytype_int16 yycheck[] =
{
      25,   120,     8,   125,    30,     4,   128,     4,    33,   129,
      34,    35,    36,    25,   129,    41,    41,     4,     5,     6,
      32,    12,    13,    14,    15,    16,     0,   129,     5,     4,
      37,     7,     8,    40,    59,    11,    32,    62,    63,    64,
      65,    66,   164,   165,   166,   167,    45,     4,    45,   169,
     170,   171,   172,   173,   169,   170,   171,   172,   173,    46,
     180,   181,    87,    38,    41,   180,   181,   169,   170,   171,
     172,   173,     4,     4,     4,     4,     5,     6,   180,   181,
       9,     4,    39,     4,    41,     7,     8,     4,    10,    11,
       4,     4,    21,    22,    12,    13,    14,    15,    16,     4,
       4,     4,   153,     4,    34,    35,    36,    39,    39,   115,
       4,    38,    41,    40,   233,    37,    39,    46,    39,    29,
      42,    31,    39,     7,     8,    39,    10,    11,   228,    47,
      38,   231,    40,    38,    38,    38,   187,    14,    15,    16,
      34,    35,    36,     4,     5,   267,   268,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    12,    13,    14,    15,
      16,    12,    13,    14,    15,    16,    12,    13,    14,    15,
      16,    14,    15,    16,     4,   200,   201,    27,    28,    44,
      45,    25,     7,     8,    40,    10,    11,    43,     7,     8,
       7,     8,    43,    10,    11,    33,    74,    43,    76,    26,
      12,    13,    14,    15,    16,    12,    13,    14,    15,    16,
       4,     5,    37,    12,    13,    14,    15,    16,   105,   106,
      37,    12,    13,    14,    15,    16,   251,   252,    40,    42,
      40,    23,    41,    40,     4,    41,    41,    41,    37,    12,
      13,    14,    15,    16,    44,    30,    37,    12,    13,    14,
      15,    16,    12,    13,    14,    15,    16,     4,     4,    23,
       4,    16,    44,     3,    37,    34,    34,    45,    35,    35,
       4,    24,    37,    36,    36,     4,    37,     4,     4,     4,
       4,    39,    41,    39,     4,    39,     4,    37,    24,    41,
      41,     8,    40,    42,    16,     4,    41,    39,     4,    38,
      38,    38,    45,    38,    23,    38,    38,    38,    38,    37,
      24,     4,    24,    24,    37,     4,     4,    38,     4,    24,
     254,    37,    13,    20,   217,    40,   127,    -1,    -1,    86,
      -1,    44,    36,    -1,    -1,    45,    44,    39,    -1,    -1,
      47,    -1,    -1,    46
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    25,    49,    61,    66,    67,    27,    28,    72,    73,
       0,    29,    31,    62,    32,    57,    68,     4,    74,    75,
       4,    76,    78,    73,     4,     5,     4,    63,    66,    58,
      33,    50,    26,    42,    77,    38,    40,    77,    38,    40,
       5,    41,   104,   104,    40,    41,    79,    84,     4,    34,
      35,    36,    59,    60,    51,     4,    69,   104,    75,    41,
      76,   104,    12,    13,    14,    15,    16,    63,     4,    34,
      35,    36,    80,    81,    84,    30,    79,    85,    89,     4,
      39,    34,    35,    36,    52,    41,    70,    40,    43,   104,
      37,   104,   104,   104,   104,   104,    44,    41,    89,     4,
      89,     4,    90,    91,    92,    23,    23,     4,    39,     4,
      39,     4,    39,     4,    71,    72,   104,    37,     4,    39,
      82,    83,     4,     5,     6,     9,    21,    22,    41,    46,
      94,    96,    97,    98,    99,   100,   102,   103,    86,    44,
       3,    93,    60,    60,    34,    34,    35,    35,    36,    36,
      37,    40,    43,    41,    83,    45,    94,     4,    95,    95,
      94,    98,   100,   102,     7,     8,    10,    11,    37,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    44,    45,
     101,    42,    24,     4,    87,    88,     4,    41,     4,     4,
      39,     4,    39,     4,    39,     4,    39,     4,    96,    37,
      41,    41,    37,    47,    94,    94,    94,    94,   100,   100,
     100,   100,   100,   100,   100,    24,    41,    40,    45,    96,
       4,    38,    39,    38,    38,    53,    38,    38,    54,    38,
      38,    55,    38,    37,   104,   104,    43,     4,     5,    64,
      65,    88,    23,    37,    38,    38,     4,    56,    56,    56,
      83,    40,    40,    37,    40,     4,    24,     4,    38,    38,
      38,   104,   104,    64,    44,     4,    24,    37,    37,     4,
      45,    24,     4,    94,    94,     4,    45,     4,    24,    45,
       4
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    48,    49,    49,    49,    49,    50,    51,    51,    53,
      52,    54,    52,    55,    52,    52,    52,    52,    52,    52,
      52,    56,    56,    56,    56,    57,    58,    58,    59,    59,
      59,    59,    60,    60,    60,    60,    61,    61,    61,    62,
      62,    63,    63,    64,    64,    65,    65,    66,    67,    67,
      68,    68,    69,    69,    70,    71,    71,    72,    72,    73,
      73,    74,    74,    75,    76,    77,    77,    77,    78,    78,
      79,    79,    80,    80,    81,    81,    81,    81,    82,    82,
      83,    83,    83,    83,    84,    84,    86,    85,    87,    87,
      88,    88,    89,    89,    90,    90,    91,    92,    92,    92,
      93,    93,    94,    94,    94,    94,    94,    94,    94,    94,
      94,    94,    95,    96,    97,    97,    98,    99,   100,   100,
     100,   100,   100,   100,   100,   100,   100,   100,   101,   101,
     101,   101,   101,   101,   102,   102,   102,   103,   104,   104,
     104,   104,   104,   104,   104
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     6,     6,     5,     3,     2,     0,     2,     0,
       7,     0,     7,     0,     7,     5,     5,     5,     5,     5,
       5,     1,     5,     2,     6,     2,     0,     2,     7,     6,
       6,     7,     1,     1,     1,     1,     4,     4,     0,     0,
       2,     1,     3,     1,     3,     1,     1,     2,     2,     0,
       0,     5,     1,     0,     3,     1,     3,     1,     2,     3,
       3,     1,     3,     2,     5,     0,     3,     5,     1,     3,
       6,     0,     1,     4,     1,     1,     1,     1,     1,     1,
       1,     4,     2,     5,     0,     2,     0,     4,     1,     3,
       1,     4,     0,     2,     2,     5,     1,    10,     9,     8,
       0,     1,     1,     1,     3,     3,     3,     3,     3,     2,
       8,     8,     1,     1,     1,     1,     4,     3,     3,     3,
       3,     3,     3,     1,     1,     1,     1,     3,     1,     1,
       1,     1,     1,     1,     1,     2,     3,     1,     3,     3,
       3,     3,     3,     3,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

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
#ifndef YYINITDEPTH
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
static YYSIZE_T
yystrlen (const char *yystr)
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
static char *
yystpcpy (char *yydest, const char *yysrc)
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

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
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
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
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

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
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
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

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
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 5:
#line 108 "pdsparse.y" /* yacc.c:1646  */
    {
       	if (!mc_logic_lang_CTL)  
      	{ 	
	    		mc_pds.start_stack= name_stack_empty();
	      	name_stack_push(&mc_pds.start_stack,sharp,fullset);	   	  
	    		mc_pds.start_state = name_create("csinit");
	    		mc_pds.start_bdd = fullset;
				}
      }
#line 1518 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 9:
#line 124 "pdsparse.y" /* yacc.c:1646  */
    {pp_current_label = (yyvsp[-2].name);}
#line 1524 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 11:
#line 125 "pdsparse.y" /* yacc.c:1646  */
    {pp_current_label = 0;}
#line 1530 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 13:
#line 126 "pdsparse.y" /* yacc.c:1646  */
    {pp_current_label = 0;}
#line 1536 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 21:
#line 135 "pdsparse.y" /* yacc.c:1646  */
    {
		 if(pp_current_label)
		 {
  		int propos = (yyvsp[0].name);
  		NameTreeList tmp = mc_atoms;
  
  		while (tmp)
  		{
  			if (tmp->name == propos && !(tmp->valList)) break;
  			tmp = tmp->next;
  		}
  		if (!tmp)
  		{
  			tmp = common_malloc(sizeof(struct NameTreeListStruct));
  			tmp->name = propos;
  			tmp->tree = NULL;
  			tmp->valList =NULL;
  			tmp->length = 0;
  			tmp->next = mc_atoms;
  			mc_atoms = tmp;
  			}
  		name_bal_insert(&(tmp->tree),pp_current_label);		 	
  		}
		}
#line 1565 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 22:
#line 160 "pdsparse.y" /* yacc.c:1646  */
    {
	 if(pp_current_label)
		 {
			char* str=malloc( strlen(name_cstring((yyvsp[-4].name))) + strlen(name_cstring((yyvsp[-2].name))) + strlen(name_cstring((yyvsp[0].name)))+3);
		 	sprintf(str, "%s'%s'%s",name_cstring((yyvsp[-4].name)),name_cstring((yyvsp[-2].name)),name_cstring((yyvsp[0].name)));		
		 		 
  		int propos = name_create(str);
  		NameTreeList tmp = mc_atoms;
      free(str);
  		while (tmp)
  		{
  			if (tmp->name == propos && !(tmp->valList)) break;
  			tmp = tmp->next;
  		}
  		if (!tmp)
  		{
  			tmp = common_malloc(sizeof(struct NameTreeListStruct));
  			tmp->name = propos;
  			tmp->tree = NULL;
  			tmp->valList =NULL;
  			tmp->length = 0;
  			tmp->next = mc_atoms;
  			mc_atoms = tmp;
  			}
  		name_bal_insert(&(tmp->tree),pp_current_label);		 	
  		}
		}
#line 1597 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 23:
#line 188 "pdsparse.y" /* yacc.c:1646  */
    {
		 if(pp_current_label)
		 {
  		int propos = (yyvsp[0].name);
  		NameTreeList tmp = mc_atoms;
  
  		while (tmp)
  		{
  			if (tmp->name == propos && !(tmp->valList)) break;
  			tmp = tmp->next;
  		}
  		if (!tmp)
  		{
  			tmp = common_malloc(sizeof(struct NameTreeListStruct));
  			tmp->name = propos;
  			tmp->tree = NULL;
  			tmp->valList =NULL;
  			tmp->length = 0;
  			tmp->next = mc_atoms;
  			mc_atoms = tmp;
  			}
  		name_bal_insert(&(tmp->tree),pp_current_label);		 	
  		}
		}
#line 1626 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 24:
#line 213 "pdsparse.y" /* yacc.c:1646  */
    { 
		 if(pp_current_label)
		 {
		  char* str=malloc( strlen(name_cstring((yyvsp[-4].name)))+strlen(name_cstring((yyvsp[-2].name)))+strlen(name_cstring((yyvsp[0].name)))+3);
		  sprintf(str, "%s'%s'%s",name_cstring((yyvsp[-4].name)),name_cstring((yyvsp[-2].name)),name_cstring((yyvsp[0].name)));		 
  		int propos = name_create(str);
  		free(str);
  		NameTreeList tmp = mc_atoms;
  
  		while (tmp)
  		{
  			if (tmp->name == propos && !(tmp->valList)) break;
  			tmp = tmp->next;
  		}
  		if (!tmp)
  		{
  			tmp = common_malloc(sizeof(struct NameTreeListStruct));
  			tmp->name = propos;
  			tmp->tree = NULL;
  			tmp->valList =NULL;
  			tmp->length = 0;
  			tmp->next = mc_atoms;
  			mc_atoms = tmp;
  			}
  		name_bal_insert(&(tmp->tree),pp_current_label);		 	
  		}
		}
#line 1658 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 28:
#line 247 "pdsparse.y" /* yacc.c:1646  */
    { 
		if((mc_logic_lang_CTL))
		{
			ABPDSInsertTransition(&mc_abpds,(yyvsp[-6].name), ctlTree, (yyvsp[-5].name), (yyvsp[-3].name), (yyvsp[-2].name), (yyvsp[-1].name));
    	ABPDSInsert2StackList(&(mc_abpds.stackList), (yyvsp[-5].name));
    	ABPDSInsert2StackList(&(mc_abpds.stackList), (yyvsp[-2].name));
    	ABPDSInsert2StackList(&(mc_abpds.stackList), (yyvsp[-1].name));
			ABPDSInsert2StateList(&(mc_abpds.ctrlList), (yyvsp[-6].name));
			ABPDSInsert2StateList(&(mc_abpds.ctrlList), (yyvsp[-3].name));	
		}
		else
		 	pds_insert_trans(&mc_pds, (yyvsp[-6].name), (yyvsp[-5].name), (yyvsp[-3].name), (yyvsp[-2].name), (yyvsp[-1].name), fullset, 0, 0);	
	  }
#line 1676 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 29:
#line 261 "pdsparse.y" /* yacc.c:1646  */
    { 
	  if(name_create("_")!=(yyvsp[-1].name))
		{
  		if(mc_logic_lang_CTL)
  		{
  			ABPDSInsertTransition(&mc_abpds,(yyvsp[-5].name), ctlTree, (yyvsp[-4].name), (yyvsp[-2].name), (yyvsp[-1].name), 0);
      	ABPDSInsert2StackList(&(mc_abpds.stackList), (yyvsp[-1].name));
  			ABPDSInsert2StackList(&(mc_abpds.stackList), (yyvsp[-4].name));
    		ABPDSInsert2StateList(&(mc_abpds.ctrlList), (yyvsp[-5].name));
  			ABPDSInsert2StateList(&(mc_abpds.ctrlList), (yyvsp[-2].name));	      	
      }
      else               
        pds_insert_trans(&mc_pds, (yyvsp[-5].name), (yyvsp[-4].name), (yyvsp[-2].name), (yyvsp[-1].name), 0, fullset, 0, 0);	
  	}
  	else
  	{
  	  if(mc_logic_lang_CTL)
  	  {
  	    	ABPDSInsert2StackList(&(mc_abpds.stackList), (yyvsp[-4].name));
    			ABPDSInsert2StateList(&(mc_abpds.ctrlList), (yyvsp[-5].name));
  				ABPDSInsert2StateList(&(mc_abpds.ctrlList), (yyvsp[-2].name));	
  		   	ABPDSInsertTransition(&mc_abpds,(yyvsp[-5].name), ctlTree, (yyvsp[-4].name), (yyvsp[-2].name), 0, 0); 	
  		}
  		else pds_insert_trans(&mc_pds, (yyvsp[-5].name), (yyvsp[-4].name), (yyvsp[-2].name), 0, 0, fullset, 0, 0);
  	}	

	}
#line 1708 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 30:
#line 289 "pdsparse.y" /* yacc.c:1646  */
    {
	  if(mc_logic_lang_CTL)
	  {
   	ABPDSInsertTransition(&mc_abpds,(yyvsp[-5].name), ctlTree, sharp, (yyvsp[-2].name), sharp, 0); 		
    ABPDSInsert2StateList(&(mc_abpds.ctrlList), (yyvsp[-5].name));
  	ABPDSInsert2StateList(&(mc_abpds.ctrlList), (yyvsp[-2].name));	
  	}
  	else pds_insert_trans(&mc_pds, (yyvsp[-5].name), sharp, (yyvsp[-2].name), sharp, 0, fullset, 0, 0);
	}
#line 1722 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 31:
#line 299 "pdsparse.y" /* yacc.c:1646  */
    {
	  if(mc_logic_lang_CTL)
{   	ABPDSInsertTransition(&mc_abpds,(yyvsp[-6].name), ctlTree, sharp, (yyvsp[-3].name), (yyvsp[-2].name), sharp); 		
    ABPDSInsert2StateList(&(mc_abpds.ctrlList), (yyvsp[-6].name));
  	ABPDSInsert2StateList(&(mc_abpds.ctrlList), (yyvsp[-3].name));
  	ABPDSInsert2StackList(&(mc_abpds.stackList), (yyvsp[-2].name));	
  	}
  	else pds_insert_trans(&mc_pds, (yyvsp[-6].name), sharp, (yyvsp[-3].name), (yyvsp[-2].name), 0, fullset, 0, 0);
	}
#line 1736 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 32:
#line 309 "pdsparse.y" /* yacc.c:1646  */
    {(yyval.name)=(yyvsp[0].name);}
#line 1742 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 33:
#line 310 "pdsparse.y" /* yacc.c:1646  */
    {(yyval.name)=name_create("csq");}
#line 1748 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 34:
#line 311 "pdsparse.y" /* yacc.c:1646  */
    {(yyval.name)=name_create("csinit");}
#line 1754 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 35:
#line 312 "pdsparse.y" /* yacc.c:1646  */
    {(yyval.name)=name_create("csend");}
#line 1760 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 36:
#line 315 "pdsparse.y" /* yacc.c:1646  */
    {
		const_t con = common_malloc(sizeof(struct const_element));
		con->name = (yyvsp[-1].name);
		con->value = (yyvsp[0].integer);
		con->next = constlist;
		constlist = con;
	  }
#line 1772 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 41:
#line 329 "pdsparse.y" /* yacc.c:1646  */
    {
		abpdsValList = common_malloc(sizeof(struct ValueListStruct));
		abpdsValList->name =(yyvsp[0].name);
		abpdsValList->next = NULL;
		abpdsValNum++;
  }
#line 1783 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 42:
#line 336 "pdsparse.y" /* yacc.c:1646  */
    {
		ValList tmp;
		for(tmp=abpdsValList; tmp; tmp=tmp->next)
		{
			if(tmp->name==(yyvsp[-2].name)) break;
		}
		if(!tmp)
		{	
			tmp = common_malloc(sizeof(struct ValueListStruct));
			tmp->name =(yyvsp[-2].name);
			tmp->next = abpdsValList;
			abpdsValNum++;
			abpdsValList = tmp;
		}
	}
#line 1803 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 43:
#line 352 "pdsparse.y" /* yacc.c:1646  */
    {
  	length++;
		(yyval.valList) = createVal((yyvsp[0].name),NULL);
  }
#line 1812 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 44:
#line 357 "pdsparse.y" /* yacc.c:1646  */
    {
		(yyval.valList) = createVal((yyvsp[-2].name),(yyvsp[0].valList));
		length++;
	}
#line 1821 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 45:
#line 363 "pdsparse.y" /* yacc.c:1646  */
    {
      (yyval.name) =(yyvsp[0].name);
    }
#line 1829 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 46:
#line 367 "pdsparse.y" /* yacc.c:1646  */
    {
    	char* tmp =malloc(sizeof(int)+ 1);
      sprintf(tmp,"%d", (yyvsp[0].integer));
      (yyval.name)=name_create(tmp);
      free(tmp);
    }
#line 1840 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 47:
#line 375 "pdsparse.y" /* yacc.c:1646  */
    {
		block_t tmp;
		mc_blocks = common_malloc(sizeof(struct block_element));
		mc_blocks->procname = 0;
		mc_blocks->names = NULL;
		mc_blocks->idents = (yyvsp[-1].ident);
		mc_blocks->next = mc_maxblock = tmp = (yyvsp[0].block);

		global_vars = (yyvsp[-1].ident)? (yyvsp[-1].ident)->index + ((yyvsp[-1].ident)->type? (yyvsp[-1].ident)->type : 1)
					    * ((yyvsp[-1].ident)->dim? (yyvsp[-1].ident)->dim : 1) : 0;
		local_vars = 0;
		while (tmp)
		{
			ident_t id = tmp->idents;
			int x = id? id->index + (id->type? id->type : 1)
						* (id->dim? id->dim : 1) : 0;
			if (x>local_vars) { mc_maxblock = tmp; local_vars = x; }
			tmp = tmp->next;
		}

		for (tmp = mc_blocks; tmp; tmp = tmp->next)
		{
			ident_t id = tmp->idents, nl = NULL, tmpid;
			
			while (id)
			{
				tmpid = id->next;
				id->next = nl;
				nl = id;
				id = tmpid;
			}
			tmp->idents = nl;
		}

		pds_p1 = pds_y1 = pds_p2 = pds_y2 = pds_y3 = 0;
		idchain = NULL;
		init_cudd();

		for (tmp = mc_blocks; tmp; tmp = tmp->next)
			build_properties(tmp);
	  }
#line 1886 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 48:
#line 417 "pdsparse.y" /* yacc.c:1646  */
    { (yyval.ident) = (yyvsp[0].ident);   }
#line 1892 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 49:
#line 418 "pdsparse.y" /* yacc.c:1646  */
    { (yyval.ident) = NULL; }
#line 1898 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 50:
#line 420 "pdsparse.y" /* yacc.c:1646  */
    { (yyval.block) = NULL; local_vars = 0; }
#line 1904 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 51:
#line 422 "pdsparse.y" /* yacc.c:1646  */
    {
		NameSet set;
		(yyval.block) = common_malloc(sizeof(struct block_element));
		(yyval.block)->procname = (yyvsp[-2].name);
		(yyval.block)->names = (yyvsp[-1].set);
		(yyval.block)->idents = (yyvsp[0].ident);
		(yyval.block)->next = (yyvsp[-4].block);
		for (set = (yyval.block)->names; set; set = set->next)
			name_tree_insert(&mc_localtree,
				name_tree_mknode(set->name,0,(yyval.block)));
		local_vars = 0;
	  }
#line 1921 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 53:
#line 436 "pdsparse.y" /* yacc.c:1646  */
    { (yyval.name) = 0; }
#line 1927 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 54:
#line 439 "pdsparse.y" /* yacc.c:1646  */
    { (yyval.set) = (yyvsp[-1].set); }
#line 1933 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 55:
#line 442 "pdsparse.y" /* yacc.c:1646  */
    {
		NameSet set = NULL;
		name_set_insert(&set,(yyvsp[0].name));
		(yyval.set) = set;
	  }
#line 1943 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 56:
#line 448 "pdsparse.y" /* yacc.c:1646  */
    {
		name_set_insert(&(yyvsp[-2].set),(yyvsp[0].name));
		(yyval.set) = (yyvsp[-2].set);
	  }
#line 1952 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 58:
#line 455 "pdsparse.y" /* yacc.c:1646  */
    {
		(yyval.ident) = (yyvsp[0].ident);
		while ((yyvsp[0].ident)->next) (yyvsp[0].ident) = (yyvsp[0].ident)->next;
		(yyvsp[0].ident)->next = (yyvsp[-1].ident);
	  }
#line 1962 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 59:
#line 461 "pdsparse.y" /* yacc.c:1646  */
    { (yyval.ident) = (yyvsp[-1].ident); }
#line 1968 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 60:
#line 462 "pdsparse.y" /* yacc.c:1646  */
    { (yyval.ident) = (yyvsp[-1].ident); }
#line 1974 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 61:
#line 464 "pdsparse.y" /* yacc.c:1646  */
    { ((yyval.ident) = (yyvsp[0].ident))->next = NULL; }
#line 1980 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 62:
#line 465 "pdsparse.y" /* yacc.c:1646  */
    { ((yyval.ident) = (yyvsp[0].ident))->next = (yyvsp[-2].ident); }
#line 1986 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 63:
#line 468 "pdsparse.y" /* yacc.c:1646  */
    {
		(yyval.ident) = (yyvsp[0].ident);
		(yyval.ident)->varname = (yyvsp[-1].name);
		(yyval.ident)->type = 0;
		(yyval.ident)->index = local_vars;
		local_vars += (yyval.ident)->dim? (yyval.ident)->dim : 1;
	   }
#line 1998 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 64:
#line 477 "pdsparse.y" /* yacc.c:1646  */
    {
		(yyval.ident) = (yyvsp[-3].ident);
		(yyval.ident)->varname = (yyvsp[-4].name);
		if ((yyvsp[-1].integer) <= 0) pdserror("positive integer expected");
		(yyval.ident)->index = local_vars;
		local_vars += ((yyval.ident)->type = (yyvsp[-1].integer)) * ((yyval.ident)->dim? (yyval.ident)->dim : 1);
	  }
#line 2010 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 65:
#line 485 "pdsparse.y" /* yacc.c:1646  */
    {
		((yyval.ident) = common_malloc(sizeof(struct ident_element)))->dim = 0; 
	  }
#line 2018 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 66:
#line 489 "pdsparse.y" /* yacc.c:1646  */
    {
		if ((yyvsp[-1].integer) <= 0) pdserror("positive integer expected");
		(yyval.ident) = common_malloc(sizeof(struct ident_element));
		(yyval.ident)->dim = (yyvsp[-1].integer); (yyval.ident)->lo = 0; (yyval.ident)->hi = (yyvsp[-1].integer)-1;
	  }
#line 2028 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 67:
#line 495 "pdsparse.y" /* yacc.c:1646  */
    {
		if ((yyvsp[-3].integer) < 0) pdserror("nonnegative integer expected");
		(yyval.ident) = common_malloc(sizeof(struct ident_element));
		(yyval.ident)->dim = (yyvsp[-1].integer)-(yyvsp[-3].integer)+1; (yyval.ident)->lo = (yyvsp[-3].integer); (yyval.ident)->hi = (yyvsp[-1].integer);
	  }
#line 2038 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 68:
#line 501 "pdsparse.y" /* yacc.c:1646  */
    { ((yyval.ident) = (yyvsp[0].ident))->next = NULL; }
#line 2044 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 69:
#line 502 "pdsparse.y" /* yacc.c:1646  */
    { ((yyval.ident) = (yyvsp[0].ident))->next = (yyvsp[-2].ident); }
#line 2050 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 70:
#line 507 "pdsparse.y" /* yacc.c:1646  */
    {
	  	if (mc_logic_lang_CTL)
	  	{
	  		mc_abpds.startStack = (yyvsp[-2].stack);
	  	}
	  	else
	  	{
				mc_pds.start_stack = (yyvsp[-2].stack);
			}
	  }
#line 2065 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 71:
#line 518 "pdsparse.y" /* yacc.c:1646  */
    {
	    mc_pds.start_stack =0;
	    mc_abpds.startState=0;
	    mc_abpds.startStack=0;
	    mc_pds.start_state = 0; 
	  }
#line 2076 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 72:
#line 528 "pdsparse.y" /* yacc.c:1646  */
    {
	  	if (mc_logic_lang_CTL)
	  	{
	  		mc_abpds.startState = (yyvsp[0].name);
	  	}
	  	else
	  	{
				mc_pds.start_state = (yyvsp[0].name);
				mc_pds.start_bdd = fullset;
	  	}
	  }
#line 2092 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 73:
#line 540 "pdsparse.y" /* yacc.c:1646  */
    {
	  	if (mc_logic_lang_CTL)
	  	{
	  		mc_abpds.startState = (yyvsp[-3].name);
	  	}
	  	else
	  	{
				mc_pds.start_state = (yyvsp[-3].name);
				mc_pds.start_bdd = (yyvsp[-1].bdd);
			}
	  }
#line 2108 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 74:
#line 554 "pdsparse.y" /* yacc.c:1646  */
    {
	  if (mc_logic_lang_CTL) ABPDSInsert2StateList(&(mc_abpds.ctrlList), (yyvsp[0].name));
		pds_p1 = (yyval.name) = (yyvsp[0].name);
		pds_y1 = pds_p2 = pds_y2 = pds_y3 = 0;
	    }
#line 2118 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 75:
#line 560 "pdsparse.y" /* yacc.c:1646  */
    {pds_p1= (yyval.name)=name_create("csinit");
		pds_y1 = pds_p2 = pds_y2 = pds_y3 = 0; ABPDSInsert2StateList(&(mc_abpds.ctrlList), (yyval.name));}
#line 2125 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 76:
#line 563 "pdsparse.y" /* yacc.c:1646  */
    {pds_p1= (yyval.name)=name_create("csq");
		pds_y1 = pds_p2 = pds_y2 = pds_y3 = 0; ABPDSInsert2StateList(&(mc_abpds.ctrlList), (yyval.name));}
#line 2132 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 77:
#line 566 "pdsparse.y" /* yacc.c:1646  */
    {pds_p1= (yyval.name)=name_create("csend");
		pds_y1 = pds_p2 = pds_y2 = pds_y3 = 0; ABPDSInsert2StateList(&(mc_abpds.ctrlList), (yyval.name));}
#line 2139 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 78:
#line 571 "pdsparse.y" /* yacc.c:1646  */
    {
    if (mc_logic_lang_CTL) ABPDSInsert2StackList(&(mc_abpds.stackList), (yyvsp[0].name));
		pds_y1 = (yyval.name) = (yyvsp[0].name);
		pds_p1 = pds_p2 = pds_y2 = pds_y3 = 0;
	    }
#line 2149 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 79:
#line 578 "pdsparse.y" /* yacc.c:1646  */
    {
	  pds_y1 = (yyval.name) = sharp;		
		pds_p1 = pds_p2 = pds_y2 = pds_y3 = 0;	    
	  }
#line 2158 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 80:
#line 585 "pdsparse.y" /* yacc.c:1646  */
    {
	    (yyval.stack) = name_stack_empty();
	    if (mc_logic_lang_CTL)  
	    {	
	    	name_stack_push(&(yyval.stack),sharp,NULL);
	    	name_stack_push(&(yyval.stack),(yyvsp[0].name),NULL);
	    }
	    else name_stack_push(&(yyval.stack),(yyvsp[0].name),fullset);
	  }
#line 2172 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 81:
#line 595 "pdsparse.y" /* yacc.c:1646  */
    {
	    (yyval.stack) = name_stack_empty();
	  	if (mc_logic_lang_CTL)  name_stack_push(&(yyval.stack),(yyvsp[-3].name),NULL);
	  	else name_stack_push(&(yyval.stack),(yyvsp[-3].name),(yyvsp[-1].bdd));
	  }
#line 2182 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 82:
#line 601 "pdsparse.y" /* yacc.c:1646  */
    {
	  	if (mc_logic_lang_CTL) name_stack_push(&(yyvsp[0].stack),(yyvsp[-1].name),NULL);
	  	else name_stack_push(&(yyvsp[0].stack),(yyvsp[-1].name),fullset);
	  	(yyval.stack)=(yyvsp[0].stack);
	  }
#line 2192 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 83:
#line 607 "pdsparse.y" /* yacc.c:1646  */
    {
	  	if (mc_logic_lang_CTL) name_stack_push(&(yyvsp[0].stack),(yyvsp[-4].name),NULL);
	  	else name_stack_push(&(yyvsp[0].stack),(yyvsp[-4].name),(yyvsp[-2].bdd));
	  	(yyval.stack)=(yyvsp[0].stack);
	  }
#line 2202 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 86:
#line 617 "pdsparse.y" /* yacc.c:1646  */
    { pp_current_label = (yyvsp[0].name); }
#line 2208 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 90:
#line 623 "pdsparse.y" /* yacc.c:1646  */
    {
		int propos = (yyvsp[0].name);
		NameTreeList tmp = mc_atoms;

		while (tmp)
		{
			if (tmp->name == propos && !(tmp->valList)) break;
			tmp = tmp->next;
		}
		if (!tmp)
		{
			tmp = common_malloc(sizeof(struct NameTreeListStruct));
			tmp->name = propos;
			tmp->tree = NULL;
			tmp->valList =NULL;
			tmp->length = 0;
			tmp->next = mc_atoms;
			mc_atoms = tmp;
		}
		name_bal_insert(&(tmp->tree),pp_current_label);
	}
#line 2234 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 91:
#line 645 "pdsparse.y" /* yacc.c:1646  */
    {
		if(!(yyvsp[-1].valList)) pdserror("The value of atomic proposition with parentheses can no be empty!");
		int propos = (yyvsp[-3].name);
		NameTreeList tmp = mc_atoms;

		while (tmp)
		{
			if (tmp->name == propos && compareVal(tmp->valList,(yyvsp[-1].valList)))
			{
				deleteVal((yyvsp[-1].valList));
				break;
			}
			tmp = tmp->next;
		}
		if (!tmp)
		{
			tmp = common_malloc(sizeof(struct NameTreeListStruct));
			tmp->name = propos;
			tmp->tree = NULL;
			tmp->valList =(yyvsp[-1].valList);
			tmp->length = length;
			tmp->next = mc_atoms;
			mc_atoms = tmp;
		}
		length=0;
		name_bal_insert(&(tmp->tree),pp_current_label);
	}
#line 2266 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 92:
#line 674 "pdsparse.y" /* yacc.c:1646  */
    { 
		if(mc_CTPL) init_CTL_BDD_Manager(); 
	}
#line 2274 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 94:
#line 681 "pdsparse.y" /* yacc.c:1646  */
    {
	  	if (mc_logic_lang_CTL)
	  	{
	  		ABPDSInsertTransition(&mc_abpds,pds_p1,ctlTree, pds_y1, pds_p2, pds_y2, pds_y3);
	  	}
	  	else
	  	{
				pds_insert_trans(&mc_pds, pds_p1, pds_y1, pds_p2, pds_y2, pds_y3, fullset, (yyvsp[0].string), 0);
	  	}
	  	if ((yyvsp[0].string)) free((yyvsp[0].string));
	  }
#line 2290 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 95:
#line 693 "pdsparse.y" /* yacc.c:1646  */
    {
	  	if (mc_logic_lang_CTL)
	  	{
	  		ABPDSInsertTransition(&mc_abpds,pds_p1, ctlTree, pds_y1, pds_p2, pds_y2, pds_y3);
	  	}
	  	else
	  	{
				pds_insert_trans(&mc_pds, pds_p1, pds_y1, pds_p2, pds_y2, pds_y3, (yyvsp[-1].bdd), (yyvsp[-3].string), 0);
				Cudd_RecursiveDeref(mc_mgr,(yyvsp[-1].bdd));
			}
			if ((yyvsp[-3].string)) free((yyvsp[-3].string));
	  }
#line 2307 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 96:
#line 708 "pdsparse.y" /* yacc.c:1646  */
    {
		if (mc_verbose > 1) fprintf(stdout,"-## %s %s %s %s %s\n",
			name_cstring(pds_p1),name_cstring(pds_y1),
			name_cstring(pds_p2),name_cstring(pds_y2),
			name_cstring(pds_y3));
	  }
#line 2318 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 97:
#line 717 "pdsparse.y" /* yacc.c:1646  */
    { 
		pds_p1 = (yyvsp[-9].name); pds_y1 = (yyvsp[-7].name);
		pds_p2 = (yyvsp[-4].name); pds_y2 = (yyvsp[-2].name);
		pds_y3 = (yyvsp[-1].name);
    if(mc_logic_lang_CTL)
    {
    	ABPDSInsert2StackList(&(mc_abpds.stackList), (yyvsp[-7].name));
    	ABPDSInsert2StackList(&(mc_abpds.stackList), (yyvsp[-2].name));
    	ABPDSInsert2StackList(&(mc_abpds.stackList), (yyvsp[-1].name));
			ABPDSInsert2StateList(&(mc_abpds.ctrlList), (yyvsp[-9].name));
			ABPDSInsert2StateList(&(mc_abpds.ctrlList), (yyvsp[-4].name));		
	  }
	 }
#line 2336 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 98:
#line 731 "pdsparse.y" /* yacc.c:1646  */
    {
			pds_p1 = (yyvsp[-8].name); pds_y1 = (yyvsp[-6].name);
			pds_p2 = (yyvsp[-3].name); pds_y2 = (yyvsp[-1].name); pds_y3 = 0;
    if(mc_logic_lang_CTL)
    {
    	ABPDSInsert2StackList(&(mc_abpds.stackList), (yyvsp[-6].name));
    	ABPDSInsert2StackList(&(mc_abpds.stackList), (yyvsp[-1].name));
			ABPDSInsert2StateList(&(mc_abpds.ctrlList), (yyvsp[-8].name));
			ABPDSInsert2StateList(&(mc_abpds.ctrlList), (yyvsp[-3].name));	
	  }
	 }
#line 2352 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 99:
#line 743 "pdsparse.y" /* yacc.c:1646  */
    { 
		pds_p1 = (yyvsp[-7].name); pds_y1 = (yyvsp[-5].name);
		pds_p2 = (yyvsp[-2].name); pds_y2 = pds_y3 = 0;
		if(mc_logic_lang_CTL)
		{ABPDSInsert2StackList(&(mc_abpds.stackList), (yyvsp[-5].name));
    ABPDSInsert2StateList(&(mc_abpds.ctrlList), (yyvsp[-7].name));
		ABPDSInsert2StateList(&(mc_abpds.ctrlList), (yyvsp[-2].name));	
	  }}
#line 2365 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 100:
#line 752 "pdsparse.y" /* yacc.c:1646  */
    { (yyval.string) = NULL; }
#line 2371 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 104:
#line 757 "pdsparse.y" /* yacc.c:1646  */
    { (yyval.etree) = (yyvsp[-1].etree); }
#line 2377 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 105:
#line 758 "pdsparse.y" /* yacc.c:1646  */
    { (yyval.etree) = etree_create(E_OR,0,(yyvsp[-2].etree),(yyvsp[0].etree)); }
#line 2383 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 106:
#line 759 "pdsparse.y" /* yacc.c:1646  */
    { (yyval.etree) = etree_create(E_AND,0,(yyvsp[-2].etree),(yyvsp[0].etree)); }
#line 2389 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 107:
#line 760 "pdsparse.y" /* yacc.c:1646  */
    { (yyval.etree) = etree_create(E_EQV,0,(yyvsp[-2].etree),(yyvsp[0].etree)); }
#line 2395 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 108:
#line 761 "pdsparse.y" /* yacc.c:1646  */
    { (yyval.etree) = etree_create(E_XOR,0,(yyvsp[-2].etree),(yyvsp[0].etree)); }
#line 2401 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 109:
#line 762 "pdsparse.y" /* yacc.c:1646  */
    { (yyval.etree) = etree_create(E_NOT,0,(yyvsp[0].etree),NULL); }
#line 2407 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 110:
#line 764 "pdsparse.y" /* yacc.c:1646  */
    {
		quantlist->lower = (yyvsp[-4].integer);
		quantlist->upper = (yyvsp[-2].integer);
		(yyval.etree) = etree_create(E_FORALL,(identref_t)quantlist,(yyvsp[0].etree),NULL);
		quantlist = quantlist->next;
	  }
#line 2418 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 111:
#line 771 "pdsparse.y" /* yacc.c:1646  */
    {
		quantlist->lower = (yyvsp[-4].integer);
		quantlist->upper = (yyvsp[-2].integer);
		(yyval.etree) = etree_create(E_EXISTS,(identref_t)quantlist,(yyvsp[0].etree),NULL);
		quantlist = quantlist->next;
	  }
#line 2429 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 112:
#line 779 "pdsparse.y" /* yacc.c:1646  */
    {
		const_t q;
		q = common_malloc(sizeof(struct const_element));
		q->name = (yyvsp[0].name);
		q->next = quantlist;
		quantlist = q;
	  }
#line 2441 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 113:
#line 788 "pdsparse.y" /* yacc.c:1646  */
    {
		(yyval.bdd) = build_expr((yyvsp[0].etree));
		free_expr_tree((yyvsp[0].etree));
	  }
#line 2450 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 114:
#line 794 "pdsparse.y" /* yacc.c:1646  */
    {
		if ((yyvsp[0].identref)->ident->type) pdserror("boolean variable expected");
		if ((yyvsp[0].identref)->ident->dim) pdserror("missing subscript");
		(yyval.etree) = etree_create(E_BVAR,(yyvsp[0].identref),NULL,NULL);
	  }
#line 2460 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 115:
#line 800 "pdsparse.y" /* yacc.c:1646  */
    {
		if ((yyvsp[0].etree)->idref->ident->type)
			pdserror("boolean variable expected");
		(yyval.etree) = (yyvsp[0].etree);
		(yyvsp[0].etree)->token = E_BVAR;
		(yyvsp[0].etree)->left = (FormulaTree) idchain;
		idchain = NULL;
	  }
#line 2473 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 116:
#line 810 "pdsparse.y" /* yacc.c:1646  */
    {
		if (!(yyvsp[-3].identref)->ident->dim) pdserror("unexpected subscript");
		(yyval.etree) = etree_create(0,(yyvsp[-3].identref),NULL,(yyvsp[-1].etree));
	  }
#line 2482 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 117:
#line 816 "pdsparse.y" /* yacc.c:1646  */
    {
		FormulaTree tree = etree_create((yyvsp[-1].integer),0,(yyvsp[-2].etree),(yyvsp[0].etree));
		(yyval.etree) = etree_create(E_IEXPR,0,idchain,tree);
		idchain = NULL;
	  }
#line 2492 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 118:
#line 822 "pdsparse.y" /* yacc.c:1646  */
    { (yyval.etree) = etree_create(E_PLUS,0,(yyvsp[-2].etree),(yyvsp[0].etree)); }
#line 2498 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 119:
#line 823 "pdsparse.y" /* yacc.c:1646  */
    { (yyval.etree) = etree_create(E_MINUS,0,(yyvsp[-2].etree),(yyvsp[0].etree)); }
#line 2504 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 120:
#line 824 "pdsparse.y" /* yacc.c:1646  */
    { (yyval.etree) = etree_create(E_MULT,0,(yyvsp[-2].etree),(yyvsp[0].etree)); }
#line 2510 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 121:
#line 825 "pdsparse.y" /* yacc.c:1646  */
    { (yyval.etree) = etree_create(E_DIV,0,(yyvsp[-2].etree),(yyvsp[0].etree)); }
#line 2516 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 122:
#line 826 "pdsparse.y" /* yacc.c:1646  */
    { (yyval.etree) = etree_create(E_SHL,0,(yyvsp[-2].etree),(yyvsp[0].etree)); }
#line 2522 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 123:
#line 828 "pdsparse.y" /* yacc.c:1646  */
    {
		if (!(yyvsp[0].identref)->ident->type) pdserror("integer variable expected");
		(yyvsp[0].identref) = add_to_idchain((yyvsp[0].identref));
		(yyval.etree) = etree_create(E_IDENT,(yyvsp[0].identref),NULL,NULL);
		if ((yyvsp[0].identref)->ident->dim) pdserror("missing subscript");
	  }
#line 2533 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 124:
#line 835 "pdsparse.y" /* yacc.c:1646  */
    {
		if (!(yyvsp[0].etree)->idref->ident->type)
			pdserror("integer variable expected");
		(yyval.etree) = (yyvsp[0].etree);
		(yyval.etree)->token = E_IDENT;
		(yyvsp[0].etree)->idref->sub = (yyvsp[0].etree)->right;
		(yyvsp[0].etree)->idref->next = idchain;
		idchain = (yyvsp[0].etree)->idref;
		(yyvsp[0].etree)->right = NULL;
	  }
#line 2548 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 125:
#line 845 "pdsparse.y" /* yacc.c:1646  */
    { (yyval.etree) = etree_create(E_QUANT,(identref_t)(yyvsp[0].quant),NULL,NULL); }
#line 2554 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 126:
#line 846 "pdsparse.y" /* yacc.c:1646  */
    { (yyval.etree) = etree_create(E_NUMBER,(identref_t)(yyvsp[0].integer),NULL,NULL); }
#line 2560 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 127:
#line 847 "pdsparse.y" /* yacc.c:1646  */
    { (yyval.etree) = (yyvsp[-1].etree); }
#line 2566 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 128:
#line 849 "pdsparse.y" /* yacc.c:1646  */
    { (yyval.integer) = E_LT; }
#line 2572 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 129:
#line 850 "pdsparse.y" /* yacc.c:1646  */
    { (yyval.integer) = E_LE; }
#line 2578 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 130:
#line 851 "pdsparse.y" /* yacc.c:1646  */
    { (yyval.integer) = E_EQ; }
#line 2584 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 131:
#line 852 "pdsparse.y" /* yacc.c:1646  */
    { (yyval.integer) = E_NE; }
#line 2590 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 132:
#line 853 "pdsparse.y" /* yacc.c:1646  */
    { (yyval.integer) = E_GE; }
#line 2596 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 133:
#line 854 "pdsparse.y" /* yacc.c:1646  */
    { (yyval.integer) = E_GT; }
#line 2602 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 134:
#line 857 "pdsparse.y" /* yacc.c:1646  */
    {				/* in init cfg: (x,y) -> (p,y) */
		if (pds_p1 * pds_y1 == 0)
		    (yyval.identref) = find_ident(pds_y1, (yyvsp[0].name), pds_p1? pvars : NULL, yvars);
		else
		    (yyval.identref) = find_ident(pds_y1, (yyvsp[0].name), ppvars, ypvars);
		if (!(yyval.identref)) pdserror("invalid identifier");
	  }
#line 2614 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 135:
#line 865 "pdsparse.y" /* yacc.c:1646  */
    {
		if (pds_p1 * pds_y1 == 0) pdserror("invalid identifier");
		(yyval.identref) = find_ident(pds_y2, (yyvsp[-1].name), pvars, yvars);
		if (!(yyval.identref)) pdserror("invalid identifier");
	  }
#line 2624 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 136:
#line 871 "pdsparse.y" /* yacc.c:1646  */
    {
		(yyval.identref) = find_ident(mc_restrict? 0 : pds_y3, (yyvsp[-2].name), NULL, yppvars);
		if (!(yyval.identref)) pdserror("invalid identifier");
	  }
#line 2633 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 138:
#line 878 "pdsparse.y" /* yacc.c:1646  */
    { (yyval.integer) = (yyvsp[-2].integer) + (yyvsp[0].integer); }
#line 2639 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 139:
#line 879 "pdsparse.y" /* yacc.c:1646  */
    { (yyval.integer) = (yyvsp[-2].integer) - (yyvsp[0].integer); }
#line 2645 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 140:
#line 880 "pdsparse.y" /* yacc.c:1646  */
    { (yyval.integer) = (yyvsp[-2].integer) * (yyvsp[0].integer); }
#line 2651 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 141:
#line 881 "pdsparse.y" /* yacc.c:1646  */
    { (yyval.integer) = (yyvsp[-2].integer) / (yyvsp[0].integer); }
#line 2657 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 142:
#line 882 "pdsparse.y" /* yacc.c:1646  */
    { (yyval.integer) = (yyvsp[-2].integer) << (yyvsp[0].integer); }
#line 2663 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 143:
#line 883 "pdsparse.y" /* yacc.c:1646  */
    { (yyval.integer) = (yyvsp[-1].integer); }
#line 2669 "pdsparse.c" /* yacc.c:1646  */
    break;

  case 144:
#line 884 "pdsparse.y" /* yacc.c:1646  */
    { (yyval.integer) = (yyvsp[0].integer); }
#line 2675 "pdsparse.c" /* yacc.c:1646  */
    break;


#line 2679 "pdsparse.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
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

  /* Else will try to reuse lookahead token after shifting the error
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

  /* Do not reclaim the symbols of the rule whose action triggered
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
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
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

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


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

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
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
  return yyresult;
}
#line 886 "pdsparse.y" /* yacc.c:1906  */


#include "pdslex.c"

void pdserror (char *info)
{ 
	common_error("syntax error in pushdown system: %s, line %d",
			info,pdslineno);
}

void parse_pds (char *filename)
{
	mc_pds = pds_empty();
	quantlist = NULL;
	mc_localtree = NULL;
	mc_atoms = NULL;

	if (!(pdsin = fopen(filename,"r")))
		common_error("modelfile %s not found", filename);

	if (pdsparse())
		common_error("parse errors encountered while reading pds: token: %s", pdstext);

	fclose(pdsin);
}

void build_abpds(char *filename)
{
  abpdsValList = NULL;
  abpdsValNum =0;
	quantlist = NULL;
	mc_localtree = NULL;
	mc_atoms = NULL;

  mc_abpds = ABPDSInitial();
  sharp=sharp? sharp: name_create("sharp");
  
	if (!(pdsin = fopen(filename,"r")))
		common_error("modelfile %s not found", filename);

	if (pdsparse())
		common_error("parse errors encountered while reading pds");
	fclose(pdsin);  
	 
//	if(pdslineno>50000) mc_pds_line_num++;
	
}

