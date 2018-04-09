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
#define yyparse         ctlparse
#define yylex           ctllex
#define yyerror         ctlerror
#define yydebug         ctldebug
#define yynerrs         ctlnerrs

#define yylval          ctllval
#define yychar          ctlchar

/* Copy the first part of user declarations.  */
#line 1 "ctlparse.y" /* yacc.c:339  */


#include <stdio.h>
#include <stdlib.h>

#include "ftree.h"
#include "string.h"
#include "common.h"

int ctllex();
void ctlerror(char*);

FormulaTree pushNegation(FormulaTree);
void CTLPrintFormula(FormulaTree);
FormulaTree ctlTree=NULL;

FormulaTree* treeBuffer;
int treeBufferLength =0;
int treeBufferLengthMax =0;
int treeBufferStep =10;
int paramLength =0;
extern int mc_pds_line_num;
 
//int ctllineno=0;
extern int mc_ctl_read_from_file;
extern int mc_CTPL;

#line 102 "ctlparse.c" /* yacc.c:339  */

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
extern int ctldebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    T_ATOM = 258,
    T_Regexpr = 259,
    T_EX = 260,
    T_AX = 261,
    T_EF = 262,
    T_AF = 263,
    T_EG = 264,
    T_AG = 265,
    T_E = 266,
    T_A = 267,
    T_U = 268,
    T_W = 269,
    T_FALSEEXP = 270,
    T_TRUEEXP = 271,
    T_LB = 272,
    T_RB = 273,
    T_SQ = 274,
    T_IMP = 275,
    T_IFF = 276,
    T_OR = 277,
    T_AND = 278,
    T_NOT = 279
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 30 "ctlparse.y" /* yacc.c:355  */

	char*  string;
	FormulaTree  ftree;
	ParamList param;
	Name name;

#line 171 "ctlparse.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE ctllval;

int ctlparse (void);



/* Copy the second part of user declarations.  */

#line 188 "ctlparse.c" /* yacc.c:358  */

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
#define YYFINAL  31
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   97

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  28
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  6
/* YYNRULES -- Number of rules.  */
#define YYNRULES  30
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  65

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   279

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      26,    20,     2,     2,    27,     2,     2,     2,     2,     2,
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
      15,    16,    17,    18,    19,    21,    22,    23,    24,    25
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    54,    54,    56,    89,   121,   150,   179,   182,   222,
     256,   290,   471,   561,   598,   635,   668,   701,   761,   823,
     884,   945,   979,  1013,  1047,  1083,  1084,  1090,  1095,  1101,
    1106
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "T_ATOM", "T_Regexpr", "T_EX", "T_AX",
  "T_EF", "T_AF", "T_EG", "T_AG", "T_E", "T_A", "T_U", "T_W", "T_FALSEEXP",
  "T_TRUEEXP", "T_LB", "T_RB", "T_SQ", "')'", "T_IMP", "T_IFF", "T_OR",
  "T_AND", "T_NOT", "'('", "','", "$accept", "start", "form", "PARAM",
  "parameters", "parameter", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
      41,   275,   276,   277,   278,   279,    40,    44
};
# endif

#define YYPACT_NINF -10

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-10)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int8 yypact[] =
{
      30,     0,   -10,    30,    30,    30,    30,    30,    30,     3,
       4,   -10,   -10,    30,    30,    27,    36,     9,   -10,    20,
      20,    20,    20,    20,    20,    30,    30,    30,    30,   -10,
      73,   -10,    30,    30,    30,    30,   -10,    44,    28,    39,
      36,    -5,    36,    40,   -10,    -9,    20,   -10,   -10,   -10,
     -10,     9,    30,    30,    30,    30,   -10,    47,    54,    61,
      68,   -10,   -10,   -10,   -10
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    25,     4,     0,     0,     0,     0,     0,     0,     0,
       0,     6,     5,     0,     0,     0,     2,     0,     3,    15,
      16,    17,    18,    19,    20,     0,     0,     0,     0,     8,
       0,     1,     0,     0,     0,     0,    29,     0,     0,    27,
      13,     0,    14,     0,     7,    12,    11,    10,     9,    30,
      26,     0,     0,     0,     0,     0,    28,     0,     0,     0,
       0,    22,    24,    21,    23
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -10,   -10,    -3,   -10,    16,   -10
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,    15,    16,    18,    38,    39
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      19,    20,    21,    22,    23,    24,    25,    27,    52,    53,
      29,    30,    36,    33,    34,    35,    32,    33,    34,    35,
      26,    28,    40,    41,    42,    43,    17,    31,    37,    45,
      46,    47,    48,     1,     2,     3,     4,     5,     6,     7,
       8,     9,    10,    34,    35,    11,    12,    49,    50,    57,
      58,    59,    60,    54,    55,    13,    14,    32,    33,    34,
      35,    32,    33,    34,    35,    61,    51,    56,    32,    33,
      34,    35,    62,     0,     0,    32,    33,    34,    35,    63,
       0,     0,    32,    33,    34,    35,    64,     0,     0,    32,
      33,    34,    35,    44,    32,    33,    34,    35
};

static const yytype_int8 yycheck[] =
{
       3,     4,     5,     6,     7,     8,     3,     3,    13,    14,
      13,    14,     3,    22,    23,    24,    21,    22,    23,    24,
      17,    17,    25,    26,    27,    28,    26,     0,    19,    32,
      33,    34,    35,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    23,    24,    15,    16,     3,    20,    52,
      53,    54,    55,    13,    14,    25,    26,    21,    22,    23,
      24,    21,    22,    23,    24,    18,    27,    51,    21,    22,
      23,    24,    18,    -1,    -1,    21,    22,    23,    24,    18,
      -1,    -1,    21,    22,    23,    24,    18,    -1,    -1,    21,
      22,    23,    24,    20,    21,    22,    23,    24
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    15,    16,    25,    26,    29,    30,    26,    31,    30,
      30,    30,    30,    30,    30,     3,    17,     3,    17,    30,
      30,     0,    21,    22,    23,    24,     3,    19,    32,    33,
      30,    30,    30,    30,    20,    30,    30,    30,    30,     3,
      20,    27,    13,    14,    13,    14,    32,    30,    30,    30,
      30,    18,    18,    18,    18
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    28,    29,    30,    30,    30,    30,    30,    30,    30,
      30,    30,    30,    30,    30,    30,    30,    30,    30,    30,
      30,    30,    30,    30,    30,    31,    31,    32,    32,    33,
      33
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     3,     2,     3,
       3,     3,     3,     3,     3,     2,     2,     2,     2,     2,
       2,     6,     6,     6,     6,     0,     3,     1,     3,     1,
       2
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
        case 2:
#line 54 "ctlparse.y" /* yacc.c:1646  */
    { ctlTree = (yyvsp[0].ftree); }
#line 1305 "ctlparse.c" /* yacc.c:1646  */
    break;

  case 3:
#line 57 "ctlparse.y" /* yacc.c:1646  */
    {
	  	Name tmp = (yyvsp[-1].name);
	   
    	int i =0;
    	for (;i<treeBufferLength;i++)
    	{
    		if (treeBuffer[i] == NULL ) continue;
				if (treeBuffer[i]->token == F_IDENT && treeBuffer[i]->ident == tmp )
				{
					if (compareParams((ParamList)(treeBuffer[i]->right),(yyvsp[0].param)))
					{ 
						(yyval.ftree) = treeBuffer[i];
						break;
					}
				}    	
    	}
    	if (i == treeBufferLength)
    	{
				(yyval.ftree) = ftree_create(F_IDENT, tmp, (FormulaTree)paramLength, (FormulaTree)(yyvsp[0].param));
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] = (yyval.ftree);
			}
	  }
