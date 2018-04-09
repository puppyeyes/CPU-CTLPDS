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
#define yyparse         bpparse
#define yylex           bplex
#define yyerror         bperror
#define yydebug         bpdebug
#define yynerrs         bpnerrs

#define yylval          bplval
#define yychar          bpchar

/* Copy the first part of user declarations.  */
#line 1 "bp.y" /* yacc.c:339  */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bpstruc.h"
#include "common.h"
#include "abpds.h"

int bplex();
void yyerror(char*);
extern int line;

bp_ident_t bp_globals;
bp_ident_t bp_parms;
bp_fun_t bp_functions;
bp_tree_t bp_nametree;
bp_tree_t bp_funtree;
int bp_maxrets;
int bp_numglobals;
int bp_maxlocs;
int bp_labelcount;

short bp_funcount;
int bp_declcount;
char bp_doing_parms;
bp_ident_t bp_decls;
bp_ident_t *bp_dtail;
bp_tree_t bp_labeltree;

bp_ident_t bp_locids;
bp_ident_t bp_globids;
bp_ident_t bp_allglobids;


#line 110 "bp.c" /* yacc.c:339  */

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
extern int bpdebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    IDENTIFIER = 258,
    INTEGER = 259,
    NONDET = 260,
    ASSIGN = 261,
    EQ_OP = 262,
    NE_OP = 263,
    IMPLIES = 264,
    DECL = 265,
    ENFORCE = 266,
    AND = 267,
    OR = 268,
    IF = 269,
    THEN = 270,
    ELSE = 271,
    ELSIF = 272,
    FI = 273,
    WHILE = 274,
    DO = 275,
    OD = 276,
    RETURN = 277,
    SKIP = 278,
    GOTO = 279,
    BEG = 280,
    END = 281,
    BOOL = 282,
    VOID = 283,
    ASSERT = 284,
    ASSUME = 285,
    PRINT = 286,
    CHOOSE = 287,
    CONSTRAIN = 288,
    DEAD = 289
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 37 "bp.y" /* yacc.c:355  */

	bp_stmt_t stmt;
	bp_expr_t expr;
	bp_ident_t ident;
	bp_idref_t idref;
	bp_fun_t fun;
	char* c;
	int   i;
	ABPDSStackList labels;

#line 193 "bp.c" /* yacc.c:355  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE bplval;

int bpparse (void);



/* Copy the second part of user declarations.  */

#line 210 "bp.c" /* yacc.c:358  */

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
#define YYFINAL  3
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   219

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  48
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  40
/* YYNRULES -- Number of rules.  */
#define YYNRULES  87
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  177

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   289

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    44,     2,     2,     2,     2,     2,    42,
      39,    40,     2,     2,    36,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    41,    35,
      37,     2,    38,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    46,     2,    47,    45,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,    43,     2,     2,     2,
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
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    83,    83,    89,   105,   112,   120,   121,   124,   125,
     127,   145,   146,   156,   198,   203,   204,   205,   207,   208,
     210,   214,   215,   220,   221,   225,   266,   273,   280,   281,
     282,   283,   284,   285,   286,   287,   288,   289,   292,   314,
     319,   347,   348,   351,   359,   364,   369,   375,   381,   388,
     397,   404,   413,   415,   417,   427,   435,   440,   445,   449,
     454,   463,   470,   485,   493,   502,   506,   507,   511,   512,
     515,   516,   518,   522,   523,   527,   528,   532,   533,   537,
     538,   540,   542,   546,   549,   550,   555,   561
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "IDENTIFIER", "INTEGER", "NONDET",
  "ASSIGN", "EQ_OP", "NE_OP", "IMPLIES", "DECL", "ENFORCE", "AND", "OR",
  "IF", "THEN", "ELSE", "ELSIF", "FI", "WHILE", "DO", "OD", "RETURN",
  "SKIP", "GOTO", "BEG", "END", "BOOL", "VOID", "ASSERT", "ASSUME",
  "PRINT", "CHOOSE", "CONSTRAIN", "\"dead\"", "';'", "','", "'<'", "'>'",
  "'('", "')'", "':'", "'\\''", "'~'", "'!'", "'^'", "'['", "']'",
  "$accept", "program", "pstart", "global_decls", "decl_start",
  "decl_list", "identifier_list", "decl_id", "fun_list",
  "function_definition", "fun_type", "enforce", "parms", "parameter_list",
  "statement_list", "labelled_statement", "label_list", "statement",
  "parallel_assign", "assign_id_list", "asgn_id", "assert", "print",
  "call", "selection_statement", "elsif_list", "iteration_statement",
  "jump_statement", "dead_statement", "primary_expression",
  "unary_expression", "unary_operator", "equality_expression",
  "and_expression", "exclusive_or_expression", "or_expression",
  "expression", "decider", "choose_expression", "expression_list", YY_NULLPTR
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
     285,   286,   287,   288,   289,    59,    44,    60,    62,    40,
      41,    58,    39,   126,    33,    94,    91,    93
};
# endif

