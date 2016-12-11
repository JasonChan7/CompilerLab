/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

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
#define YYBISON_VERSION "3.0.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 1 "small.y" /* yacc.c:339  */

	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>
	#include <malloc.h>
	#include <memory.h>

	#define bool int
	#define true 1
	#define false 0

	// #define norw 13       /* ??保留字个数 */
	#define symbolTablePointmax 100     /* 符号表容量 */
	// #define nmax 14       /* ??数字的最大位数 */
	#define al 10         /* 标识符的最大长度 */
	// #define maxerr 30     /* ??允许的最多错误数 */
	#define amax 2048     /* 地址上界 */
	#define levmax 3      /* 最大允许过程嵌套声明层数 */
	#define codeTablePointmax 200     /* 最多的虚拟机代码数 */
	#define stacksize 500 /* 运行时数据栈元素最多为500个 */
	#define paraTableMax 200

	/* 符号表中的类型 */
	enum object {
		constant, 
		variable, 
		parameter,
		procedure,
	};
	
	/* 符号表结构 */
	struct tablestruct
	{
		char name[al];	    /* 名字 */
		enum object kind;	/* 类型：const，var或procedure */
		int val;            /* 数值，仅const使用 */
		int level;          /* 所处层，仅const不使用 */
		int adr;            /* 地址，仅const不使用 */
		int size;           /* 需要分配的数据区空间, 仅procedure使用 */
	};
	struct tablestruct table[symbolTablePointmax]; /* 符号表 */
	int paraTable[paraTableMax];

	/* 虚拟机代码指令 */
	enum fct {
		lit,     opr,     lod, 
		sto,     cal,     ini, 
		jmp,     jpc,     
	};
	#define fctnum 8

	/* 虚拟机代码结构 */
	struct instruction
	{
		enum fct f; /* 虚拟机代码指令 */
		int l;      /* 引用层与声明层的层次差 */
		int a;      /* 根据f的不同而不同 */
	};
	struct instruction code[codeTablePointmax]; /* 存放虚拟机代码的数组 */

	char id[al+1];      		/* 当前ident，多出的一个字节用于存放0 */
	int proctable[3];			/* 嵌套过程索引表，最多嵌套三层 */
	int symbolTablePoint;		/* 符号表当前尾指针 */
	int codeTablePoint;         /* 虚拟机代码指针, 取值范围[0, codeTablePointmax-1]*/
	int procTablePoint;			/* 嵌套过程索引表proctable的指针 */
	int lev = 0;				/* 记录当前所在层 */
	int num;            		/* 当前number */
	bool listswitch ;   		/* 显示虚拟机代码与否 */
	bool tableswitch ;  		/* 显示符号表与否 */
	int currParaCnt = 0;		/* 记录当前调用函数的所扫描到的参数个数 */

	FILE* fin;      /* 输入源文件 */
	FILE* ftable;	/* 输出符号表 */
	FILE* fcode;    /* 输出虚拟机代码 */
	FILE* foutput;  /* 输出文件及出错示意（如有错）、各行对应的生成代码首地址（如无错） */
	FILE* fresult;  /* 输出执行结果 */
	char fname[al];
	int err;        /* 错误计数器 */
	extern int line;
	
	int yylex(void);
	void yyerror(char *);
	void gen(enum fct x, int y, int z);
	int position(char* idt);
	void enter(enum object k);
	void interpret(void);
	int base(int l, int* s, int b);
	void init(void);
	void setdx(int n);
	void listall();
	void displaytable();

#line 159 "small.tab.c" /* yacc.c:339  */

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

/* In a future release of Bison, this section will be replaced
   by #include "small.tab.h".  */
#ifndef YY_YY_SMALL_TAB_H_INCLUDED
# define YY_YY_SMALL_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    IDENT = 258,
    INTEGER = 259,
    IF = 260,
    THEN = 261,
    ELSE = 262,
    END = 263,
    REPEAT = 264,
    UNTIL = 265,
    READ = 266,
    WRITE = 267,
    CALL = 268,
    CONST = 269,
    VAR = 270,
    BEGIN = 271,
    XOR = 272,
    ODD = 273,
    PROC = 274,
    WHILE = 275,
    DO = 276,
    IFX = 277,
    BC = 278,
    GT = 279,
    LT = 280,
    GE = 281,
    LE = 282,
    EQ = 283,
    NE = 284,
    UMINUS = 285
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 93 "small.y" /* yacc.c:355  */

	bool tf; /* boolean value */
	int integer; /* integer value */ 
	char *ident; /* identifier */ 