#line 1342 "ctlparse.c" /* yacc.c:1646  */
    break;

  case 4:
#line 90 "ctlparse.y" /* yacc.c:1646  */
    {
	  	Name tmp = name_create((yyvsp[0].string));
    	int i =0;
    	for (;i<treeBufferLength;i++)
    	{
    		if (treeBuffer[i] == NULL ) continue;
				if (treeBuffer[i]->token == F_IDENT)
				{
					if (treeBuffer[i]->ident == tmp)
					{ 
						(yyval.ftree) = treeBuffer[i];
						break;
					}
				}    	
    	}
    	if (i == treeBufferLength)
    	{
				(yyval.ftree) = ftree_create(F_IDENT,tmp,NULL,(FormulaTree)1);
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] = (yyval.ftree);
			}
	  }
#line 1378 "ctlparse.c" /* yacc.c:1646  */
    break;

  case 5:
#line 122 "ctlparse.y" /* yacc.c:1646  */
    {	
    	int i =0;
    	for (;i<treeBufferLength;i++)
    	{
    		if (treeBuffer[i] == NULL ) continue;
				if (treeBuffer[i]->token == F_TRUEEXP)
				{
					(yyval.ftree) = treeBuffer[i];
					treeBuffer[i]->ident++;
					break;
				}    	
    	}
    	if (i == treeBufferLength)
    	{
				(yyval.ftree) = ftree_create(F_TRUEEXP,1,NULL,NULL);  
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] = (yyval.ftree);
			}
	  }
#line 1411 "ctlparse.c" /* yacc.c:1646  */
    break;

  case 6:
#line 151 "ctlparse.y" /* yacc.c:1646  */
    {	
    	int i =0;
    	for (;i<treeBufferLength;i++)
    	{
    		if (treeBuffer[i] == NULL ) continue;
				if (treeBuffer[i]->token == F_FALSEEXP)
				{
					(yyval.ftree) = treeBuffer[i];
					treeBuffer[i]->ident++;
					break;
				}    	
    	}
    	if (i == treeBufferLength)
    	{
				(yyval.ftree) = ftree_create(F_FALSEEXP,1,NULL,NULL);  
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] = (yyval.ftree);
			}
	  }
#line 1444 "ctlparse.c" /* yacc.c:1646  */
    break;

  case 7:
#line 180 "ctlparse.y" /* yacc.c:1646  */
    { (yyval.ftree)=(yyvsp[-1].ftree); }
#line 1450 "ctlparse.c" /* yacc.c:1646  */
    break;

  case 8:
#line 183 "ctlparse.y" /* yacc.c:1646  */
    {	
    	int i =0;
    	for (;i<treeBufferLength;i++)
    	{
    		if (treeBuffer[i] == NULL ) continue;
				if (treeBuffer[i]->token == F_NOT)
				{
					if ((yyvsp[0].ftree) == treeBuffer[i]->left)
					{
					  (yyval.ftree) = treeBuffer[i];
					  treeBuffer[i]->ident++;
					  if ((yyvsp[0].ftree)->token!=F_IDENT) (yyvsp[0].ftree)->ident--;
					  break;
					}
				}    	
    	}
    	if (i == treeBufferLength)
    	{
    	  if ((yyvsp[0].ftree)->token == F_IDENT)	
    	  {
    	  	(yyval.ftree) = ftree_create(F_NOT,1,(yyvsp[0].ftree),NULL);
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] = (yyval.ftree);  	  
    	  }	
    	  else 
    	  {
     	  	(yyval.ftree) = pushNegation(ftree_create(F_NOT,1,(yyvsp[0].ftree),NULL));
    	  }	
			}
	  }
#line 1494 "ctlparse.c" /* yacc.c:1646  */
    break;

  case 9:
#line 223 "ctlparse.y" /* yacc.c:1646  */
    {	
    	int i =0;
    	for (;i<treeBufferLength;i++)
    	{
    		if (treeBuffer[i] == NULL ) continue;
				if (treeBuffer[i]->token == F_AND)
				{
					if (((yyvsp[-2].ftree) == treeBuffer[i]->left && (yyvsp[0].ftree) == treeBuffer[i]->right) ||((yyvsp[0].ftree) == treeBuffer[i]->left && (yyvsp[-2].ftree) == treeBuffer[i]->right))
					{
					  (yyval.ftree) = treeBuffer[i];
					  treeBuffer[i]->ident++;
					  if ((yyvsp[-2].ftree)->token!=F_IDENT) (yyvsp[-2].ftree)->ident--;
					  if ((yyvsp[0].ftree)->token!=F_IDENT) (yyvsp[0].ftree)->ident--;					  
					  break;
					}
				}    	
    	}
    	if (i == treeBufferLength)
    	{
				(yyval.ftree)  = ftree_create(F_AND,1,(yyvsp[-2].ftree),(yyvsp[0].ftree));
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] = (yyval.ftree);
			}
	  }
#line 1532 "ctlparse.c" /* yacc.c:1646  */
    break;

  case 10:
#line 257 "ctlparse.y" /* yacc.c:1646  */
    {	
    	int i =0;
    	for (;i<treeBufferLength;i++)
    	{
    		if (treeBuffer[i] == NULL ) continue;
				if (treeBuffer[i]->token == F_OR)
				{
					if (((yyvsp[-2].ftree) == treeBuffer[i]->left && (yyvsp[0].ftree) == treeBuffer[i]->right) ||((yyvsp[0].ftree) == treeBuffer[i]->left && (yyvsp[-2].ftree) == treeBuffer[i]->right))
					{
						treeBuffer[i]->ident++;
					  (yyval.ftree) = treeBuffer[i];
					  if ((yyvsp[-2].ftree)->token!=F_IDENT) (yyvsp[-2].ftree)->ident--;
					  if ((yyvsp[0].ftree)->token!=F_IDENT) (yyvsp[0].ftree)->ident--;						  
					  break;
					}
				}    	
    	}
    	if (i == treeBufferLength)
    	{
				(yyval.ftree) = ftree_create(F_OR,1,(yyvsp[-2].ftree),(yyvsp[0].ftree));
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] = (yyval.ftree);
			}
	  }
#line 1570 "ctlparse.c" /* yacc.c:1646  */
    break;

  case 11:
#line 291 "ctlparse.y" /* yacc.c:1646  */
    {  
		if((yyvsp[-2].ftree)->token!=F_IDENT) (yyvsp[-2].ftree)->ident--;
		if((yyvsp[0].ftree)->token!=F_IDENT) (yyvsp[0].ftree)->ident--;		
		FormulaTree ctlTree1 = NULL;
		FormulaTree ctlTree2 = NULL;
		FormulaTree ctlTree3 = NULL;
		FormulaTree ctlTree4 = NULL;
  	int i =0;
  	for (;i<treeBufferLength;i++)
  	{
  		if (treeBuffer[i] == NULL ) continue;
			if (treeBuffer[i]->token == F_AND)
			{
				if (((yyvsp[-2].ftree) == treeBuffer[i]->left && (yyvsp[0].ftree) == treeBuffer[i]->right) ||((yyvsp[0].ftree) == treeBuffer[i]->left && (yyvsp[-2].ftree) == treeBuffer[i]->right))
				{
					treeBuffer[i]->ident++;
				  ctlTree1 = treeBuffer[i];
				  break;
				}
			}    	
  	}
  	if (i == treeBufferLength)
  	{
			ctlTree1 =  ftree_create(F_AND,1,(yyvsp[-2].ftree),(yyvsp[0].ftree));
			if((yyvsp[-2].ftree)->token!=F_IDENT) (yyvsp[-2].ftree)->ident++;
			if((yyvsp[0].ftree)->token!=F_IDENT) (yyvsp[0].ftree)->ident++;					
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] = ctlTree1;
		}
		
		

  	for (i=0;i<treeBufferLength;i++)
  	{
  		if (treeBuffer[i] == NULL ) continue;
			if (treeBuffer[i]->token == F_NOT)
			{
				if ((yyvsp[-2].ftree) == treeBuffer[i]->left)
				{
					treeBuffer[i]->ident++;
				  ctlTree2 = treeBuffer[i];
				  break;
				}
			} 
		}   	
  	if (i == treeBufferLength)
  	{
  		if((yyvsp[-2].ftree)->token ==F_IDENT ) 
  		{
  			ctlTree2 =  ftree_create(F_NOT,1,(yyvsp[-2].ftree),NULL);
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] = ctlTree2;  		
  		}
  		else
  		{
  			(yyvsp[-2].ftree)->ident++;
  			ctlTree2 =  pushNegation(ftree_create(F_NOT,1,(yyvsp[-2].ftree),NULL));
  		}

		}
	
 
  	for (i =0 ;i<treeBufferLength;i++)
  	{
  		if (treeBuffer[i] == NULL ) continue;
			if (treeBuffer[i]->token == F_NOT)
			{
				if ((yyvsp[0].ftree) == treeBuffer[i]->left)
				{
					treeBuffer[i]->ident++;
				  ctlTree3 = treeBuffer[i];
				  break;
				}
			}    	
  	}
  	if (i == treeBufferLength)
  	{
  		if((yyvsp[0].ftree)->token ==F_IDENT )
  		{
  			ctlTree3 =  ftree_create(F_NOT,1,(yyvsp[0].ftree),NULL);
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] = ctlTree3;		
  		}
  		else
  		{
  			(yyvsp[0].ftree)->ident++;
  			ctlTree3 =  pushNegation(ftree_create(F_NOT,1,(yyvsp[0].ftree),NULL));
  		}  		
		}
	
  	for (i =0;i<treeBufferLength;i++)
  	{
  		if (treeBuffer[i] == NULL ) continue;
			if (treeBuffer[i]->token == F_AND)
			{
				if ((ctlTree2 == treeBuffer[i]->left && ctlTree3 ==treeBuffer[i]->right)||(ctlTree3 == treeBuffer[i]->left && ctlTree2 ==treeBuffer[i]->right))
				{
					treeBuffer[i]->ident++;
				  ctlTree4 = treeBuffer[i];
				  if(ctlTree2->token!=F_IDENT) 	ctlTree2->ident--;
				  if(ctlTree3->token!=F_IDENT) 	ctlTree3->ident--;				  
				  break;
				}
			}    	
  	}
  	if (i == treeBufferLength)
  	{
			ctlTree4 =  ftree_create(F_AND,1,ctlTree2,ctlTree3);
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] = ctlTree4;
		}	
  	
  	for (i=0;i<treeBufferLength;i++)
  	{
  		if (treeBuffer[i] == NULL ) continue;
			if (treeBuffer[i]->token == F_OR)
			{
				if ((ctlTree1 == treeBuffer[i]->left && ctlTree4== treeBuffer[i]->right) ||(ctlTree4 == treeBuffer[i]->left && ctlTree1== treeBuffer[i]->right))
				{
					treeBuffer[i]->ident++;
				  (yyval.ftree) = treeBuffer[i];
					ctlTree1->ident--;
					ctlTree4->ident--;
			  				  
				  break;
				}
			}    	
  	}
  	if (i == treeBufferLength)
  	{
			(yyval.ftree) = ftree_create(F_OR,1,ctlTree1,ctlTree4);	
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] = (yyval.ftree);
		}			
	 }
#line 1755 "ctlparse.c" /* yacc.c:1646  */
    break;

  case 12:
#line 472 "ctlparse.y" /* yacc.c:1646  */
    { 
		if((yyvsp[-2].ftree)->token!=F_IDENT) (yyvsp[-2].ftree)->ident--;	
		if((yyvsp[0].ftree)->token!=F_IDENT) (yyvsp[0].ftree)->ident--;	
		FormulaTree ctlTree1 = NULL;	
  	int i =0;
  	for (;i<treeBufferLength;i++)
  	{
  		if (treeBuffer[i] == NULL ) continue;
			if (treeBuffer[i]->token == F_NOT)
			{
				if ((yyvsp[-2].ftree) == treeBuffer[i]->left )
				{
					treeBuffer[i]->ident++;
				  ctlTree1 = treeBuffer[i];
				  break;
				}
			}    	
  	}
  	if (i == treeBufferLength)
  	{	
  	  if((yyvsp[-2].ftree)->token == F_IDENT)
  	  { 
  	  	ctlTree1 =  ftree_create(F_NOT,1,(yyvsp[-2].ftree),NULL);
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] =  ctlTree1;  	
  	  }
  		else	
  		{	
  			(yyvsp[-2].ftree)->ident++;	
  			ctlTree1 =  pushNegation(ftree_create(F_NOT,1,(yyvsp[-2].ftree),NULL));
			}
			
			(yyval.ftree) = ftree_create(F_OR,1,ctlTree1,(yyvsp[0].ftree));	
			if((yyvsp[0].ftree)->token!=F_IDENT) (yyvsp[0].ftree)->ident++;	
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] = (yyval.ftree);							
		}
		else
		{
    	for (i=0;i<treeBufferLength;i++)
    	{ 		
    		if (treeBuffer[i] == NULL ) continue;
  			if (treeBuffer[i]->token == F_OR)
  			{
  				if ((ctlTree1 == treeBuffer[i]->left && (yyvsp[0].ftree)== treeBuffer[i]->right)||(ctlTree1 == treeBuffer[i]->right && (yyvsp[0].ftree)== treeBuffer[i]->left))
  				{
  				  (yyval.ftree) = treeBuffer[i];
  				  treeBuffer[i]->ident++;
  				  ctlTree1->ident--;
  				  break;
  				}
  			}    	
    	}
    	if (i == treeBufferLength)
    	{
  			(yyval.ftree) = ftree_create(F_OR,1,ctlTree1,(yyvsp[0].ftree));	
  			if((yyvsp[0].ftree)->token!=F_IDENT) (yyvsp[0].ftree)->ident++;	
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] = (yyval.ftree);
  		}				
		}
	}
#line 1849 "ctlparse.c" /* yacc.c:1646  */
    break;

  case 13:
#line 562 "ctlparse.y" /* yacc.c:1646  */
    {
		if(!mc_CTPL) mc_CTPL++;
		Name tmp = (yyvsp[-1].name);
   	int i =0;
  	for (;i<treeBufferLength;i++)
  	{
  		if (treeBuffer[i] == NULL ) continue;
			if (treeBuffer[i]->token == F_E)
			{
				if (((ParamList)(treeBuffer[i]->right))->name == tmp)
				{
					if ((yyvsp[0].ftree) == treeBuffer[i]->left)
					{
				 		(yyval.ftree) = treeBuffer[i];
				 		treeBuffer[i]->ident++;
				 		break;
				 	}
				}    	
  		}
  	}
  	if (i == treeBufferLength)
  	{
			(yyval.ftree) = ftree_create(F_E,1,(yyvsp[0].ftree), (FormulaTree)createParam(tmp,0, NULL));
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] = (yyval.ftree);
		}			
	}