#define YYPACT_NINF -148

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-148)))

#define YYTABLE_NINF -24

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -148,    68,  -148,  -148,    -6,    72,    64,  -148,  -148,    -6,
     111,   112,  -148,   116,  -148,  -148,  -148,    -1,  -148,    83,
      85,    97,    72,   112,  -148,   112,     1,  -148,  -148,    92,
      89,  -148,   119,  -148,    36,  -148,  -148,  -148,  -148,    36,
     128,  -148,   129,  -148,  -148,    22,    98,   121,    93,   126,
      20,   114,    67,   113,    -7,  -148,  -148,  -148,    69,    69,
      69,    69,    69,    69,  -148,  -148,  -148,    21,    47,   102,
      14,  -148,   142,   109,   110,   115,   117,   147,  -148,   118,
      18,  -148,   120,   122,   123,   124,   125,   127,   130,  -148,
    -148,  -148,  -148,    98,   121,    93,   126,     5,  -148,    36,
     143,   136,    36,   131,   143,  -148,   132,   133,    36,    36,
      14,    36,  -148,   134,  -148,    27,   147,  -148,  -148,  -148,
    -148,  -148,  -148,  -148,  -148,    58,    -7,   135,  -148,   138,
      36,    14,   158,   139,   140,    59,   141,   137,  -148,   -13,
    -148,  -148,   148,   101,   144,     7,  -148,  -148,  -148,  -148,
    -148,  -148,   145,  -148,  -148,   146,  -148,  -148,  -148,    36,
      36,   101,   149,    36,   150,    -5,    -2,  -148,  -148,   151,
    -148,  -148,  -148,   157,  -148,   101,  -148
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     5,     1,     0,     7,    15,    17,     2,    11,
       0,     0,     4,     0,    12,     5,    10,     0,     8,     0,
       0,     0,     7,     0,    16,    22,     7,     6,     9,    21,
       0,    14,    19,    20,     0,    27,    62,    64,    81,     0,
       0,    68,    69,    66,    70,     0,    73,    75,    77,    79,
       0,     0,    27,     0,     0,    63,    82,    67,     0,     0,
       0,     0,     0,     0,    18,    13,    24,    43,     0,     0,
      56,    58,     0,     0,     0,     0,     0,     0,    25,     0,
       0,    41,     0,     0,     0,    33,    32,     0,     0,    65,
      69,    71,    72,    74,    76,    78,    80,     0,    26,     0,
      83,     0,     0,     0,    84,    86,    57,    59,     0,     0,
       0,     0,    43,    61,    28,     0,     0,    29,    30,    31,
      35,    34,    36,    37,    49,     0,     0,     0,    27,     0,
       0,     0,     0,     0,     0,     0,     0,    62,    39,    38,
      42,    48,     0,     0,     0,     0,    87,    60,    44,    45,
      47,    46,     0,    27,    27,     0,    52,    51,    27,     0,
       0,     0,     0,     0,     0,     0,     0,    50,    53,     0,
      55,    85,    40,     0,    27,     0,    54
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -148,  -148,  -148,  -148,   159,    78,   161,   160,   164,  -148,
    -148,  -148,  -148,  -148,   -51,  -148,  -148,  -148,  -148,   105,
      50,  -148,  -148,    73,  -148,  -147,  -148,  -148,  -148,   152,
      51,  -148,   153,   154,   155,   156,   -34,   -96,    56,   -23
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,     4,     5,    12,    17,    18,     8,     9,
      10,    35,    21,    30,    51,    52,    53,    78,    79,    80,
      81,    82,    83,    84,    85,   157,    86,    87,    88,    43,
      44,    45,    46,    47,    48,    49,   100,   101,   105,   106
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      50,    66,    63,   127,    63,    54,   129,    63,    36,    37,
      38,    11,   133,   134,   167,   136,    63,    36,    37,    38,
     152,     6,     7,   131,   115,    36,    37,    31,   176,    63,
     137,    37,    38,    89,    22,    23,   104,   103,   172,    36,
      37,    38,   171,   159,    39,   124,   103,    40,    41,    42,
      36,    37,    38,    39,   116,    64,    40,    41,    42,   103,
      97,    39,    98,   104,    40,   126,    39,   169,     3,    40,
      41,    42,    36,    37,   125,    39,   104,   143,    40,    41,
      42,   104,    11,   -23,   -23,   -23,    99,   135,   -23,    40,
      41,    42,   139,   -23,   131,   131,   145,   104,   141,   150,
      27,    13,   161,   162,    32,    58,    59,   164,    39,    91,
      92,    40,    41,    90,    15,    16,    67,   154,   155,   156,
      19,    24,    26,   175,    25,   165,   166,    68,    23,    33,
      34,    55,    69,    60,    56,    70,    71,    72,    61,    62,
      65,   102,    73,    74,    75,   107,    76,    77,   108,   109,
     112,   128,    63,   114,   110,   117,   111,   118,   119,   120,
     121,   147,   122,   153,   158,   123,   140,   168,   131,   132,
     116,   170,   174,    14,    20,   142,    97,   130,   144,   148,
     149,   151,   113,    28,   160,   163,    29,   146,   138,     0,
       0,   173,     0,     0,     0,     0,     0,    57,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    93,     0,    94,     0,    95,     0,    96
};