#line 236 "small.tab.c" /* yacc.c:355  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_SMALL_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 251 "small.tab.c" /* yacc.c:358  */

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
#define YYFINAL  4
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   114

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  40
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  42
/* YYNRULES -- Number of rules.  */
#define YYNRULES  73
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  137

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   285

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,    34,     2,     2,
      38,    39,    32,    30,    37,    31,     2,    33,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    36,
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
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    35
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   110,   110,   113,   119,   122,   113,   131,   134,   140,
     143,   148,   151,   156,   166,   169,   174,   177,   182,   191,
     194,   199,   206,   209,   214,   220,   228,   230,   233,   239,
     247,   248,   251,   252,   253,   254,   255,   256,   257,   260,
     260,   265,   267,   269,   265,   276,   281,   294,   308,   315,
     315,   323,   336,   339,   344,   356,   371,   374,   378,   382,
     386,   390,   394,   398,   404,   409,   414,   419,   424,   429,
     433,   438,   461,   466
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "IDENT", "INTEGER", "IF", "THEN", "ELSE",
  "END", "REPEAT", "UNTIL", "READ", "WRITE", "CALL", "CONST", "VAR",
  "BEGIN", "XOR", "ODD", "PROC", "WHILE", "DO", "IFX", "BC", "GT", "LT",
  "GE", "LE", "EQ", "NE", "'+'", "'-'", "'*'", "'/'", "'%'", "UMINUS",
  "';'", "','", "'('", "')'", "$accept", "program", "block", "@1", "$@2",
  "$@3", "get_table_addr", "get_code_addr", "const_decl", "const_list",
  "const_def", "var_decl", "var_list", "var_def", "proc_decls",
  "proc_decl", "para_list", "para_stmt", "proc_body",
  "increase_procTablePoint", "decrease_level", "increase_level",
  "stmt_sequence", "statement", "if_stmt", "$@4", "$@5", "$@6", "$@7",
  "repeat_stmt", "assign_stmt", "read_stmt", "write_stmt", "while_stmt",
  "$@8", "call_stmt", "arg_list", "arg_stmt", "expr", "simple_expr",
  "identifier", "proc_identifier", YY_NULLPTR
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
      43,    45,    42,    47,    37,   285,    59,    44,    40,    41
};
# endif

#define YYPACT_NINF -53

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-53)))

#define YYTABLE_NINF -28

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int8 yypact[] =
{
     -53,    15,   -53,   -53,   -53,    18,    14,    20,    -3,   -10,
     -53,    34,   -53,    43,   -53,    14,   -53,     9,   -53,   -53,
     -53,   -53,   -53,    34,    29,   -53,    79,    33,    32,   -53,
       3,   -53,    49,     3,    51,   -53,    19,   -53,   -53,   -53,
     -53,   -53,   -53,   -53,   -53,    45,   -53,    66,   -53,   -53,
       5,     5,     5,   -53,    76,    79,   -53,   -53,   -53,    35,
      36,    79,     3,   -53,    62,   -53,    -8,   -53,   -20,   -53,
       5,     5,     5,     5,     5,     5,     5,     5,     5,     5,
      -5,    49,     3,   -53,   -53,    39,   -53,    38,   -53,    65,
      71,    -8,    -8,    -8,    -8,    -8,    -8,    -4,    -4,   -53,
     -53,     3,    46,    50,   -53,    47,   -53,    86,    79,    79,
     -53,   -53,    49,   -53,   -53,    54,    57,    -6,    19,   -53,
     -53,    59,    93,   -53,   -53,    77,   -53,   -53,    90,    94,
     -53,    79,    79,     6,     8,   -53,   -53
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     2,     7,     1,    10,     0,    15,     0,     0,
      11,     0,     4,     0,     9,     0,    18,     0,    16,    20,
      13,    12,    14,     0,     5,    17,     0,     0,     0,    72,
       0,     8,     0,     0,     0,     8,     6,    31,    32,    33,
      34,    35,    36,    37,    38,     0,     3,     0,    71,    70,
       0,     0,     0,     8,    56,     0,    47,    48,    73,     0,
       0,     0,     0,    28,     0,    29,    57,    64,     0,    39,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    53,     0,    30,    46,     0,    19,     0,    69,     0,
       0,    62,    60,    61,    63,    58,    59,    65,    66,    67,
      68,     0,     0,    52,    54,     0,    26,    23,     0,     0,
      45,    51,     0,     8,    24,     0,    22,     0,     8,    55,
      49,     0,     0,    40,    42,     0,    21,    25,     0,     0,
      43,     0,     0,     0,     0,    50,    44
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -53,   -53,    67,   -53,   -53,   -53,   -53,   -35,   -53,   -53,
      96,   -53,   -53,    89,   -53,   -53,   -53,   -53,   -53,   -53,
     -53,   -53,   -52,    53,   -53,   -53,   -53,   -53,   -53,   -53,
     -53,   -53,   -53,   -53,   -53,   -53,   -53,   -53,   -29,   -12,
     -31,   -53
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     2,     3,    19,    26,     5,    55,     7,     9,
      10,    12,    17,    18,    24,    27,   115,   116,    64,    28,
      85,    87,    36,    37,    38,    89,    90,   128,   132,    39,
      40,    41,    42,    43,   125,    44,   102,   103,    53,    54,
      45,    59
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      60,    56,   123,    80,    57,   101,    48,    49,    48,    49,
      76,    77,    78,    79,   135,     4,   136,     8,    69,    88,
      13,    50,    76,    77,    78,    79,    14,    15,    78,    79,
      61,    61,     6,    84,    51,    11,    51,    16,    66,    67,
      68,    52,    61,    52,    61,    22,    23,    20,   -27,    46,
     104,    47,    29,   105,    58,    61,   117,   118,    91,    92,
      93,    94,    95,    96,    97,    98,    99,   100,    62,    65,
      86,   108,   110,    81,    82,   106,   107,   109,   120,   133,
     134,   119,    29,   124,    30,   111,   113,   112,    31,   114,
      32,    33,    34,   121,   122,   126,   127,   130,   129,    35,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
     131,    21,    25,    63,    83
};