#line 1890 "ctlparse.c" /* yacc.c:1646  */
    break;

  case 14:
#line 599 "ctlparse.y" /* yacc.c:1646  */
    {
		if(!mc_CTPL) mc_CTPL++;
		Name tmp =(yyvsp[-1].name);
   	int i =0;
  	for (;i<treeBufferLength;i++)
  	{
  		if (treeBuffer[i] == NULL ) continue;
			if (treeBuffer[i]->token == F_A)
			{
				if (((ParamList)(treeBuffer[i]->right))->name == tmp)
				{
					if ((yyvsp[0].ftree) == treeBuffer[i]->left)
					{
				 		(yyval.ftree) = treeBuffer[i];
				 		treeBuffer[i]->ident++;
				 		break;
				 	}
				}    	
  		}
  	}
  	if (i == treeBufferLength)
  	{
			(yyval.ftree) = ftree_create(F_A,1,(yyvsp[0].ftree),(FormulaTree)createParam(tmp,0,NULL));
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] = (yyval.ftree);
		}			
	}
#line 1931 "ctlparse.c" /* yacc.c:1646  */
    break;

  case 15:
#line 636 "ctlparse.y" /* yacc.c:1646  */
    {
   	int i =0;
  	for (;i<treeBufferLength;i++)
  	{
  		if (treeBuffer[i] == NULL ) continue;
			if (treeBuffer[i]->token == F_EX)
			{
				if ((yyvsp[0].ftree) == treeBuffer[i]->left )
				{
				  (yyval.ftree) = treeBuffer[i];
				  treeBuffer[i]->ident++;
				  if ((yyvsp[0].ftree)->token!=F_IDENT) (yyvsp[0].ftree)->ident--;
				  break;
				}
			}    	
  	}
  	if (i == treeBufferLength)
  	{
			(yyval.ftree) = ftree_create(F_EX,1,(yyvsp[0].ftree),NULL);
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] = (yyval.ftree);
		}
	}
#line 1968 "ctlparse.c" /* yacc.c:1646  */
    break;

  case 16:
#line 669 "ctlparse.y" /* yacc.c:1646  */
    {
   	int i =0;
  	for (;i<treeBufferLength;i++)
  	{
  		if (treeBuffer[i] == NULL ) continue;
			if (treeBuffer[i]->token == F_AX)
			{
				if ((yyvsp[0].ftree) == treeBuffer[i]->left )
				{
				  (yyval.ftree) = treeBuffer[i];
				  treeBuffer[i]->ident++;
				  if ((yyvsp[0].ftree)->token!=F_IDENT) (yyvsp[0].ftree)->ident--;
				  break;
				}
			}    	
  	}
  	if (i == treeBufferLength)
  	{
			(yyval.ftree) = ftree_create(F_AX,1,(yyvsp[0].ftree),NULL);
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] = (yyval.ftree);
		}
	}
#line 2005 "ctlparse.c" /* yacc.c:1646  */
    break;

  case 17:
#line 702 "ctlparse.y" /* yacc.c:1646  */
    {
  		FormulaTree ctlTree1 = NULL;
  	  int i;
    	for (i =0;i<treeBufferLength;i++)
    	{
    		if (treeBuffer[i] == NULL ) continue;
				if (treeBuffer[i]->token == F_TRUEEXP)
				{
					ctlTree1 = treeBuffer[i];
					ctlTree1->ident++;
					break;
				}    	
    	}
    	if (i == treeBufferLength)
    	{
				ctlTree1 = ftree_create(F_TRUEEXP,1,NULL,NULL); 
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] = ctlTree1;
			}
    	for (i=0;i<treeBufferLength;i++)
    	{
    		if (treeBuffer[i] == NULL ) continue;
				if (treeBuffer[i]->token == F_EU)
				{
					if (ctlTree1 ==treeBuffer[i]->left && (yyvsp[0].ftree) ==treeBuffer[i]->right)
					{
						(yyval.ftree) = treeBuffer[i];
						treeBuffer[i]->ident++;
						ctlTree1->ident--;
						if((yyvsp[0].ftree)->token!=F_IDENT) (yyvsp[0].ftree)->ident--;						
						break;
					}
				}    	
    	}
    	if (i == treeBufferLength)
    	{
				(yyval.ftree) = ftree_create(F_EU,1,ctlTree1,(yyvsp[0].ftree));
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] = (yyval.ftree);
			}			
  	}
#line 2069 "ctlparse.c" /* yacc.c:1646  */
    break;

  case 18:
#line 762 "ctlparse.y" /* yacc.c:1646  */
    {
  		FormulaTree ctlTree1 = NULL;
  	  int i =0;
    	for (;i<treeBufferLength;i++)
    	{
    		if (treeBuffer[i] == NULL ) continue;
				if (treeBuffer[i]->token == F_TRUEEXP)
				{
					ctlTree1 = treeBuffer[i];
					ctlTree1->ident++;
					break;
				}    	
    	}
    	if (i == treeBufferLength)
    	{
				ctlTree1 = ftree_create(F_TRUEEXP,1,NULL,NULL); 
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] =  ctlTree1;
			}
  	 
    	for (i=0;i<treeBufferLength;i++)
    	{
    		if (treeBuffer[i] == NULL ) continue;
				if (treeBuffer[i]->token == F_AU)
				{
					if (treeBuffer[i] == NULL ) continue;
					if (ctlTree1 ==treeBuffer[i]->left && (yyvsp[0].ftree) ==treeBuffer[i]->right)
					{
						(yyval.ftree) = treeBuffer[i];
						treeBuffer[i]->ident++;
						ctlTree1->ident--;
						if((yyvsp[0].ftree)->token!=F_IDENT) (yyvsp[0].ftree)->ident--;						
						break;
					}
				}    	
    	}
    	if (i == treeBufferLength)
    	{
				(yyval.ftree) = ftree_create(F_AU,1,ctlTree1,(yyvsp[0].ftree));
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] = (yyval.ftree);
			} 	  		
  	}
#line 2135 "ctlparse.c" /* yacc.c:1646  */
    break;

  case 19:
#line 824 "ctlparse.y" /* yacc.c:1646  */
    { 
  		FormulaTree ctlTree1 = NULL;
  	  int i =0;
    	for (;i<treeBufferLength;i++)
    	{
    		if (treeBuffer[i] == NULL ) continue;
				if (treeBuffer[i]->token == F_FALSEEXP)
				{
					ctlTree1 = treeBuffer[i];
					ctlTree1->ident++;
					break;
				}    	
    	}
    	if (i == treeBufferLength)
    	{
				ctlTree1 = ftree_create(F_FALSEEXP,1,NULL,NULL); 
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] = ctlTree1;
			}

    	for (i=0;i<treeBufferLength;i++)
    	{
    		if (treeBuffer[i] == NULL ) continue;
				if (treeBuffer[i]->token == F_EW)
				{
					if (ctlTree1 ==treeBuffer[i]->left && (yyvsp[0].ftree) ==treeBuffer[i]->right)
					{
						(yyval.ftree) = treeBuffer[i];
						(yyval.ftree)->ident++;
						ctlTree1->ident--;
						if((yyvsp[0].ftree)->token!=F_IDENT) (yyvsp[0].ftree)->ident--;						
						break;
					}
				}    	
    	}
    	if (i == treeBufferLength)
    	{
				(yyval.ftree) = ftree_create(F_EW,1,ctlTree1,(yyvsp[0].ftree)); 
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] = (yyval.ftree);
			} 	  		
  	}