static const yytype_int16 yycheck[] =
{
      34,    52,     9,    99,     9,    39,   102,     9,     3,     4,
       5,    10,   108,   109,   161,   111,     9,     3,     4,     5,
      33,    27,    28,    36,     6,     3,     4,    26,   175,     9,
       3,     4,     5,    40,    35,    36,    70,    32,    40,     3,
       4,     5,    47,    36,    39,    40,    32,    42,    43,    44,
       3,     4,     5,    39,    36,    35,    42,    43,    44,    32,
      39,    39,    41,    97,    42,    99,    39,   163,     0,    42,
      43,    44,     3,     4,    97,    39,   110,   128,    42,    43,
      44,   115,    10,    16,    17,    18,    39,   110,    21,    42,
      43,    44,   115,    26,    36,    36,   130,   131,    40,    40,
      22,    37,   153,   154,    26,     7,     8,   158,    39,    58,
      59,    42,    43,    44,     3,     3,     3,    16,    17,    18,
       4,    38,    25,   174,    39,   159,   160,    14,    36,    40,
      11,     3,    19,    12,     5,    22,    23,    24,    45,    13,
      26,    39,    29,    30,    31,     3,    33,    34,    39,    39,
       3,    15,     9,    35,    39,    35,    39,    35,    35,    35,
      35,     3,    35,    15,    20,    35,   116,    18,    36,    36,
      36,    21,    15,     9,    15,    40,    39,    46,    40,    40,
      40,    40,    77,    23,    39,    39,    25,   131,   115,    -1,
      -1,    40,    -1,    -1,    -1,    -1,    -1,    45,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    60,    -1,    61,    -1,    62,    -1,    63
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    49,    50,     0,    51,    52,    27,    28,    56,    57,
      58,    10,    53,    37,    56,     3,     3,    54,    55,     4,
      52,    60,    35,    36,    38,    39,    25,    53,    55,    54,
      61,    26,    53,    40,    11,    59,     3,     4,     5,    39,
      42,    43,    44,    77,    78,    79,    80,    81,    82,    83,
      84,    62,    63,    64,    84,     3,     5,    77,     7,     8,
      12,    45,    13,     9,    35,    26,    62,     3,    14,    19,
      22,    23,    24,    29,    30,    31,    33,    34,    65,    66,
      67,    68,    69,    70,    71,    72,    74,    75,    76,    40,
      44,    78,    78,    80,    81,    82,    83,    39,    41,    39,
      84,    85,    39,    32,    84,    86,    87,     3,    39,    39,
      39,    39,     3,    67,    35,     6,    36,    35,    35,    35,
      35,    35,    35,    35,    40,    87,    84,    85,    15,    85,
      46,    36,    36,    85,    85,    87,    85,     3,    71,    87,
      68,    40,    40,    62,    40,    84,    86,     3,    40,    40,
      40,    40,    33,    15,    16,    17,    18,    73,    20,    36,
      39,    62,    62,    39,    62,    84,    84,    73,    18,    85,
      21,    47,    40,    40,    15,    62,    73
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    48,    49,    50,    51,    52,    53,    53,    54,    54,
      55,    56,    56,    57,    57,    58,    58,    58,    59,    59,
      60,    61,    61,    62,    62,    63,    64,    64,    65,    65,
      65,    65,    65,    65,    65,    65,    65,    65,    66,    66,
      66,    67,    67,    68,    69,    69,    69,    70,    71,    71,
      72,    72,    73,    73,    73,    74,    75,    75,    75,    75,
      75,    76,    77,    77,    77,    77,    78,    78,    79,    79,
      80,    80,    80,    81,    81,    82,    82,    83,    83,    84,
      84,    84,    84,    85,    86,    86,    87,    87
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     3,     0,     2,     0,     4,     0,     1,     3,
       1,     1,     2,     8,     5,     1,     4,     1,     3,     0,
       4,     1,     0,     1,     2,     2,     3,     0,     2,     2,
       2,     2,     1,     1,     2,     2,     2,     2,     3,     3,
       7,     1,     3,     1,     4,     4,     4,     4,     4,     3,
       7,     5,     1,     3,     7,     7,     1,     2,     1,     2,
       4,     2,     1,     2,     1,     3,     1,     2,     1,     1,
       1,     3,     3,     1,     3,     1,     3,     1,     3,     1,
       3,     1,     2,     1,     1,     6,     1,     3
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
#line 84 "bp.y" /* yacc.c:1646  */
    {
		bp_globals = (yyvsp[-1].ident);
		bp_functions = (yyvsp[0].fun);
	  }
#line 1422 "bp.c" /* yacc.c:1646  */
    break;

  case 3:
#line 89 "bp.y" /* yacc.c:1646  */
    {
		bp_funcount = -1;
		bp_nametree = NULL;
		bp_funtree = NULL;
		bp_labelcount = 0;

		bp_maxlocs = 0;
		bp_maxrets = 0;
		bp_locids = NULL;
		bp_globids = NULL;
		bp_allglobids = NULL;
	  }
#line 1439 "bp.c" /* yacc.c:1646  */
    break;

  case 4:
#line 106 "bp.y" /* yacc.c:1646  */
    {
		(yyval.ident) = bp_decls;
		bp_numglobals = bp_declcount;
	  }
#line 1448 "bp.c" /* yacc.c:1646  */
    break;

  case 5:
#line 112 "bp.y" /* yacc.c:1646  */
    {
		bp_declcount = 0;
		bp_funcount++;
		bp_decls = NULL;
		bp_doing_parms = 1;
	  }
#line 1459 "bp.c" /* yacc.c:1646  */
    break;

  case 10:
#line 128 "bp.y" /* yacc.c:1646  */
    {
		bp_ident_t newid = alloc_bp_ident();
		newid->varname = name_create((yyvsp[0].c));
		newid->index = bp_funcount? bp_doing_parms?
									bp_assign_parm_index(newid->varname,bp_funcount)
								  : bp_assign_loc_index(newid->varname,bp_funcount)
								  : bp_assign_glob_index(newid->varname);
		newid->fun = bp_funcount;
		bp_insert_decl(&bp_nametree,
				newid->varname,bp_funcount,newid);
		newid->next = bp_decls;
		bp_decls = newid;
	  }
#line 1477 "bp.c" /* yacc.c:1646  */
    break;

  case 12:
#line 147 "bp.y" /* yacc.c:1646  */
    { 
	    if((yyvsp[-1].fun))
	    { 
	      (yyval.fun) = (yyvsp[-1].fun); (yyval.fun)->next = (yyvsp[0].fun); 
	    }
	    else (yyval.fun)=(yyvsp[0].fun);
	  }
#line 1489 "bp.c" /* yacc.c:1646  */
    break;

  case 13:
#line 157 "bp.y" /* yacc.c:1646  */
    {
		int i;

		(yyval.fun) = alloc_bp_fun();
		(yyval.fun)->funname = name_create((yyvsp[-6].c));
		(yyval.fun)->num_returns = (yyvsp[-7].i);
		(yyval.fun)->num_args = (yyvsp[-5].i);
		(yyval.fun)->num_locals = bp_declcount;
		(yyval.fun)->num_globals = bp_numglobals;
		(yyval.fun)->locals = bp_decls;
		(yyval.fun)->parms = bp_parms;
		(yyval.fun)->enforce = (yyvsp[-2].expr);
		(yyval.fun)->stmt = (yyvsp[-1].stmt);
		(yyval.fun)->next = NULL;

		(yyval.fun)->returns = NULL;
		(yyval.fun)->callbuf = NULL;

		for (i = 0; i < (yyval.fun)->num_returns; i++)
		{
			char tmp[16];
			bp_ident_t newid = alloc_bp_ident();
			sprintf(tmp,"*ret%d",i+1);
			newid->varname = name_create(tmp);
			newid->index = bp_assign_ret_index(newid->varname,i);
			newid->fun = 0;
			newid->next = (yyval.fun)->returns;
			(yyval.fun)->returns = newid;
		}

		for (i = 0; i < (yyval.fun)->num_locals; i++)
		{
			bp_ident_t newid = alloc_bp_ident();
			newid->index = -1;
			newid->fun = 0;
			newid->next = (yyval.fun)->callbuf;
			
			(yyval.fun)->callbuf = newid;
		}
			bp_insert_decl(&bp_funtree,(yyval.fun)->funname,-1,(bp_ident_t)(yyval.fun));
	  }
#line 1535 "bp.c" /* yacc.c:1646  */
    break;

  case 14:
#line 199 "bp.y" /* yacc.c:1646  */
    {  (yyval.fun)=NULL;  }
#line 1541 "bp.c" /* yacc.c:1646  */
    break;

  case 15:
#line 203 "bp.y" /* yacc.c:1646  */
    { (yyval.i) = 1; }
#line 1547 "bp.c" /* yacc.c:1646  */
    break;

  case 16:
#line 204 "bp.y" /* yacc.c:1646  */
    { (yyval.i) = (yyvsp[-1].i); }
#line 1553 "bp.c" /* yacc.c:1646  */
    break;

  case 17:
#line 205 "bp.y" /* yacc.c:1646  */
    { (yyval.i) = 0; }
#line 1559 "bp.c" /* yacc.c:1646  */
    break;

  case 18:
#line 207 "bp.y" /* yacc.c:1646  */
    { (yyval.expr) = (yyvsp[-1].expr); }
#line 1565 "bp.c" /* yacc.c:1646  */
    break;

  case 19:
#line 208 "bp.y" /* yacc.c:1646  */
    { (yyval.expr) = NULL; }
#line 1571 "bp.c" /* yacc.c:1646  */
    break;

  case 20:
#line 211 "bp.y" /* yacc.c:1646  */
    { (yyval.i) = bp_declcount; bp_parms = bp_decls; bp_doing_parms = 0; }
#line 1577 "bp.c" /* yacc.c:1646  */
    break;

  case 24:
#line 222 "bp.y" /* yacc.c:1646  */
    {	(yyval.stmt) = (yyvsp[-1].stmt); (yyval.stmt)->next = (yyvsp[0].stmt);		}
#line 1583 "bp.c" /* yacc.c:1646  */
    break;

  case 25:
#line 226 "bp.y" /* yacc.c:1646  */
    {    	     
      (yyval.stmt) = (yyvsp[0].stmt);   
    	if((yyvsp[-1].labels)==NULL)
    	{
    	 (yyval.stmt)->label = -(++bp_labelcount);
    	}
    	else 
    	{
    	  ABPDSStackList aa=(yyvsp[-1].labels);
    	  while((yyvsp[-1].labels)->next)
    	  {(yyvsp[-1].labels)=(yyvsp[-1].labels)->next;}
    	  (yyval.stmt)->label=(yyvsp[-1].labels)->stack;
    	/*  if($1->next==NULL)
    	  {
    	    $$=$2;
    	    ABPDSDestoryStackList($1);
    	  }
    	  else
    	  {
    	    ABPDSStackList aa=$1;
    	    bp_stmt_t tmp1=$2;
    	    bp_stmt_t tmp2;
    	  	while($1->next)
    	  	{
    	       new_bp_stmt(tmp2,BP_SKIP);
    	       tmp2->label=$1->next->stack;
    	       tmp2->next=tmp1;
    	       tmp1=tmp2;
    	       $1=$1->next;
    	  	}
    	  	$$=tmp1;
    	  	for(;aa; aa=aa->next)
    	  	{printf("\n %s \n", name_cstring(aa->stack));}
    	  	}*/
    	 ABPDSDestoryStackList(aa);
    	  
    	} 
    }
#line 1626 "bp.c" /* yacc.c:1646  */
    break;

  case 26:
#line 267 "bp.y" /* yacc.c:1646  */
    {
	  			(yyval.labels) = common_malloc(sizeof(struct ABPDSStackListStruct));
					(yyval.labels)->stack = name_create((yyvsp[-1].c));
					(yyval.labels)->next =(yyvsp[-2].labels);
		  }
#line 1636 "bp.c" /* yacc.c:1646  */
    break;

  case 27:
#line 273 "bp.y" /* yacc.c:1646  */
    {
	  		(yyval.labels) = NULL;
		  }
#line 1644 "bp.c" /* yacc.c:1646  */
    break;

  case 38:
#line 293 "bp.y" /* yacc.c:1646  */
    {
		bp_idref_t tmp1, tmp2, tmp3;

		new_bp_stmt((yyval.stmt),BP_ASSIGN);

		for (tmp1 = (yyvsp[-2].idref), tmp2 = (yyvsp[0].idref); tmp1 && tmp2; tmp1 = tmp1->next)
		{
			tmp1->expr = tmp2->expr;
			tmp3 = tmp2->next;
			free(tmp2);
			tmp2 = tmp3;
		}

		if (tmp1 || tmp2)
		{
			yyerror("number of assign clauses doesn't match");
			YYERROR;
		}

		(yyval.stmt)->e.a.asgnlist = (yyvsp[-2].idref);
	  }
#line 1670 "bp.c" /* yacc.c:1646  */
    break;

  case 39:
#line 315 "bp.y" /* yacc.c:1646  */
    { 
		(yyval.stmt) = (yyvsp[0].stmt);
		(yyval.stmt)->e.a.asgnlist = (yyvsp[-2].idref);
	  }
#line 1679 "bp.c" /* yacc.c:1646  */
    break;

  case 40:
#line 320 "bp.y" /* yacc.c:1646  */
    {
	  bp_idref_t tmp1, tmp2, tmp3;

		new_bp_stmt((yyval.stmt),BP_ASSIGN);

		for (tmp1 = (yyvsp[-6].idref), tmp2 = (yyvsp[-4].idref); tmp1 && tmp2; tmp1 = tmp1->next)
		{
			tmp1->expr = tmp2->expr;
			tmp3 = tmp2->next;
			free(tmp2);
			tmp2 = tmp3;
		}

		if (tmp1 || tmp2)
		{
			yyerror("number of assign clauses doesn't match");
			YYERROR;
		}

		(yyval.stmt)->e.a.asgnlist = (yyvsp[-6].idref);
		bp_stmt_t tmp_const;
		new_bp_stmt(tmp_const,BP_CONSTRAIN);
		tmp_const->e.c.expr = (yyvsp[-1].expr);
		(yyval.stmt)->next=tmp_const;
	  }
#line 1709 "bp.c" /* yacc.c:1646  */
    break;

  case 42:
#line 349 "bp.y" /* yacc.c:1646  */
    {	(yyval.idref) = (yyvsp[0].idref); (yyval.idref)->next = (yyvsp[-2].idref);	}
#line 1715 "bp.c" /* yacc.c:1646  */
    break;

  case 43:
#line 352 "bp.y" /* yacc.c:1646  */
    {
		(yyval.idref) = alloc_bp_idref();
		(yyval.idref)->ident = bp_find_decl(name_create((yyvsp[0].c)),bp_funcount);
		(yyval.idref)->primed = 1;
		(yyval.idref)->next = NULL;
	  }
#line 1726 "bp.c" /* yacc.c:1646  */
    break;

  case 44:
#line 360 "bp.y" /* yacc.c:1646  */
    {
		new_bp_stmt((yyval.stmt),BP_ASSERT);
		(yyval.stmt)->e.c.expr = (yyvsp[-1].expr);
	  }
#line 1735 "bp.c" /* yacc.c:1646  */
    break;

  case 45:
#line 365 "bp.y" /* yacc.c:1646  */
    {
		new_bp_stmt((yyval.stmt),BP_ASSUME);
		(yyval.stmt)->e.c.expr = (yyvsp[-1].expr);
	  }
#line 1744 "bp.c" /* yacc.c:1646  */
    break;

  case 46:
#line 370 "bp.y" /* yacc.c:1646  */
    {
		new_bp_stmt((yyval.stmt),BP_CONSTRAIN);
		(yyval.stmt)->e.c.expr = (yyvsp[-1].expr);
	  }
#line 1753 "bp.c" /* yacc.c:1646  */
    break;

  case 47:
#line 376 "bp.y" /* yacc.c:1646  */
    {
		new_bp_stmt((yyval.stmt),BP_PRINT);
		(yyval.stmt)->e.a.asgnlist = (yyvsp[-1].idref);
	  }
#line 1762 "bp.c" /* yacc.c:1646  */
    break;

  case 48:
#line 382 "bp.y" /* yacc.c:1646  */
    {
		new_bp_stmt((yyval.stmt),BP_CALL);
		(yyval.stmt)->e.a.label = name_create((yyvsp[-3].c));
		(yyval.stmt)->e.a.asgnlist = NULL;
		(yyval.stmt)->e.a.fmllist = (yyvsp[-1].idref);
	  }
#line 1773 "bp.c" /* yacc.c:1646  */
    break;

  case 49:
#line 389 "bp.y" /* yacc.c:1646  */
    {
		new_bp_stmt((yyval.stmt),BP_CALL);
		(yyval.stmt)->e.a.label = name_create((yyvsp[-2].c));
		(yyval.stmt)->e.a.asgnlist = NULL;
		(yyval.stmt)->e.a.fmllist = NULL;
	  }
#line 1784 "bp.c" /* yacc.c:1646  */
    break;

  case 50:
#line 398 "bp.y" /* yacc.c:1646  */
    {
		new_bp_stmt((yyval.stmt),BP_IF);
		(yyval.stmt)->e.c.expr = (yyvsp[-4].expr);
		(yyval.stmt)->e.c.thenstmt = (yyvsp[-1].stmt);
		(yyval.stmt)->e.c.elsestmt = (yyvsp[0].stmt);
	  }
#line 1795 "bp.c" /* yacc.c:1646  */
    break;

  case 51:
#line 405 "bp.y" /* yacc.c:1646  */
    {
		new_bp_stmt((yyval.stmt),BP_IF);
		(yyval.stmt)->e.c.expr = (yyvsp[-3].expr);
		(yyval.stmt)->e.c.thenstmt = (yyvsp[-1].stmt);
		(yyval.stmt)->e.c.elsestmt = (yyvsp[0].stmt);
	  }
#line 1806 "bp.c" /* yacc.c:1646  */
    break;

  case 52:
#line 414 "bp.y" /* yacc.c:1646  */
    {	(yyval.stmt) = NULL;	}
#line 1812 "bp.c" /* yacc.c:1646  */
    break;

  case 53:
#line 416 "bp.y" /* yacc.c:1646  */
    {	(yyval.stmt) = (yyvsp[-1].stmt);	}
#line 1818 "bp.c" /* yacc.c:1646  */
    break;

  case 54:
#line 418 "bp.y" /* yacc.c:1646  */
    {
		new_bp_stmt((yyval.stmt),BP_IF);
		(yyval.stmt)->label = -(++bp_labelcount);
		(yyval.stmt)->e.c.expr = (yyvsp[-4].expr);
		(yyval.stmt)->e.c.thenstmt = (yyvsp[-1].stmt);
		(yyval.stmt)->e.c.elsestmt = (yyvsp[0].stmt);
	  }
#line 1830 "bp.c" /* yacc.c:1646  */
    break;

  case 55:
#line 428 "bp.y" /* yacc.c:1646  */
    {
		new_bp_stmt((yyval.stmt),BP_WHILE);
		(yyval.stmt)->e.c.expr = (yyvsp[-4].expr);
		(yyval.stmt)->e.c.thenstmt = (yyvsp[-1].stmt);
	  }
#line 1840 "bp.c" /* yacc.c:1646  */
    break;

  case 56:
#line 436 "bp.y" /* yacc.c:1646  */
    {
		new_bp_stmt((yyval.stmt),BP_RETURN);
		(yyval.stmt)->e.a.asgnlist = NULL;
	  }
#line 1849 "bp.c" /* yacc.c:1646  */
    break;

  case 57:
#line 441 "bp.y" /* yacc.c:1646  */
    {
		new_bp_stmt((yyval.stmt),BP_RETURN);
		(yyval.stmt)->e.a.asgnlist = (yyvsp[0].idref);
	  }
#line 1858 "bp.c" /* yacc.c:1646  */
    break;

  case 58:
#line 446 "bp.y" /* yacc.c:1646  */
    {
		new_bp_stmt((yyval.stmt),BP_SKIP);
	  }
#line 1866 "bp.c" /* yacc.c:1646  */
    break;

  case 59:
#line 450 "bp.y" /* yacc.c:1646  */
    {
		new_bp_stmt((yyval.stmt),BP_GOTO);
		(yyval.stmt)->e.a.label = name_create((yyvsp[0].c));
	  }
#line 1875 "bp.c" /* yacc.c:1646  */
    break;

  case 60:
#line 455 "bp.y" /* yacc.c:1646  */
    {
		new_bp_stmt((yyval.stmt),BP_GOTO);
		(yyval.stmt)->e.a.label = name_create((yyvsp[-2].c));
		(yyval.stmt)->e.a.label2=name_create((yyvsp[0].c));
		(yyval.stmt)->e.a.flag=12341234;
	  }
#line 1886 "bp.c" /* yacc.c:1646  */
    break;

  case 61:
#line 464 "bp.y" /* yacc.c:1646  */
    {
	   new_bp_stmt((yyval.stmt),BP_SKIP);
	 }
#line 1894 "bp.c" /* yacc.c:1646  */
    break;

  case 62:
#line 471 "bp.y" /* yacc.c:1646  */
    { 
		if ((*(yyvsp[0].c) == 'T' || *(yyvsp[0].c) == 'F') && !(yyvsp[0].c)[1])
		{
			new_bp_expr((yyval.expr),BP_CONST,(bp_expr_t)(*(yyvsp[0].c) == 'T'),NULL);
		}
		else
		{
			bp_idref_t ref = alloc_bp_idref();
			ref->ident =
			   bp_find_var(bp_nametree,name_create((yyvsp[0].c)),bp_funcount);
			ref->primed = 0;
			new_bp_expr((yyval.expr),BP_IDENT,(bp_expr_t)ref,NULL);
		}
	  }
#line 1913 "bp.c" /* yacc.c:1646  */
    break;

  case 63:
#line 486 "bp.y" /* yacc.c:1646  */
    {
		bp_idref_t ref = alloc_bp_idref();
		ref->ident =
			bp_find_var(bp_nametree,name_create((yyvsp[0].c)),bp_funcount);
		ref->primed = 1;
		new_bp_expr((yyval.expr),BP_IDENT,(bp_expr_t)ref,NULL);
	  }
#line 1925 "bp.c" /* yacc.c:1646  */
    break;

  case 64:
#line 494 "bp.y" /* yacc.c:1646  */
    { 
		if ((yyvsp[0].i) == 1 || (yyvsp[0].i) ==0) {
			new_bp_expr((yyval.expr),BP_CONST,(bp_expr_t)(yyvsp[0].i),NULL);
		} else {
		    yyerror("Boolean constants are 0 and 1\n");
		    YYERROR;
		}
	  }
#line 1938 "bp.c" /* yacc.c:1646  */
    break;

  case 65:
#line 503 "bp.y" /* yacc.c:1646  */
    {	(yyval.expr) = (yyvsp[-1].expr);	}
#line 1944 "bp.c" /* yacc.c:1646  */
    break;

  case 67:
#line 508 "bp.y" /* yacc.c:1646  */
    {	new_bp_expr((yyval.expr),BP_NOT,(yyvsp[0].expr),NULL);		}
#line 1950 "bp.c" /* yacc.c:1646  */
    break;

  case 71:
#line 517 "bp.y" /* yacc.c:1646  */
    {	new_bp_expr((yyval.expr),BP_EQ,(yyvsp[-2].expr),(yyvsp[0].expr));	}
#line 1956 "bp.c" /* yacc.c:1646  */
    break;

  case 72:
#line 519 "bp.y" /* yacc.c:1646  */
    {	new_bp_expr((yyval.expr),BP_NE,(yyvsp[-2].expr),(yyvsp[0].expr));	}
#line 1962 "bp.c" /* yacc.c:1646  */
    break;

  case 74:
#line 524 "bp.y" /* yacc.c:1646  */
    {	new_bp_expr((yyval.expr),BP_AND,(yyvsp[-2].expr),(yyvsp[0].expr));	}
#line 1968 "bp.c" /* yacc.c:1646  */
    break;

  case 76:
#line 529 "bp.y" /* yacc.c:1646  */
    {	new_bp_expr((yyval.expr),BP_NE,(yyvsp[-2].expr),(yyvsp[0].expr));	}
#line 1974 "bp.c" /* yacc.c:1646  */
    break;

  case 78:
#line 534 "bp.y" /* yacc.c:1646  */
    {	new_bp_expr((yyval.expr),BP_OR,(yyvsp[-2].expr),(yyvsp[0].expr));	}
#line 1980 "bp.c" /* yacc.c:1646  */
    break;

  case 80:
#line 539 "bp.y" /* yacc.c:1646  */
    {	new_bp_expr((yyval.expr),BP_IMP,(yyvsp[-2].expr),(yyvsp[0].expr));	}
#line 1986 "bp.c" /* yacc.c:1646  */
    break;

  case 81:
#line 541 "bp.y" /* yacc.c:1646  */
    {	new_bp_expr((yyval.expr),BP_ND,NULL,NULL);	}
#line 1992 "bp.c" /* yacc.c:1646  */
    break;

  case 82:
#line 543 "bp.y" /* yacc.c:1646  */
    {	new_bp_expr((yyval.expr),BP_ND,NULL,NULL);	}
#line 1998 "bp.c" /* yacc.c:1646  */
    break;

  case 85:
#line 551 "bp.y" /* yacc.c:1646  */
    {	new_bp_expr((yyval.expr),BP_CHOOSE,(yyvsp[-3].expr),(yyvsp[-1].expr));	}
#line 2004 "bp.c" /* yacc.c:1646  */
    break;

  case 86:
#line 556 "bp.y" /* yacc.c:1646  */
    {
		(yyval.idref) = alloc_bp_idref();
		(yyval.idref)->expr = (yyvsp[0].expr);
		(yyval.idref)->next = NULL;
	  }
#line 2014 "bp.c" /* yacc.c:1646  */
    break;

  case 87:
#line 562 "bp.y" /* yacc.c:1646  */
    {
		(yyval.idref) = alloc_bp_idref();
		(yyval.idref)->expr = (yyvsp[0].expr);
		(yyval.idref)->next = (yyvsp[-2].idref);
	  }
#line 2024 "bp.c" /* yacc.c:1646  */
    break;


#line 2028 "bp.c" /* yacc.c:1646  */
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
#line 568 "bp.y" /* yacc.c:1906  */


#include "bplex.c"

extern int column;

void yyerror(char *s)
{
	fflush(stdout);
	printf("Line %d: syntax error, token: %s.\n",line, bptext);
}

void parse_bp (char *filename)
{
	line = column = 1;
	if (!(bpin = fopen(filename,"r")))
		common_error("program file %s not found",filename);
	if (bpparse())
		common_error("parse errors encountered");
	fclose(bpin);
  
	/* translate bp tree into pds */
	bp_create_pds();
}