static const yytype_uint8 yycheck[] =
{
      35,    32,     8,    55,    33,    10,     3,     4,     3,     4,
      30,    31,    32,    33,     8,     0,     8,     3,    53,    39,
      23,    18,    30,    31,    32,    33,    36,    37,    32,    33,
      36,    36,    14,    62,    31,    15,    31,     3,    50,    51,
      52,    38,    36,    38,    36,    36,    37,     4,    19,    16,
      81,    19,     3,    82,     3,    36,   108,   109,    70,    71,
      72,    73,    74,    75,    76,    77,    78,    79,    23,     3,
       8,     6,   101,    38,    38,    36,    38,     6,   113,   131,
     132,   112,     3,   118,     5,    39,    39,    37,     9,     3,
      11,    12,    13,    39,    37,    36,     3,     7,    21,    20,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      16,    15,    23,    46,    61
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    41,    42,    43,     0,    46,    14,    48,     3,    49,
      50,    15,    51,    23,    36,    37,     3,    52,    53,    44,
       4,    50,    36,    37,    54,    53,    45,    55,    59,     3,
       5,     9,    11,    12,    13,    20,    62,    63,    64,    69,
      70,    71,    72,    73,    75,    80,    16,    19,     3,     4,
      18,    31,    38,    78,    79,    47,    80,    78,     3,    81,
      47,    36,    23,    42,    58,     3,    79,    79,    79,    47,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      62,    38,    38,    63,    78,    60,     8,    61,    39,    65,
      66,    79,    79,    79,    79,    79,    79,    79,    79,    79,
      79,    10,    76,    77,    80,    78,    36,    38,     6,     6,
      78,    39,    37,    39,     3,    56,    57,    62,    62,    80,
      47,    39,    37,     8,    47,    74,    36,     3,    67,    21,
       7,    16,    68,    62,    62,     8,     8
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    40,    41,    43,    44,    45,    42,    46,    47,    48,
      48,    49,    49,    50,    51,    51,    52,    52,    53,    54,
      54,    55,    56,    56,    57,    57,    58,    59,    60,    61,
      62,    62,    63,    63,    63,    63,    63,    63,    63,    65,
      64,    66,    67,    68,    64,    69,    70,    71,    72,    74,
      73,    75,    76,    76,    77,    77,    78,    78,    78,    78,
      78,    78,    78,    78,    79,    79,    79,    79,    79,    79,
      79,    79,    80,    81
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     0,     0,     8,     0,     0,     3,
       0,     1,     3,     3,     3,     0,     1,     3,     1,     5,
       0,     8,     1,     0,     1,     3,     3,     0,     0,     0,
       3,     1,     1,     1,     1,     1,     1,     1,     1,     0,
       7,     0,     0,     0,    12,     5,     3,     2,     2,     0,
      11,     5,     1,     0,     1,     3,     1,     2,     3,     3,
       3,     3,     3,     3,     2,     3,     3,     3,     3,     3,
       1,     1,     1,     1
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
        case 3:
#line 113 "small.y" /* yacc.c:1646  */
    {
	table[symbolTablePoint].adr = codeTablePoint;
	(yyval.integer) = codeTablePoint;
	gen(jmp, 0, 0);
}
#line 1427 "small.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 119 "small.y" /* yacc.c:1646  */
    {
	  setdx((yyvsp[-2].integer)+(yyvsp[-1].integer));
  }
#line 1435 "small.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 122 "small.y" /* yacc.c:1646  */
    {
	code[(yyvsp[-5].integer)].a = codeTablePoint;

	// table[$2].adr = codeTablePoint;
	// table[$2].size = $4+3;
	// gen(ini, 0, $4+3);
	displaytable();
}
#line 1448 "small.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 131 "small.y" /* yacc.c:1646  */
    {
	(yyval.integer) = symbolTablePoint;
}
#line 1456 "small.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 134 "small.y" /* yacc.c:1646  */
    {
	(yyval.integer) = codeTablePoint; 
}
#line 1464 "small.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 140 "small.y" /* yacc.c:1646  */
    {
		(yyval.integer) = (yyvsp[-1].integer);
	}
#line 1472 "small.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 143 "small.y" /* yacc.c:1646  */
    {
		(yyval.integer) = 0;
	}
#line 1480 "small.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 148 "small.y" /* yacc.c:1646  */
    {
		(yyval.integer) = (yyvsp[0].integer);
	}
#line 1488 "small.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 151 "small.y" /* yacc.c:1646  */
    {
		(yyval.integer) = (yyvsp[-2].integer)+1;
	}
#line 1496 "small.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 156 "small.y" /* yacc.c:1646  */
    {
		strcpy(id, (yyvsp[-2].ident));
		num = (yyvsp[0].integer);
		enter(constant);
		(yyval.integer) = 1;
	}
#line 1507 "small.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 166 "small.y" /* yacc.c:1646  */
    {
		(yyval.integer) = (yyvsp[-1].integer);
	}
#line 1515 "small.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 169 "small.y" /* yacc.c:1646  */
    {
		(yyval.integer) = 0;
	}
#line 1523 "small.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 174 "small.y" /* yacc.c:1646  */
    {
		(yyval.integer) = (yyvsp[0].integer);
	}
#line 1531 "small.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 177 "small.y" /* yacc.c:1646  */
    {
		(yyval.integer) = (yyvsp[-2].integer)+(yyvsp[0].integer);
	}
#line 1539 "small.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 182 "small.y" /* yacc.c:1646  */
    {
		strcpy(id, (yyvsp[0].ident));
		enter(variable);
		(yyval.integer) = 1;
	}
#line 1549 "small.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 191 "small.y" /* yacc.c:1646  */
    {
		(yyval.integer) = (yyval.integer)1+1;
	}
#line 1557 "small.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 194 "small.y" /* yacc.c:1646  */
    {
		(yyval.integer) = 0;
	}
#line 1565 "small.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 199 "small.y" /* yacc.c:1646  */
    {
		strcpy(id, (yyvsp[-5].ident));
		enter(procedure);
		proctable[procTablePoint] = symbolTablePoint;
	}
#line 1575 "small.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 206 "small.y" /* yacc.c:1646  */
    {
		(yyval.integer) = (yyvsp[0].integer);
	}
#line 1583 "small.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 209 "small.y" /* yacc.c:1646  */
    {
		(yyval.integer) = 0;
	}
#line 1591 "small.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 214 "small.y" /* yacc.c:1646  */
    {
		(yyval.integer) = 1;
		strcpy(id, (yyvsp[0].ident));
		enter(parameter);
		gen(sto, lev-table[symbolTablePoint].level, table[symbolTablePoint].adr); /* 存变量 */
	}
#line 1602 "small.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 220 "small.y" /* yacc.c:1646  */
    {
		(yyval.integer) = (yyvsp[-2].integer)+1;
		strcpy(id, (yyvsp[0].ident));
		enter(parameter);
		gen(sto, lev-table[symbolTablePoint].level, table[symbolTablePoint].adr); /* 存变量 */
	}
#line 1613 "small.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 230 "small.y" /* yacc.c:1646  */
    {
	procTablePoint++;
}
#line 1621 "small.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 233 "small.y" /* yacc.c:1646  */
    {
	lev--;
}
#line 1629 "small.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 239 "small.y" /* yacc.c:1646  */
    {
	lev++;
	if (lev > levmax) {
		yyerror("level overflow");
	}
}
#line 1640 "small.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 260 "small.y" /* yacc.c:1646  */
    {
		gen(jpc, 0, 0);
	}
#line 1648 "small.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 262 "small.y" /* yacc.c:1646  */
    {
		code[(yyvsp[-4].integer)].a = codeTablePoint;
	}
#line 1656 "small.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 265 "small.y" /* yacc.c:1646  */
    {
		gen(jpc, 0, 0);
	}
#line 1664 "small.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 267 "small.y" /* yacc.c:1646  */
    {
		gen(jmp, 0, 0);
	}
#line 1672 "small.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 269 "small.y" /* yacc.c:1646  */
    {
		code[(yyvsp[-6].integer)].a = codeTablePoint;
	}
#line 1680 "small.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 271 "small.y" /* yacc.c:1646  */
    {
		code[(yyvsp[-6].integer)].a = codeTablePoint;
	}
#line 1688 "small.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 276 "small.y" /* yacc.c:1646  */
    {
		gen(jpc, 0, (yyvsp[-3].integer));
	}
#line 1696 "small.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 281 "small.y" /* yacc.c:1646  */
    {
		if ((yyvsp[-2].integer) == 0) {
			yyerror("undeclared variable");	/* 未声明标识符 */
			exit(1);
		}
		else if (table[(yyvsp[-2].integer)].kind != variable) {
			yyerror("%s is not a variable", table[(yyvsp[-2].ident)].name);	/* 标识符非变量 */
			exit(1);
		}
		gen(sto, lev-table[(yyvsp[-2].integer)].level, table[(yyvsp[-2].integer)].adr);
	}
#line 1712 "small.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 294 "small.y" /* yacc.c:1646  */
    {
		if ((yyvsp[0].integer) == 0) {
			yyerror("undeclared variable");	/* 未声明标识符 */
			exit(1);
		}
		else if (table[(yyvsp[0].integer)].kind != variable) {
			yyerror("%s is not a variable", table[(yyvsp[0].ident)].name);	/* 标识符非变量 */
			exit(1);
		}
		gen(opr, 0, 16); /* 读操作 */
		gen(sto, lev-table[(yyvsp[0].integer)].level, table[(yyvsp[0].integer)].adr); /* 存变量 */
	}
#line 1729 "small.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 308 "small.y" /* yacc.c:1646  */
    {
		gen(opr, 0, 14); /* 写操作 */
		gen(opr, 0, 15);
		(yyval.integer) = (yyvsp[0].integer);
	}
#line 1739 "small.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 315 "small.y" /* yacc.c:1646  */
    {
		gen(jpc, 0, 0);
	}
#line 1747 "small.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 317 "small.y" /* yacc.c:1646  */
    {
		gen(jmp, 0, (yyvsp[-9].integer))
		code[(yyvsp[-5].integer)].a = codeTablePoint;
	}
#line 1756 "small.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 323 "small.y" /* yacc.c:1646  */
    {
		if ((yyvsp[-3].integer) == 0) {
			yyerror("undeclared procedure");	/* 未声明标识符 */
			exit(1);
		}
		else if (table[(yyvsp[-3].integer)].size != (yyvsp[-1].integer)) {
			yyerror("wrong arguments");	/* 参数列表错误 */
			exit(1);
		}
		gen(cal, lev-table[(yyvsp[-3].integer)].level, table[(yyvsp[-3].integer)].adr);
	}
#line 1772 "small.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 336 "small.y" /* yacc.c:1646  */
    {
		(yyval.integer) = (yyvsp[0].integer);
	}
#line 1780 "small.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 339 "small.y" /* yacc.c:1646  */
    {
		(yyval.integer) = 0;
	}
#line 1788 "small.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 344 "small.y" /* yacc.c:1646  */
    {
		(yyval.integer) = 1;
		if ((yyvsp[0].integer) == 0) {
			yyerror("undeclared variable");	/* 未声明标识符 */
			exit(1);
		}
		else if (table[(yyvsp[0].integer)].kind != variable) {
			yyerror("%s is not a variable", table[(yyvsp[0].ident)].name);	/* 标识符是过程 */
			exit(1);
		}
		gen(lod, lev-table[(yyvsp[0].integer)].level, table[(yyvsp[0].integer)].adr); /* 取变量 */
	}
#line 1805 "small.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 356 "small.y" /* yacc.c:1646  */
    {
		(yyval.integer) = (yyvsp[-2].integer)+1;
		if ((yyvsp[0].integer) == 0) {
			yyerror("undeclared variable");	/* 未声明标识符 */
			exit(1);
		}
		else if (table[(yyvsp[0].integer)].kind != variable) {
			yyerror("%s is not a variable", table[(yyvsp[0].ident)].name);	/* 标识符是过程 */
			exit(1);
		}
		gen(lod, lev-table[(yyvsp[0].integer)].level, table[(yyvsp[0].integer)].adr); /* 取变量 */
	}
#line 1822 "small.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 371 "small.y" /* yacc.c:1646  */
    {
		(yyval.integer) = (yyvsp[0].integer)==0 ? 1:0;
	}
#line 1830 "small.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 374 "small.y" /* yacc.c:1646  */
    {
		gen(opr, 0, 6);
		(yyval.integer) = (yyvsp[0].integer)%2;
	}
#line 1839 "small.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 378 "small.y" /* yacc.c:1646  */
    {
		gen(opr, 0, 8);
		(yyval.integer) = (yyvsp[-2].integer)==(yyvsp[0].integer) ? 1:0;
	}
#line 1848 "small.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 382 "small.y" /* yacc.c:1646  */
    {
		gen(opr, 0, 9);
		(yyval.integer) = (yyvsp[-2].integer)!=(yyvsp[0].integer) ? 1:0;
	}
#line 1857 "small.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 386 "small.y" /* yacc.c:1646  */
    {
		gen(opr, 0, 10);
		(yyval.integer) = (yyvsp[-2].integer)<(yyvsp[0].integer) ? 1:0;
	}
#line 1866 "small.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 390 "small.y" /* yacc.c:1646  */
    {
		gen(opr, 0, 11);
		(yyval.integer) = (yyvsp[-2].integer)>(yyvsp[0].integer) ? 1:0;
	}
#line 1875 "small.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 394 "small.y" /* yacc.c:1646  */
    {
		gen(opr, 0, 12);
		(yyval.integer) = (yyvsp[-2].integer)>=(yyvsp[0].integer) ? 1:0;
	}
#line 1884 "small.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 398 "small.y" /* yacc.c:1646  */
    {
		gen(opr, 0, 13);
		(yyval.integer) = (yyvsp[-2].integer)<=(yyvsp[0].integer) ? 1:0;
	}
#line 1893 "small.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 404 "small.y" /* yacc.c:1646  */
    {
		gen(opr, 0, 1);
		(yyval.integer) = -1*(yyvsp[0].integer);
		fprintf(stdout, "%d\n", (yyval.integer));
	}
#line 1903 "small.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 409 "small.y" /* yacc.c:1646  */
    {
		gen(opr, 0, 2);
		(yyval.integer) = (yyvsp[-2].integer)+(yyvsp[0].integer);
		fprintf(stdout, "%d\n", (yyval.integer));
	}
#line 1913 "small.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 414 "small.y" /* yacc.c:1646  */
    {
		gen(opr, 0, 3);
		(yyval.integer) = (yyvsp[-2].integer)-(yyvsp[0].integer);
		fprintf(stdout, "%d\n", (yyval.integer));
	}
#line 1923 "small.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 419 "small.y" /* yacc.c:1646  */
    {
		gen(opr, 0, 4);
		(yyval.integer) = (yyvsp[-2].integer)*(yyvsp[0].integer);
		fprintf(stdout, "%d\n", (yyval.integer));
	}
#line 1933 "small.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 424 "small.y" /* yacc.c:1646  */
    {
		gen(opr, 0, 5);
		(yyval.integer) = (yyvsp[-2].integer)/(yyvsp[0].integer);
		fprintf(stdout, "%d\n", (yyval.integer));
	}
#line 1943 "small.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 429 "small.y" /* yacc.c:1646  */
    {
		(yyval.integer) = (yyvsp[-1].integer);
		fprintf(stdout, "%d\n", (yyval.integer));
	}
#line 1952 "small.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 433 "small.y" /* yacc.c:1646  */
    {
		gen(lit, 0, (yyvsp[0].integer));
		(yyval.integer) = (yyvsp[0].integer);
		fprintf(stdout, "%d\n", (yyval.integer));
	}
#line 1962 "small.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 438 "small.y" /* yacc.c:1646  */
    {
		int i = position((yyvsp[0].ident));	/* 查找标识符在符号表中的位置 */
		if (i==0) {
			yyerror("undefined variable");	/* 标识符未声明，只能使用本层的标识符 */
		}
		else {
			switch (table[i].kind) {
				case constant:	/* 标识符为常量 */
					gen(lit, 0, table[i].val);	/* 直接把常量的值入栈 */
					(yyval.integer) = table[i].val;
					break;
				case variable:	/* 标识符为变量 */
					gen(lod, lev-table[i].level, table[i].adr);	/* 找到变量地址并将其值入栈 */
					(yyval.integer) = getValue(i); /* 表达式的值即identifier的值 */
					break;
				case procedure:	/* 标识符为过程 */
					yyerror("cannot be procedure");	/* 不能为过程 */
					break;
			}
		}
	}
#line 1988 "small.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 461 "small.y" /* yacc.c:1646  */
    {
		(yyval.integer) = position((yyvsp[0].ident));
	}
#line 1996 "small.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 466 "small.y" /* yacc.c:1646  */
    {
		(yyval.integer) = proc_position((yyvsp[0].ident));
	}
#line 2004 "small.tab.c" /* yacc.c:1646  */
    break;


#line 2008 "small.tab.c" /* yacc.c:1646  */
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
#line 469 "small.y" /* yacc.c:1906  */

void yyerror(char *s) {
	err++;
	fprintf(stdout, "line %d error: %s\n", line, s);
}
void enter(enum object k) {
	symbolTablePoint++;
	strcpy(table[symbolTablePoint].name, id); /* 符号表的name域记录标识符的名字 */
	table[symbolTablePoint].kind = k;	
	switch (k) {
		case constant:	/* 常量 */
			table[symbolTablePoint].val = num; /* 登记常数的值 */
			table[symbolTablePoint].level = lev;
			break;
		case variable:	/* 变量 */
			table[symbolTablePoint].val = 0; /* 变量默认初始值为0 */
			table[symbolTablePoint].level = lev;
			break;
		case procedure:	/* 过程 */
			table[symbolTablePoint].level = lev;
			break;
		case parameter: /* 参数 */
			table[symbolTablePoint].level = lev;
			break;
	}
}
void init() {
	symbolTablePoint = 0;
	codeTablePoint = 0;
	procTablePoint = 0;
	lev = 0;
	proctable[0] = 0;
	num = 0;
	err = 0;
}
int position(char* id) {
	int i = symbolTablePoint;
	strcpy(table[0].name, id);
	while (strcmp(table[i].name, id)!=0 || table[i].level!=lev || table[i].kind==procedure) {
        i--;
    }
	return i;
}
int proc_position(char* id) {
	int i = symbolTablePoint;
	strcpy(table[0].name, id);
	while (strcmp(table[i].name, id)!=0 || table[i].kind!=procedure) {
        i--;
    }
	return i;
}
void setdx(int n) {
	int i;
	for (i=1; i<=n; i++) {
		table[symbolTablePoint-i+1] = n-i+3;
	}
}
void gen(enum fct x, int y, int z) {
	if (codeTablePoint >= codeTablePointmax) {
		yyerror("Program is too long!");	/* 生成的虚拟机代码程序过长 */
		exit(1);
	}
	if (z >= amax) {
		yyerror("Displacement address is too big!");	/* 地址偏移越界 */
		exit(1);
	}
	code[codeTablePoint].f = x;
	code[codeTablePoint].l = y;
	code[codeTablePoint].a = z;
	codeTablePoint++;
}
void listall() {
	int i;
	char name[][5] = {
		{"lit"}, {"opr"}, {"lod"}, {"sto"}, {"cal"}, {"int"}, {"jmp"}, {"jpc"},
	};
	if (listswitch) {
		for (i = 0; i < codeTablePoint; i++) {
			printf("%d %s %d %d\n", i, name[code[i].f], code[i].l, code[i].a);
			fprintf(fcode, "%d %s %d %d\n", i, name[code[i].f], code[i].l, code[i].a);		
		}
	}
}
void displaytable() {
	int i;
	if (tableswitch) {
		for (i = 1; i <= symbolTablePoint; i++) {
			switch(table[i].kind) {
				case constant:
					printf("    %d const %s ", i, table[i].name);
					printf("val=%d\n", table[i].val);
					fprintf(ftable, "    %d const %s ", i, table[i].name);
					fprintf(ftable, "val=%d\n", table[i].val);
					break;
				case variable:
					printf("    %d var   %s ", i, table[i].name);
					printf("lev=%d addr=%d\n", table[i].level, table[i].adr);
					fprintf(ftable, "    %d var   %s ", i, table[i].name);
					fprintf(ftable, "lev=%d addr=%d\n", table[i].level, table[i].adr);
					break;
				case procedure:
					printf("    %d proc  %s ", i, table[i].name);
					printf("lev=%d addr=%d size=%d\n", table[i].level, table[i].adr, table[i].size);
					fprintf(ftable,"    %d proc  %s ", i, table[i].name);
					fprintf(ftable,"lev=%d addr=%d size=%d\n", table[i].level, table[i].adr, table[i].size);
					break;
			}
		}
		printf("\n");
		fprintf(ftable,"\n");
	}
}
void interpret()
{
	int p = 0; /* 指令指针 */
	int b = 1; /* 指令基址 */
	int t = 0; /* 栈顶指针 */
	struct instruction i;	/* 存放当前指令 */
	int s[stacksize];	/* 栈 */

	printf("Start small\n");
	fprintf(fresult,"Start small\n");
	s[0] = 0; /* s[0]不用 */
	s[1] = 0; /* 主程序的三个联系单元均置为0 */
	s[2] = 0;
	s[3] = 0;
	do {
	    i = code[p];	/* 读当前指令 */
		p = p + 1;
		switch (i.f)
		{
			case lit:	/* 将常量a的值取到栈顶 */
				t = t + 1;
				s[t] = i.a;				
				break;
			case opr:	/* 数学、逻辑运算 */
				switch (i.a)
				{
					case 0:  /* 函数调用结束后返回 */
						t = b - 1;
						p = s[t + 3];
						b = s[t + 2];
						break;
					case 1: /* 栈顶元素取反 */
						s[t] = - s[t];
						break;
					case 2: /* 次栈顶项加上栈顶项，退两个栈元素，相加值进栈 */
						t = t - 1;
						s[t] = s[t] + s[t + 1];
						break;
					case 3:/* 次栈顶项减去栈顶项 */
						t = t - 1;
						s[t] = s[t] - s[t + 1];
						break;
					case 4:/* 次栈顶项乘以栈顶项 */
						t = t - 1;
						s[t] = s[t] * s[t + 1];
						break;
					case 5:/* 次栈顶项除以栈顶项 */
						t = t - 1;
						s[t] = s[t] / s[t + 1];
						break;
					case 6:/* 栈顶元素的奇偶判断 */
						s[t] = s[t] % 2;
						break;
					case 8:/* 次栈顶项与栈顶项是否相等 */
						t = t - 1;
						s[t] = (s[t] == s[t + 1]);
						break;
					case 9:/* 次栈顶项与栈顶项是否不等 */
						t = t - 1;
						s[t] = (s[t] != s[t + 1]);
						break;
					case 10:/* 次栈顶项是否小于栈顶项 */
						t = t - 1;
						s[t] = (s[t] < s[t + 1]);
						break;
					case 11:/* 次栈顶项是否大于等于栈顶项 */
						t = t - 1;
						s[t] = (s[t] >= s[t + 1]);
						break;
					case 12:/* 次栈顶项是否大于栈顶项 */
						t = t - 1;
						s[t] = (s[t] > s[t + 1]);
						break;
					case 13: /* 次栈顶项是否小于等于栈顶项 */
						t = t - 1;
						s[t] = (s[t] <= s[t + 1]);
						break;
					case 14:/* 栈顶值输出 */
						printf("%d", s[t]);
						fprintf(fresult, "%d", s[t]);
						t = t - 1;
						break;
					case 15:/* 输出换行符 */
						printf("\n");
						fprintf(fresult,"\n");
						break;
					case 16:/* 读入一个输入置于栈顶 */
						t = t + 1;
						printf("?");
						fprintf(fresult, "?");
						scanf("%d", &(s[t]));
						fprintf(fresult, "%d\n", s[t]);						
						break;
				}
				break;
			case lod:	/* 取相对当前过程的数据基地址为a的内存的值到栈顶 */
				t = t + 1;
				s[t] = s[base(i.l,s,b) + i.a];				
				break;
			case sto:	/* 栈顶的值存到相对当前过程的数据基地址为a的内存 */
				s[base(i.l, s, b) + i.a] = s[t];
				t = t - 1;
				break;
			case cal:	/* 调用子过程 */
				s[t + 1] = base(i.l, s, b);	/* 将父过程基地址入栈，即建立静态链 */
				s[t + 2] = b;	/* 将本过程基地址入栈，即建立动态链 */
				s[t + 3] = p;	/* 将当前指令指针入栈，即保存返回地址 */
				b = t + 1;	/* 改变基地址指针值为新过程的基地址 */
				p = i.a;	/* 跳转 */
				break;
			case ini:	/* 在数据栈中为被调用的过程开辟a个单元的数据区 */
				t = t + i.a;	
				break;
			case jmp:	/* 直接跳转 */
				p = i.a;
				break;
			case jpc:	/* 条件跳转 */
				if (s[t] == 0) 
					p = i.a;
				t = t - 1;
				break;
		}
	} while (p != 0);
	printf("End pl0\n");
	fprintf(fresult,"End pl0\n");
}
int base(int l, int* s, int b)
{
	int b1;
	b1 = b;
	while (l > 0)
	{
		b1 = s[b1];
		l--;
	}
	return b1;
}

/* 主程序开始 */
int main()
{
	printf("Input small test file?   ");
	scanf("%s", fname);		/* 输入文件名 */

	if ((fin = fopen(fname, "r")) == NULL)
	{
		printf("Can't open the input file!\n");
		exit(1);
	}

	if ((foutput = fopen("foutput.txt", "w")) == NULL)
	{
		printf("Can't open the output file!\n");
		exit(1);
	}

	if ((ftable = fopen("ftable.txt", "w")) == NULL)
	{
		printf("Can't open ftable.txt file!\n");
		exit(1);
	}

	printf("List object codes?(Y/N)");	/* 是否输出虚拟机代码 */
	scanf("%s", fname);
	listswitch = (fname[0]=='y' || fname[0]=='Y');

	printf("List symbol table?(Y/N)");	/* 是否输出符号表 */
	scanf("%s", fname);
	tableswitch = (fname[0]=='y' || fname[0]=='Y');        
	
	redirectInput(fin);
    init();		/* 初始化 */
	yyparse();
    if (err == 0) {
		printf("\n===Parsing success!===\n");
		fprintf(foutput,"\n===Parsing success!===\n");

		if ((fcode = fopen("fcode.txt", "w")) == NULL)
		{
			printf("Can't open fcode.txt file!\n");
			exit(1);
		}		

		if ((fresult = fopen("fresult.txt", "w")) == NULL)
		{
			printf("Can't open fresult.txt file!\n");
			exit(1);
		}
		
		listall();	 /* 输出所有代码 */		
		fclose(fcode);

        interpret();	/* 调用解释执行程序 */        	
		fclose(fresult);
    }
	else {
		printf("\n%d errors in pl/0 program!\n",err);
		fprintf(foutput,"\n%d errors in pl/0 program!\n",err);
	}
		
    fclose(ftable);
	fclose(foutput);
	fclose(fin);
	return 0;
}