#line 2200 "ctlparse.c" /* yacc.c:1646  */
    break;

  case 20:
#line 885 "ctlparse.y" /* yacc.c:1646  */
    {
  		FormulaTree ctlTree1 = NULL;
  	  int i =0;
    	for (;i<treeBufferLength;i++)
    	{
    		if (treeBuffer[i] == NULL ) continue;
				if (treeBuffer[i]->token == F_FALSEEXP)
				{
					ctlTree1 = treeBuffer[i];
					ctlTree1->ident++;
					break;
				}    	
    	}
    	if (i == treeBufferLength)
    	{
				ctlTree1 = ftree_create(F_FALSEEXP,1,NULL,NULL); 
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] = ctlTree1;
			}
 
    	for (i=0;i<treeBufferLength;i++)
    	{
    		if (treeBuffer[i] == NULL ) continue;
				if (treeBuffer[i]->token == F_AW)
				{
					if (ctlTree1 ==treeBuffer[i]->left && (yyvsp[0].ftree) ==treeBuffer[i]->right)
					{	
						(yyval.ftree) = treeBuffer[i];
						(yyval.ftree)->ident++;
						ctlTree1->ident--;
						if((yyvsp[0].ftree)->token!=F_IDENT) (yyvsp[0].ftree)->ident--;						
						break;
					}
				}    	
    	}
    	if (i == treeBufferLength)
    	{
				(yyval.ftree) = ftree_create(F_AW,1,ctlTree1,(yyvsp[0].ftree)); 
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] = (yyval.ftree);
			} 	  		
  	}
#line 2265 "ctlparse.c" /* yacc.c:1646  */
    break;

  case 21:
#line 946 "ctlparse.y" /* yacc.c:1646  */
    {
  	  int i =0;
    	for (;i<treeBufferLength;i++)
    	{
    		if (treeBuffer[i] == NULL ) continue;
				if (treeBuffer[i]->token == F_AU)
				{
					if ((yyvsp[-3].ftree) ==treeBuffer[i]->left && (yyvsp[-1].ftree) ==treeBuffer[i]->right)
					{
						(yyval.ftree) = treeBuffer[i];
						treeBuffer[i]->ident++;
						if ((yyvsp[-3].ftree)->token!=F_IDENT) (yyvsp[-3].ftree)->ident--;
						if ((yyvsp[-1].ftree)->token!=F_IDENT) (yyvsp[-1].ftree)->ident--;						
						break;
					}
				}    	
    	}
    	if (i == treeBufferLength)
    	{
				(yyval.ftree) = ftree_create(F_AU,1,(yyvsp[-3].ftree),(yyvsp[-1].ftree)); 
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] = (yyval.ftree);
			} 	  		
  	}
#line 2303 "ctlparse.c" /* yacc.c:1646  */
    break;

  case 22:
#line 980 "ctlparse.y" /* yacc.c:1646  */
    {
  	  int i =0;
    	for (;i<treeBufferLength;i++)
    	{
    		if (treeBuffer[i] == NULL ) continue;
				if (treeBuffer[i]->token == F_EU)
				{
					if ((yyvsp[-3].ftree) ==treeBuffer[i]->left && (yyvsp[-1].ftree) ==treeBuffer[i]->right)
					{
						(yyval.ftree) = treeBuffer[i];
						treeBuffer[i]->ident++;
						if ((yyvsp[-3].ftree)->token!=F_IDENT) (yyvsp[-3].ftree)->ident--;
						if ((yyvsp[-1].ftree)->token!=F_IDENT) (yyvsp[-1].ftree)->ident--;							
						break;
					}
				}    	
    	}
    	if (i == treeBufferLength)
    	{
				(yyval.ftree) = ftree_create(F_EU,1,(yyvsp[-3].ftree),(yyvsp[-1].ftree));
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] = (yyval.ftree);
			} 	  		
  	}
#line 2341 "ctlparse.c" /* yacc.c:1646  */
    break;

  case 23:
#line 1014 "ctlparse.y" /* yacc.c:1646  */
    {
  	  int i =0;
    	for (;i<treeBufferLength;i++)
    	{
    		if (treeBuffer[i] == NULL ) continue;
				if (treeBuffer[i]->token == F_AW)
				{	
					if ((yyvsp[-3].ftree) ==treeBuffer[i]->left && (yyvsp[-1].ftree) ==treeBuffer[i]->right)
					{
						(yyval.ftree) = treeBuffer[i];
						treeBuffer[i]->ident++;
						if ((yyvsp[-3].ftree)->token!=F_IDENT) (yyvsp[-3].ftree)->ident--;
						if ((yyvsp[-1].ftree)->token!=F_IDENT) (yyvsp[-1].ftree)->ident--;							
						break;
					}
				}    	
    	}
    	if (i == treeBufferLength)
    	{
				(yyval.ftree) = ftree_create(F_AW,1,(yyvsp[-3].ftree),(yyvsp[-1].ftree));
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] = (yyval.ftree);
			} 	  		
  	}
#line 2379 "ctlparse.c" /* yacc.c:1646  */
    break;

  case 24:
#line 1048 "ctlparse.y" /* yacc.c:1646  */
    {
	  int i =0;
  	for (;i<treeBufferLength;i++)
  	{
  		if (treeBuffer[i] == NULL ) continue;
			if (treeBuffer[i]->token == F_EW)
			{
				if ((yyvsp[-3].ftree) ==treeBuffer[i]->left && (yyvsp[-1].ftree) ==treeBuffer[i]->right)
				{
					(yyval.ftree) = treeBuffer[i];
					treeBuffer[i]->ident++;
					if ((yyvsp[-3].ftree)->token!=F_IDENT) (yyvsp[-3].ftree)->ident--;
					if ((yyvsp[-1].ftree)->token!=F_IDENT) (yyvsp[-1].ftree)->ident--;						
					break;
				}
			}    	
  	}
  	if (i == treeBufferLength)
  	{
			(yyval.ftree) = ftree_create(F_EW,1,(yyvsp[-3].ftree),(yyvsp[-1].ftree)); 
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] = (yyval.ftree);
		} 	  		
  }
#line 2417 "ctlparse.c" /* yacc.c:1646  */
    break;

  case 25:
#line 1083 "ctlparse.y" /* yacc.c:1646  */
    { paramLength =0; (yyval.param) =NULL; }
#line 2423 "ctlparse.c" /* yacc.c:1646  */
    break;

  case 26:
#line 1085 "ctlparse.y" /* yacc.c:1646  */
    {
  	(yyval.param)=(yyvsp[-1].param);
  }
#line 2431 "ctlparse.c" /* yacc.c:1646  */
    break;

  case 27:
#line 1091 "ctlparse.y" /* yacc.c:1646  */
    {
		paramLength=1;
		(yyval.param) = (yyvsp[0].param);
	}
#line 2440 "ctlparse.c" /* yacc.c:1646  */
    break;

  case 28:
#line 1096 "ctlparse.y" /* yacc.c:1646  */
    {
		paramLength++;
		(yyvsp[-2].param)->next = (yyvsp[0].param);
		(yyval.param)=(yyvsp[-2].param);
	}
#line 2450 "ctlparse.c" /* yacc.c:1646  */
    break;

  case 29:
#line 1102 "ctlparse.y" /* yacc.c:1646  */
    {
			if(!mc_CTPL) mc_CTPL++;
			(yyval.param) = createParam((yyvsp[0].name),0, NULL);
		}
#line 2459 "ctlparse.c" /* yacc.c:1646  */
    break;

  case 30:
#line 1107 "ctlparse.y" /* yacc.c:1646  */
    {
			if(!mc_CTPL) mc_CTPL++;
			(yyval.param) = createParam((yyvsp[0].name),1, NULL);
		}
#line 2468 "ctlparse.c" /* yacc.c:1646  */
    break;


#line 2472 "ctlparse.c" /* yacc.c:1646  */
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
#line 1112 "ctlparse.y" /* yacc.c:1906  */


#define YY_ALWAYS_INTERACTIVE 1
#include "ctllex.c"

void ctlerror (char *info)
{ 
	common_error("CTL syntax error in formula: %s", info);
}

void CTLPrintFormula(FormulaTree tree)
{
	char *op = NULL;
	char *aux1 = NULL;
	char *aux2 = NULL;
	if (!tree) return;
	switch (tree->token)
	{
	    case F_FALSEEXP:
		common_addbuf("false"); return;
	    case F_TRUEEXP: 
		common_addbuf("true"); return;
	    case F_NOT:	if (!op) op = "!";
	    case F_EX:	if (!op) op = "EX";
	    case F_AX:	if (!op) op = "AX";
		common_addbuf(op);
		common_addbuf("(");
		CTLPrintFormula(tree->left);
		common_addbuf(")");	    
	  return;
	    case F_A:   if (!op) op = "A ";
	    case F_E:   if (!op) op = "E ";
		common_addbuf(op);
		common_addbuf(name_cstring(((ParamList)(tree->right))->name));
		common_addbuf("(");
		CTLPrintFormula(tree->left);
		common_addbuf(")");	    
	  return;	    
	    case F_AND:	if (!op) op = "&&";
	    case F_OR:	if (!op) op = "||";
		common_addbuf("(");
		CTLPrintFormula(tree->left);
		common_addbuf(op);
		CTLPrintFormula(tree->right);
		common_addbuf(")");
		return;	    
	    case F_EU:	if (!aux1 && !aux2) { aux1 = "E[("; aux2 = ")U("; }
	    case F_AU:	if (!aux1 && !aux2) {mc_pds_line_num++; aux1 = "A[("; aux2 = ")U("; }
	    case F_EW:	if (!aux1 && !aux2) { aux1 = "E[("; aux2 = ")W("; }
	    case F_AW:	if (!aux1 && !aux2) {mc_pds_line_num++; aux1 = "A[("; aux2 = ")W("; }
				common_addbuf(aux1);
				CTLPrintFormula(tree->left);
				common_addbuf(aux2);
				CTLPrintFormula(tree->right);
				common_addbuf(")]");
				return;	    
	    case F_IDENT:
	    {
	      if(tree->right)
	      {
	        if(tree->left)
	        {
	          common_addbuf(name_cstring(tree->ident));
  	    		common_addbuf("(");
  	    		ParamList tmp =(ParamList)tree->right;
  	    		while(tmp->next)
  	    		{
  	    			if(tmp->type) common_addbuf("'");
  	    			common_addbuf(name_cstring(tmp->name));
  	    			common_addbuf(",");
  	    			tmp = tmp->next;
  	    		}
  	    		if(tmp->type) common_addbuf("'");
  	    		common_addbuf(name_cstring(tmp->name));
  	    		common_addbuf(")");		          
	        }
	        else
	        {
	    		  common_addbuf("$");	
					  common_addbuf(name_cstring(tree->ident));
					  common_addbuf("$");		          
	        }
	      }
	      else common_addbuf(name_cstring(tree->ident));			
				return;
			}
	    default:
				common_error("unknown token (%i)",tree->token);
		return;
	}
}

FormulaTree pushNegation(FormulaTree ftree)
{
	FormulaTree tmp1 = NULL;
	FormulaTree tmp2 = NULL;  
	char dualToken = 0;
	int i, j;
  switch(ftree->left->token)	
  {
		case F_FALSEEXP:	if (!dualToken) dualToken = F_TRUEEXP;
		case F_TRUEEXP:  if (!dualToken) dualToken = F_FALSEEXP;
    	for (i =0;i<treeBufferLength;i++)
    	{
    		if (treeBuffer[i] == NULL) continue;
				if (treeBuffer[i]->token == dualToken)
				{			
					if (ftree->left->ident ==1 )
					{
						for (j =0;j<treeBufferLength;j++)
						{
							if (treeBuffer[j] == NULL) continue;
							if (treeBuffer[j]==ftree->left)
							{
								treeBuffer[j] =NULL;
								common_free(ftree->left,sizeof(FormulaTree));
								break;
							}
						}
					}
					else	ftree->left->ident--;
					ftree->left = NULL;
					common_free(ftree, sizeof(FormulaTree));
					treeBuffer[i]->ident++;
					return treeBuffer[i];
				} 
			}
			if (i == treeBufferLength)  	
			{
				ftree->token = dualToken;
				if (ftree->left->ident ==1 )
				{
					for (j=0;j<treeBufferLength;j++)
					{
						if (treeBuffer[j] == NULL) continue;
						if (treeBuffer[j] == ftree->left)
						{
							treeBuffer[j] =NULL;
							common_free(ftree->left,sizeof(FormulaTree));
							break;
						}
					}  				
				}
				else ftree->left->ident--;
				ftree->left = NULL;	
				ftree->ident =1;
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] = ftree;	
				return ftree;
			}
		case F_IDENT: 
		{ 
		  for (j=0;j<treeBufferLength; j++)
		  {
		  	if (treeBuffer[j] ==NULL) continue;
		  	if (treeBuffer[j]->token == F_NOT)
		  	{
		  		if (ftree->left == treeBuffer[j]->left)
		  		{
		  			common_free(ftree,sizeof(FormulaTree));
		  			treeBuffer[j]->ident++;		  			
		  			return treeBuffer[j];
		  		}
		  	}
		  }
		  if (j == treeBufferLength)
		  {
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] = ftree;
		  }
			return ftree;
		}
		case F_NOT: 
			tmp1 = ftree->left->left;

			if (ftree->left->ident == 1)
			{
				for (j=0;j<treeBufferLength;j++)
				{
					if (treeBuffer[j] ==NULL) continue;
					if (treeBuffer[j] == ftree->left)
					{
						treeBuffer[j] = NULL;				
						//common_free(ftree->left, sizeof(FormulaTree));
						ftree->left =NULL;
						break;
					}
				}
			}
			else	ftree->left->ident--;
			common_free(ftree, sizeof(FormulaTree));
	   	return tmp1;
	  case F_EX:  if (!dualToken)  dualToken=F_AX;
	  case F_AX:	if (!dualToken)  dualToken=F_EX;
	  	ftree->token = dualToken;
	  	if (ftree->left->ident ==1)
	  	{
				ftree->left->token = F_NOT;
				for(j=0;j<treeBufferLength;j++)
				{
					if(treeBuffer[j]==ftree->left) 
					{
						treeBuffer[j] = NULL;
						break;
					}
				}
				ftree->left = pushNegation(ftree->left);
			}
			else
			{
				ftree->left->ident--;
				for(j=0;j<treeBufferLength;j++)
				{
					if(treeBuffer[j] == NULL) continue;
					if(treeBuffer[j]->token==F_NOT && treeBuffer[j]->left==ftree->left->left) 
					{
						tmp1 = treeBuffer[j];
						tmp1->ident++;
						break;
					}
				}
				if(j==treeBufferLength) 	
				{		
			  	tmp1 = ftree_create(F_NOT,1,ftree->left->left,NULL);
			  }
			  
			  if(tmp1->left->token==F_IDENT)
			  {
			  	ftree->left = tmp1;
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] = tmp1;				  	
			  }
			  else
			  {
			  	if(ftree->left->left->token!=F_IDENT) ftree->left->left->ident++;
			  	ftree->left = pushNegation(tmp1);	
			  }
			}
			for (j=0;j<treeBufferLength;j++)
			{
			  if (treeBuffer[j] == NULL) continue;
				if (treeBuffer[j]->token == dualToken && treeBuffer[j]->left ==ftree->left )
				{
					common_free(ftree, sizeof(FormulaTree));
					ftree = treeBuffer[j];
					if(treeBuffer[j]->left->token!=F_IDENT) treeBuffer[j]->left->ident--;				
					break;
				}
			}
			if (j == treeBufferLength)
			{
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] = ftree;			
			}
	  	return ftree;	
	  case F_E:  if (!dualToken)  dualToken=F_A;
	  case F_A:	 if (!dualToken)  dualToken=F_E;
	  	ftree->token = dualToken;
	  	if (ftree->left->ident ==1)
	  	{
	  		ftree->right = ftree->left->right;
				ftree->left->token = F_NOT;
				ftree->left->right = NULL;
				for(j=0;j<treeBufferLength;j++)
				{
					if(treeBuffer[j]==ftree->left) 
					{
						treeBuffer[j] = NULL;
						break;
					}
				}
				ftree->left = pushNegation(ftree->left);
			}
			else
			{
				ftree->right = (FormulaTree)paramsCopy((ParamList)ftree->left->right);
				ftree->left->ident--;
				for(j=0;j<treeBufferLength;j++)
				{
					if(treeBuffer[j] == NULL) continue;
					if(treeBuffer[j]->token==F_NOT && treeBuffer[j]->left==ftree->left->left) 
					{
						tmp1 = treeBuffer[j];
						tmp1->ident++;
						break;
					}
				}
				if(j==treeBufferLength) 	
				{		
			  	tmp1 = ftree_create(F_NOT,1,ftree->left->left,NULL);

  			  if(tmp1->left->token==F_IDENT)
  			  {
  			   	ftree->left = tmp1;
  					treeBufferLength++;
  					if(treeBufferLength>treeBufferLengthMax)
  					{
  						treeBufferLengthMax+=treeBufferStep;
  						if(treeBufferLengthMax>treeBufferStep)
  						{
  							treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
  						}
  						else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
  					}
  					treeBuffer[treeBufferLength-1] =  tmp1;				  	
  			  }
  			  else
  			  {
  			  	ftree->left->left->ident++;
  			  	ftree->left = pushNegation(tmp1);	
  			  }
  			}
			}
			for (j=0;j<treeBufferLength;j++)
			{
			  if (treeBuffer[j] == NULL) continue;
				if (treeBuffer[j]->token == dualToken && treeBuffer[j]->left ==ftree->left) 
				{
				  if( ((ParamList)(treeBuffer[j]->right))->name == ((ParamList)(ftree->right))->name )
					deleteParams((ParamList)(ftree->right));
					common_free(ftree, sizeof(FormulaTree));
					ftree = treeBuffer[j];
					treeBuffer[j]->left->ident--;				
					break;
				}
			}
			if (j == treeBufferLength)
			{
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] = ftree;			
			}
	  	return ftree;	
	  case F_AND: if (!dualToken)  dualToken=F_OR;
	  case F_OR:  if (!dualToken)  dualToken=F_AND;
	  	ftree->token = dualToken;
	  	tmp1 = ftree->left->left;
	  	tmp2 = ftree->left->right;
	  	if( ftree->left->ident == 1 )
	  	{
	  		for (j=0;j<treeBufferLength;j++)
	  		{
	  			if (treeBuffer[j] == NULL) continue;
					if (treeBuffer[j] == ftree->left)
					{
					 	treeBuffer[j] = NULL;
					 	if(tmp1->token!=F_IDENT) tmp1->ident--;
					 	if(tmp2->token!=F_IDENT) tmp2->ident--;
					 	
						common_free(ftree->left, sizeof(FormulaTree));				
						break;
					}	  			
	  		}
	  	}
	  	else
	  	{
	  		ftree->left->ident--;  		
	  	} 	
	  	if(tmp1==tmp2)
	  	{
  	  	for (j=0;j<treeBufferLength;j++)
  	  	{
  	  		if (treeBuffer[j] == NULL) continue;
  	  		if (treeBuffer[j]->token == F_NOT && treeBuffer[j]->left == tmp1)
  	  		{
  	  			ftree->left = treeBuffer[j];
  	  			treeBuffer[j]->ident++;
  	  		}
  	  	}
  	  	if(j==treeBufferLength)
  	  	{
  	  		ftree->left = ftree_create(F_NOT,1,tmp1,NULL);
  	  		if(tmp1->token==F_IDENT)
  	  		{
  					treeBufferLength++;
  					if(treeBufferLength>treeBufferLengthMax)
  					{
  						treeBufferLengthMax+=treeBufferStep;
  						if(treeBufferLengthMax>treeBufferStep)
  						{
  							treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
  						}
  						else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
  					}
  					treeBuffer[treeBufferLength-1] = ftree->left;	 
  	  		}
  	  		else
  	  		{
  	  			tmp1->ident++;
  	  			ftree->left = pushNegation(ftree->left);
  	  		}
  	  	}
  	  	ftree->right=ftree->left;
  	  	if(ftree->left->token!=F_IDENT) ftree->left->ident++;	   		
	  	}
	  	else
	  	{	  	
  	  	for (j=0;j<treeBufferLength;j++)
  	  	{
  	  		if (treeBuffer[j] == NULL) continue;
  	  		if (treeBuffer[j]->token == F_NOT && treeBuffer[j]->left == tmp1)
  	  		{
  	  			ftree->left = treeBuffer[j];
  	  			treeBuffer[j]->ident++;
  	  		}
  	  	}
  	  	if(j==treeBufferLength)
  	  	{
  	  		ftree->left = ftree_create(F_NOT,1,tmp1,NULL);
  	  		if(tmp1->token==F_IDENT)
  	  		{
  				treeBufferLength++;
  				if(treeBufferLength>treeBufferLengthMax)
  				{
  					treeBufferLengthMax+=treeBufferStep;
  					if(treeBufferLengthMax>treeBufferStep)
  					{
  						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
  					}
  					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
  				}
  				treeBuffer[treeBufferLength-1] = ftree->left;	 
  	  		}
  	  		else
  	  		{
  	  			tmp1->ident++;
  	  			ftree->left = pushNegation(ftree->left);
  	  		}
  	  	}
  
  	  	for (j=0;j<treeBufferLength;j++)
  	  	{
  	  		if (treeBuffer[j] == NULL) continue;
  	  		if (treeBuffer[j]->token == F_NOT && treeBuffer[j]->left == tmp2)
  	  		{
  	  			ftree->right = treeBuffer[j];
  	  			treeBuffer[j]->ident++;
  	  		}
  	  	}
  	  	if(j==treeBufferLength)
  	  	{
  	  		ftree->right = ftree_create(F_NOT,1,tmp2,NULL);
  	  		if(tmp2->token==F_IDENT)
  	  		{
  				treeBufferLength++;
  				if(treeBufferLength>treeBufferLengthMax)
  				{
  					treeBufferLengthMax+=treeBufferStep;
  					if(treeBufferLengthMax>treeBufferStep)
  					{
  						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
  					}
  					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
  				}
  				treeBuffer[treeBufferLength-1] = ftree->right;	 
  	  		}
  	  		else
  	  		{
  	  			tmp2->ident++;	
  	  			ftree->right = pushNegation(ftree->right);	
  	  		}
  	  	}	  		
  		}
			for (j=0;j<treeBufferLength;j++)
			{
			  if (treeBuffer[j] == NULL) continue;
				if (treeBuffer[j]->token == dualToken && ( ( treeBuffer[j]->left ==ftree->left && treeBuffer[j]->right == ftree->right)\
				                                      || ( treeBuffer[j]->left ==ftree->right && treeBuffer[j]->right == ftree->left)))
				{
					common_free(ftree, sizeof(FormulaTree));
					ftree = treeBuffer[j];
					if(treeBuffer[j]->left->token!=F_IDENT) treeBuffer[j]->left->ident--;
					if(treeBuffer[j]->right->token!=F_IDENT) treeBuffer[j]->right->ident--;				
					break;
				}
			}	
			if (j == treeBufferLength)
			{
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] =  ftree;	
			}  	
	  	return ftree;		
		case F_EU:	 if (!dualToken)  dualToken=F_AW;	    		  
		case F_AU:	 if (!dualToken)  dualToken=F_EW;	  	
	  case F_EW:	 if (!dualToken)  dualToken=F_AU;	  
		case F_AW:	 if (!dualToken)  dualToken=F_EU;	  			  		  				  		  
	  	ftree->token = dualToken;
	  	tmp1 = ftree->left->left;
	  	tmp2 = ftree->left->right;
	  	
	  	if( ftree->left->ident == 1 )
	  	{
	  		for (j=0;j<treeBufferLength;j++)
	  		{
	  			if (treeBuffer[j] == NULL) continue;
					if (treeBuffer[j] == ftree->left)
					{
					 	treeBuffer[j] = NULL;
						common_free(ftree->left, sizeof(FormulaTree));	
						if(tmp1->token!=F_IDENT) tmp1->ident--;		
						if(tmp2->token!=F_IDENT) tmp2->ident--;	
						break;
					}	  			
	  		}
	  	}
	  	else
	  	{
	  		ftree->left->ident--;		
	  	} 	
	  	
	  	if(tmp1==tmp2)
	  	{
  	  	for (j=0;j<treeBufferLength;j++)
  	  	{
  	  		if (treeBuffer[j] == NULL) continue;
  	  		if (treeBuffer[j]->token == F_NOT && treeBuffer[j]->left == tmp1)
  	  		{
  	  			ftree->left = treeBuffer[j];
  	  			ftree->left->ident++;
  	  			break;
  	  		}
  	  	}
  	  	if(j==treeBufferLength)
  	  	{
  	  		if(tmp1->token==F_IDENT)
  	  		{
  	  			ftree->left = ftree_create(F_NOT,1,tmp1,NULL);
  				treeBufferLength++;
  				if(treeBufferLength>treeBufferLengthMax)
  				{
  					treeBufferLengthMax+=treeBufferStep;
  					if(treeBufferLengthMax>treeBufferStep)
  					{
  						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
  					}
  					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
  				}
  				treeBuffer[treeBufferLength-1] =  ftree->left;	  			
  	  		}
  	  		else
  	  		{
  	  			tmp1->ident++;
  	  			ftree->left = pushNegation(ftree_create(F_NOT,1,tmp1,NULL));
  	  		}
  	  	}
  	  	ftree->right =ftree->left;
  	  	if(ftree->left->token!=F_IDENT) ftree->left->ident++;	  	  		  	
	  	}
	  	else
	  	{	  	
  	  	for (j=0;j<treeBufferLength;j++)
  	  	{
  	  		if (treeBuffer[j] == NULL) continue;
  	  		if (treeBuffer[j]->token == F_NOT && treeBuffer[j]->left == tmp1)
  	  		{
  	  			ftree->left = treeBuffer[j];
  	  			ftree->left->ident++;
  	  			break;
  	  		}
  	  	}
  	  	if(j==treeBufferLength)
  	  	{
  	  		if(tmp1->token==F_IDENT)
  	  		{
  	  			ftree->left = ftree_create(F_NOT,1,tmp1,NULL);
  				treeBufferLength++;
  				if(treeBufferLength>treeBufferLengthMax)
  				{
  					treeBufferLengthMax+=treeBufferStep;
  					if(treeBufferLengthMax>treeBufferStep)
  					{
  						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
  					}
  					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
  				}
  				treeBuffer[treeBufferLength-1] =  ftree->left;	  			
  	  		}
  	  		else
  	  		{
  	  			tmp1->ident++;
  	  			ftree->left = pushNegation(ftree_create(F_NOT,1,tmp1,NULL));
  	  		}
  	  	}
  	  	
  	  	
  	  	for (j=0;j<treeBufferLength;j++)
  	  	{
  	  		if (treeBuffer[j] == NULL) continue;
  	  		if (treeBuffer[j]->token == F_NOT && treeBuffer[j]->left == tmp2)
  	  		{
  	  			ftree->right = treeBuffer[j];
  	  			ftree->right->ident++;
  	  			break;
  	  		}
  	  	}
  	  	if(j==treeBufferLength)
  	  	{
  	  		if(tmp2->token==F_IDENT)
  	  		{
  	  			ftree->right = ftree_create(F_NOT,1,tmp2,NULL);
  				treeBufferLength++;
  				if(treeBufferLength>treeBufferLengthMax)
  				{
  					treeBufferLengthMax+=treeBufferStep;
  					if(treeBufferLengthMax>treeBufferStep)
  					{
  						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
  					}
  					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
  				}
  				treeBuffer[treeBufferLength-1] =  ftree->right;	  			
  	  		}
  	  		else
  	  		{
  	  			tmp2->ident++;
  	  			ftree->right = pushNegation(ftree_create(F_NOT,1,tmp2,NULL));
  	  		}
  	  	}	  	
  		}	
			for (j=0;j<treeBufferLength;j++)
			{
			  if (treeBuffer[j] == NULL) continue;
				if (treeBuffer[j]->token == dualToken && treeBuffer[j]->left ==ftree->left && treeBuffer[j]->right == ftree->right)
				{
					common_free(ftree, sizeof(FormulaTree));
					ftree = treeBuffer[j];
					if(ftree->left->token!=F_IDENT) ftree->left->ident--;
					if(ftree->right->token!=F_IDENT) ftree->right->ident--;			
					break;
				}
			}	
			if (j == treeBufferLength)
			{
				treeBufferLength++;
				if(treeBufferLength>treeBufferLengthMax)
				{
					treeBufferLengthMax+=treeBufferStep;
					if(treeBufferLengthMax>treeBufferStep)
					{
						treeBuffer = realloc(treeBuffer,treeBufferLengthMax*sizeof(FormulaTree));
					}
					else treeBuffer = malloc(treeBufferLengthMax*sizeof(FormulaTree)); 
				}
				treeBuffer[treeBufferLength-1] = ftree;	
			}  	
	  	return ftree;    		
		default: 
			common_error("unknown token (%i) in Positive Normal Form",ftree->left->token);
			return NULL;	
	}
	return NULL;
}

void build_ctl_formula (char *formula)
{
	if (mc_ctl_read_from_file)
	{
		if (!(ctlin = fopen(formula,"r")))
			common_error("CTL formula file %s not found", formula);		
		ctlparse();
		fclose(ctlin);  
	}
	else 
	{	
		ctlstr = formula;
		ctlparse();
	}

  free(treeBuffer);
	treeBuffer = NULL;
  treeBufferLength =treeBufferLengthMax =0;
	common_strbuf_new();
	CTLPrintFormula(ctlTree);
	printf("This is the CTL formula: %s\n",common_strbuf);
}
