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
	#define levmax 10      /* 最大允许过程嵌套声明层数 */
	#define codeTablePointmax 200     /* 最多的虚拟机代码数 */
	#define stacksize 500 /* 运行时数据栈元素最多为500个 */
	#define paraTableMax 200
	#define PROCMAX 20
	#define GLOBALMAX 20
	#define LOCALMAX 50

	/* 符号表中的类型 */
	enum object {
		constant, 
		variable, 
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
		jmp,     jpc,     ret,
		res,     put,     get,
	};
	#define fctnum 12

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

	struct tablestruct procRecord[PROCMAX];	/* 记录程序中的函数 */
	int procRecordPtr = 0; 		/* 记录程序中函数的个数 */
	struct tablestruct globalRecord[PROCMAX];	/* 记录程序中全局变量 */
	int globalRecordPtr = 0;
	struct tablestruct localRecord[PROCMAX];	/* 记录程序中局部变量 */
	int localRecordPtr = 0;
	bool globallocalswitch = 1;	/* 1表示全局变量，0表示局部变量 */
	bool procDeclFinished = 0;	/* 1表示结束，0表示未结束 */
	int argPtr = 0;

	FILE* fin;      /* 输入源文件 */
	FILE* ftable;	/* 输出符号表 */
	FILE* fcode;    /* 输出虚拟机代码 */
	FILE* foutput;  /* 输出文件及出错示意（如有错）、各行对应的生成代码首地址（如无错） */
	FILE* fresult;  /* 输出执行结果 */
	char fname[al];
	int err;        /* 错误计数器 */
	extern int line;
	
	int yylex(void);
	void gen(enum fct x, int y, int z);
	int position(char* idt);
	void enter(enum object k);
	void interpret(void);
	void init(void);
	void setdx(int n);
	void listall();
	void displaytable();
	void outputStack(int i, int *s, int t);

#line 171 "small.tab.c" /* yacc.c:339  */

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
    MYBEGIN = 271,
    XOR = 272,
    ODD = 273,
    PROC = 274,
    WHILE = 275,
    DO = 276,
    RET = 277,
    IFX = 278,
    BC = 279,
    GT = 280,
    LT = 281,
    GE = 282,
    LE = 283,
    EQ = 284,
    NE = 285,
    UMINUS = 286
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 105 "small.y" /* yacc.c:355  */

	int integer; /* integer value */ 
	char *ident; /* identifier */ 

#line 248 "small.tab.c" /* yacc.c:355  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_SMALL_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 263 "small.tab.c" /* yacc.c:358  */

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
#define YYLAST   204

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  41
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  45
/* YYNRULES -- Number of rules.  */
#define YYNRULES  81
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  160

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   286

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,    35,     2,     2,
      39,    40,    33,    31,    38,    32,     2,    34,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,    37,
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
      25,    26,    27,    28,    29,    30,    36
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   123,   123,   130,   123,   143,   146,   149,   152,   155,
     159,   160,   163,   166,   171,   174,   179,   187,   190,   195,
     198,   203,   211,   211,   217,   222,   222,   230,   250,   253,
     258,   266,   275,   278,   281,   289,   290,   295,   296,   297,
     298,   299,   300,   301,   302,   305,   310,   310,   321,   321,
     330,   335,   383,   409,   416,   416,   424,   438,   441,   446,
     460,   485,   488,   492,   496,   500,   504,   508,   512,   518,
     523,   528,   533,   538,   543,   548,   553,   557,   560,   565,
     597,   602
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "IDENT", "INTEGER", "IF", "THEN", "ELSE",
  "END", "REPEAT", "UNTIL", "READ", "WRITE", "CALL", "CONST", "VAR",
  "MYBEGIN", "XOR", "ODD", "PROC", "WHILE", "DO", "RET", "IFX", "BC", "GT",
  "LT", "GE", "LE", "EQ", "NE", "'+'", "'-'", "'*'", "'/'", "'%'",
  "UMINUS", "';'", "','", "'('", "')'", "$accept", "program", "$@1", "$@2",
  "get_code_addr", "get_table_addr", "open_globallocalswitch",
  "close_globallocalswitch", "get_globallocalswitch", "proc_predecls",
  "const_decl", "const_list", "const_def", "var_decl", "var_list",
  "var_def", "proc_decls", "$@3", "return_stmt", "$@4", "proc_decl",
  "para_list", "para_stmt", "increase_procRecord", "decrease_level",
  "increase_level", "stmt_sequence", "statement", "if_stmt", "$@5",
  "if_stmt_no_else", "@6", "repeat_stmt", "assign_stmt", "read_stmt",
  "write_stmt", "while_stmt", "$@7", "call_stmt", "arg_list", "arg_stmt",
  "expr", "simple_expr", "identifier", "proc_identifier", YY_NULLPTR
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
     285,    43,    45,    42,    47,    37,   286,    59,    44,    40,
      41
};
# endif

#define YYPACT_NINF -80

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-80)))

#define YYTABLE_NINF -26

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -80,    10,   -80,   -80,   -80,     3,   -80,    12,     7,    14,
      19,     9,    28,   -80,     8,   -32,   -80,    33,   -80,   -80,
     -80,    35,   -80,    19,   -80,   -23,   -80,   -80,     1,   -80,
     -80,   -80,    33,    14,    40,   -80,    31,   -80,    11,    21,
     -80,     7,   184,    40,    16,     9,   -80,     5,   -80,    45,
       5,   -80,   -80,   -80,    29,    17,   -80,   -80,    55,   -80,
     -80,   -80,   -80,   -80,   -80,    43,   -80,   -80,   -80,   -80,
     -80,    13,    13,     5,   -80,   -80,   113,   184,   -80,   -80,
      65,    37,     5,   184,   -80,   -80,     5,   -80,   184,    68,
     -80,    30,   -80,    13,    13,    13,    13,    13,    13,    13,
      13,    13,    13,    13,    13,    41,   -80,    38,     5,    44,
     -80,    67,   -80,   -80,    46,   -80,    66,   -80,    68,    68,
      68,    68,    68,    68,    -6,    -6,    69,    69,    69,   146,
       5,    49,   -80,   184,   184,   184,     5,    50,    56,   -80,
     -80,    47,    96,    70,   -80,   -80,     5,   -80,   159,   -80,
      53,   -80,    84,   -80,   -80,    93,   184,    73,   172,   -80
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       5,     0,     2,     1,     7,     0,    11,    32,    13,     0,
       0,    18,     0,    33,     0,     0,    14,     0,     8,    34,
      10,     0,    12,     0,    21,     0,    19,    24,     0,    16,
      15,    17,     0,    32,    29,    20,     0,     3,    30,     0,
      28,    13,    25,     0,     0,    18,    80,     0,     5,     0,
       0,     6,     5,    44,     0,     4,    36,    37,     5,    38,
      39,    40,    41,    42,    43,     0,    31,     9,    22,    79,
      78,     0,     0,     0,    77,     5,    61,    25,    52,    53,
       0,     0,     0,    25,    45,    46,     0,     5,    25,    62,
      69,     0,    48,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    81,     0,     0,     0,
      35,     0,    51,    27,     0,    76,     0,    75,    67,    65,
      66,    68,    63,    64,    70,    71,    72,    73,    74,    25,
      58,     0,    26,    25,    25,    25,     0,     0,    57,    59,
       5,     0,    44,     0,    50,    56,     0,    54,    25,    33,
      49,    60,     0,    47,    23,     0,    25,     0,    25,    55
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -80,   -80,   -80,   -80,   -45,   -80,   -80,   -80,   -80,   -80,
      71,   -80,    88,    72,   -80,    81,   -80,   -80,   -19,   -80,
      98,   -80,    89,   100,   -15,   -80,   -76,   -79,   -80,   -80,
     -80,   -80,   -80,   -80,   -80,   -80,   -80,   -80,   -42,   -80,
     -80,   -48,    25,    86,   -80
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,     4,    42,     2,    80,     5,    27,    87,     7,
      11,    15,    16,    18,    25,    26,    33,    88,    53,    54,
      13,    39,    40,     9,    20,    28,    55,    56,    57,   111,
      58,   116,    59,    60,    61,    62,    63,   152,    74,   137,
     138,    75,    76,    65,   107
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      64,   105,    79,    77,   110,    22,    23,    81,    69,    70,
       3,    93,   114,    85,    31,    32,    69,    70,    51,     6,
       8,    10,    14,    71,    17,    91,    51,   102,   103,   104,
      92,    19,    21,    12,   109,    64,    24,    72,   112,    29,
      34,    64,   113,    38,    73,    72,    64,    41,    46,    43,
     110,    82,    73,    67,    83,   110,    46,   141,    47,   143,
     131,    44,    48,    84,    49,    50,    51,    86,   106,   110,
     115,   110,   135,    52,   133,   -25,   108,   130,   129,   110,
     157,   132,   139,   134,   148,    93,    93,    64,   144,   140,
     145,    64,    64,    64,   146,   147,    89,    90,   151,   100,
     101,   102,   103,   104,   149,   155,    64,   150,    64,   156,
     158,    30,    45,    35,    64,   142,    64,    68,   117,   118,
     119,   120,   121,   122,   123,   124,   125,   126,   127,   128,
      93,    36,    66,    37,   154,    78,     0,     0,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,    46,
       0,    47,     0,     0,     0,    48,   136,    49,    50,    51,
       0,     0,    46,     0,    47,     0,    52,   153,    48,     0,
      49,    50,    51,     0,     0,    46,     0,    47,     0,    52,
     159,    48,     0,    49,    50,    51,     0,    46,     0,    47,
       0,     0,    52,    48,     0,    49,    50,    51,     0,     0,
       0,     0,     0,     0,    52
};

static const yytype_int16 yycheck[] =
{
      42,    77,    50,    48,    83,    37,    38,    52,     3,     4,
       0,    17,    88,    58,    37,    38,     3,     4,    13,    16,
       8,    14,     3,    18,    15,    73,    13,    33,    34,    35,
      75,     3,    24,    19,    82,    77,     3,    32,    86,     4,
      39,    83,    87,     3,    39,    32,    88,    16,     3,    38,
     129,    22,    39,    37,    37,   134,     3,   133,     5,   135,
     108,    40,     9,     8,    11,    12,    13,    24,     3,   148,
      40,   150,     6,    20,     7,    22,    39,    39,    37,   158,
     156,    37,   130,    37,    37,    17,    17,   129,   136,    40,
      40,   133,   134,   135,    38,   140,    71,    72,   146,    31,
      32,    33,    34,    35,     8,    21,   148,    37,   150,    16,
      37,    23,    41,    32,   156,   134,   158,    45,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
      17,    33,    43,    33,   149,    49,    -1,    -1,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,     3,
      -1,     5,    -1,    -1,    -1,     9,    10,    11,    12,    13,
      -1,    -1,     3,    -1,     5,    -1,    20,     8,     9,    -1,
      11,    12,    13,    -1,    -1,     3,    -1,     5,    -1,    20,
       8,     9,    -1,    11,    12,    13,    -1,     3,    -1,     5,
      -1,    -1,    20,     9,    -1,    11,    12,    13,    -1,    -1,
      -1,    -1,    -1,    -1,    20
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    42,    45,     0,    43,    47,    16,    50,     8,    64,
      14,    51,    19,    61,     3,    52,    53,    15,    54,     3,
      65,    24,    37,    38,     3,    55,    56,    48,    66,     4,
      53,    37,    38,    57,    39,    56,    61,    64,     3,    62,
      63,    16,    44,    38,    40,    51,     3,     5,     9,    11,
      12,    13,    20,    59,    60,    67,    68,    69,    71,    73,
      74,    75,    76,    77,    79,    84,    63,    37,    54,     3,
       4,    18,    32,    39,    79,    82,    83,    45,    84,    82,
      46,    45,    22,    37,     8,    45,    24,    49,    58,    83,
      83,    82,    45,    17,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    67,     3,    85,    39,    82,
      68,    70,    82,    45,    67,    40,    72,    83,    83,    83,
      83,    83,    83,    83,    83,    83,    83,    83,    83,    37,
      39,    82,    37,     7,    37,     6,    10,    80,    81,    82,
      40,    67,    59,    67,    82,    40,    38,    45,    37,     8,
      37,    82,    78,     8,    65,    21,    16,    67,    37,     8
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    41,    43,    44,    42,    45,    46,    47,    48,    49,
      50,    50,    51,    51,    52,    52,    53,    54,    54,    55,
      55,    56,    58,    57,    57,    60,    59,    61,    62,    62,
      63,    63,    64,    65,    66,    67,    67,    68,    68,    68,
      68,    68,    68,    68,    68,    69,    70,    69,    72,    71,
      73,    74,    75,    76,    78,    77,    79,    80,    80,    81,
      81,    82,    82,    82,    82,    82,    82,    82,    82,    83,
      83,    83,    83,    83,    83,    83,    83,    83,    83,    83,
      84,    85
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     0,    13,     0,     0,     0,     0,     0,
       4,     0,     3,     0,     1,     3,     3,     3,     0,     1,
       3,     1,     0,    11,     0,     0,     4,     9,     1,     0,
       1,     3,     0,     0,     0,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     2,     0,     7,     0,     7,
       6,     3,     2,     2,     0,    12,     6,     1,     0,     1,
       3,     1,     2,     3,     3,     3,     3,     3,     3,     2,
       3,     3,     3,     3,     3,     3,     3,     1,     1,     1,
       1,     1
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
#line 123 "small.y" /* yacc.c:1646  */
    {
		gen(jmp, 0, 0);
	}
#line 1466 "small.tab.c" /* yacc.c:1646  */
    break;

  case 3:
#line 130 "small.y" /* yacc.c:1646  */
    {
		code[(yyvsp[-10].integer)].a = codeTablePoint;
		procRecord[procRecordPtr].adr = codeTablePoint;
		strcpy(procRecord[procRecordPtr].name, "___main__");
		displaytable();
		procDeclFinished = 1;
		printf("fk: %d\n", (yyvsp[-4].integer));
		gen(ini, 0, 3);
	}
#line 1480 "small.tab.c" /* yacc.c:1646  */
    break;

  case 4:
#line 138 "small.y" /* yacc.c:1646  */
    {
		gen(opr, 0, 0);
	}
#line 1488 "small.tab.c" /* yacc.c:1646  */
    break;

  case 5:
#line 143 "small.y" /* yacc.c:1646  */
    {
	(yyval.integer) = codeTablePoint; 
}
#line 1496 "small.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 146 "small.y" /* yacc.c:1646  */
    {
	(yyval.integer) = localRecordPtr;
}
#line 1504 "small.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 149 "small.y" /* yacc.c:1646  */
    {
	globallocalswitch = 1;
}
#line 1512 "small.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 152 "small.y" /* yacc.c:1646  */
    {
	globallocalswitch = 0;
}
#line 1520 "small.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 155 "small.y" /* yacc.c:1646  */
    {
	(yyval.integer) = globallocalswitch;
}
#line 1528 "small.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 163 "small.y" /* yacc.c:1646  */
    {
		(yyval.integer) = (yyvsp[-1].integer);
	}
#line 1536 "small.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 166 "small.y" /* yacc.c:1646  */
    {
		(yyval.integer) = 0;
	}
#line 1544 "small.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 171 "small.y" /* yacc.c:1646  */
    {
		(yyval.integer) = 1;
	}
#line 1552 "small.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 174 "small.y" /* yacc.c:1646  */
    {
		(yyval.integer) = (yyvsp[-2].integer)+1;
	}
#line 1560 "small.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 179 "small.y" /* yacc.c:1646  */
    {
		strcpy(id, (yyvsp[-2].ident));
		num = (yyvsp[0].integer);
		enter(constant);
		(yyval.integer) = 1;
	}
#line 1571 "small.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 187 "small.y" /* yacc.c:1646  */
    {
		(yyval.integer) = (yyvsp[-1].integer);
	}
#line 1579 "small.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 190 "small.y" /* yacc.c:1646  */
    {
		(yyval.integer) = 0;
	}
#line 1587 "small.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 195 "small.y" /* yacc.c:1646  */
    {
		(yyval.integer) = (yyvsp[0].integer);
	}
#line 1595 "small.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 198 "small.y" /* yacc.c:1646  */
    {
		(yyval.integer) = (yyvsp[-2].integer)+(yyvsp[0].integer);
	}
#line 1603 "small.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 203 "small.y" /* yacc.c:1646  */
    {
		strcpy(id, (yyvsp[0].ident));
		num = 0;
		enter(variable);
		(yyval.integer) = 1;
	}
#line 1614 "small.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 211 "small.y" /* yacc.c:1646  */
    {
		gen(ini, 0, 3);
	}
#line 1622 "small.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 213 "small.y" /* yacc.c:1646  */
    {
		(yyval.integer) = (yyvsp[-10].integer)+1;
		gen(opr, 0, 0);
	}
#line 1631 "small.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 217 "small.y" /* yacc.c:1646  */
    {
		(yyval.integer) = 0;
	}
#line 1639 "small.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 222 "small.y" /* yacc.c:1646  */
    {
		printf("line:%d\n", line);
	}
#line 1647 "small.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 224 "small.y" /* yacc.c:1646  */
    {
		
		gen(ret, 0, 0);
	}
#line 1656 "small.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 230 "small.y" /* yacc.c:1646  */
    {
		if ((yyvsp[-1].integer) == 1) { //纯声明阶段
			strcpy(id, (yyvsp[-7].ident));
			num = (yyvsp[-4].integer);
			enter(procedure);
		}
		else {
			int i = proc_position((yyvsp[-7].ident));
			if (i == 0) {
				yyerror("undeclared procedure!");
				exit(1);
			}
			num = i;
			procRecord[i].adr = (yyvsp[0].integer)-(yyvsp[-4].integer);
		}
		// setdx($<integer>5+3);
		(yyval.integer) = (yyvsp[-4].integer);
	}
#line 1679 "small.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 250 "small.y" /* yacc.c:1646  */
    {
		(yyval.integer) = (yyvsp[0].integer);
	}
#line 1687 "small.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 253 "small.y" /* yacc.c:1646  */
    {
		(yyval.integer) = 0;
	}
#line 1695 "small.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 258 "small.y" /* yacc.c:1646  */
    {
		(yyval.integer) = 1;
		if (globallocalswitch == 0) {
			strcpy(id, (yyvsp[0].ident));
			enter(variable);
			gen(sto, lev, localRecord[localRecordPtr].adr); /* 存变量 */
		}
	}
#line 1708 "small.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 266 "small.y" /* yacc.c:1646  */
    {
		(yyval.integer) = (yyvsp[0].integer)+1;
		if (globallocalswitch == 0) {
			strcpy(id, (yyvsp[-2].ident));
			enter(variable);
			gen(sto, lev, localRecord[localRecordPtr].adr); /* 存变量 */
		}
	}
#line 1721 "small.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 275 "small.y" /* yacc.c:1646  */
    {
	procRecordPtr++;
}
#line 1729 "small.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 278 "small.y" /* yacc.c:1646  */
    {
	lev--;
}
#line 1737 "small.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 281 "small.y" /* yacc.c:1646  */
    {
	lev++;
	if (lev > levmax) {
		yyerror("level overflow");
	}
}
#line 1748 "small.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 290 "small.y" /* yacc.c:1646  */
    {
		printf("line:%d\n", line);
	}
#line 1756 "small.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 305 "small.y" /* yacc.c:1646  */
    {
		printf("308:$<integer>1:%d\n", (yyvsp[-1].integer));
		code[(yyvsp[-1].integer)].a = codeTablePoint;
		printf("309line:%d\n", line);
	}
#line 1766 "small.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 310 "small.y" /* yacc.c:1646  */
    {
		gen(jmp, 0, 0);
		printf("314:$<integer>1:%d\n", (yyvsp[-1].integer));
		code[(yyvsp[-1].integer)].a = codeTablePoint;
		printf("314line:%d\n", line);
	}
#line 1777 "small.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 315 "small.y" /* yacc.c:1646  */
    {
		code[(yyvsp[-5].integer)].a = codeTablePoint;
		printf("317line:%d\n", line);
	}
#line 1786 "small.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 321 "small.y" /* yacc.c:1646  */
    {
		gen(jpc, 0, 0);
		(yyval.integer) = (yyvsp[0].integer);
		printf("326:$<integer>$:%d\n", (yyvsp[0].integer));
	}
#line 1796 "small.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 325 "small.y" /* yacc.c:1646  */
    {
		(yyval.integer) = (yyvsp[-4].integer);
	}
#line 1804 "small.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 330 "small.y" /* yacc.c:1646  */
    {
		gen(jpc, 0, (yyvsp[-4].integer));
	}
#line 1812 "small.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 335 "small.y" /* yacc.c:1646  */
    {
		if ((yyvsp[-2].integer) == 0) {
			yyerror("undeclared variable");	/* 未声明标识符 */
			exit(1);
		}
		printf("line:%d\n", line);
		int i = (yyvsp[-2].integer);
		printf("330:i:%d\n", i);
		printf("globallocalswitch:%d, lev:%d\n", globallocalswitch, lev);
		if (i%2 == 0) {
			if (globalRecord[i/2].kind != variable) {
				yyerror("is not a variable");	/* 标识符是非变量 */
				exit(1);
			}
			printf("337:globalRecord[i/2].name:%s\n", globalRecord[i/2].name);
			gen(sto, 0, globalRecord[i/2].adr); /* 存变量 */
		}
		else {
			if (localRecord[(i-1)/2].kind != variable) {
				yyerror("is not a variable");	/* 标识符是非变量 */
				exit(1);
			}
			gen(sto, lev, localRecord[(i-1)/2].adr); /* 存变量 */
		}
	}
#line 1842 "small.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 383 "small.y" /* yacc.c:1646  */
    {
		if ((yyvsp[0].integer) == 0) {
			yyerror("undeclared variable");	/* 未声明标识符 */
			exit(1);
		}
		int i = (yyvsp[0].integer);
		if (i%2 == 0) {
			if (globalRecord[i/2].kind != variable) {
				yyerror("is not a variable");	/* 标识符是非变量 */
				exit(1);
			}
			gen(opr, 0, 16); /* 读操作 */
			gen(sto, 0, globalRecord[i/2].adr); /* 存变量 */
		}
		else {
			if (localRecord[(i-1)/2].kind != variable) {
				yyerror("is not a variable");	/* 标识符是非变量 */
				exit(1);
			}
			
			gen(opr, 0, 16); /* 读操作 */
			gen(sto, lev, localRecord[(i-1)/2].adr); /* 存变量 */
		}
	}
#line 1871 "small.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 409 "small.y" /* yacc.c:1646  */
    {
		gen(opr, 0, 14); /* 写操作 */
		gen(opr, 0, 15);
		(yyval.integer) = (yyvsp[0].integer);
	}
#line 1881 "small.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 416 "small.y" /* yacc.c:1646  */
    {
		gen(jpc, 0, 0);
	}
#line 1889 "small.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 418 "small.y" /* yacc.c:1646  */
    {
		gen(jmp, 0, (yyvsp[-10].integer));
		code[(yyvsp[-6].integer)].a = codeTablePoint;
	}
#line 1898 "small.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 424 "small.y" /* yacc.c:1646  */
    {
		printf("417:line:%d\n", line);
		if ((yyvsp[-3].integer) == 0) {
			yyerror("undeclared procedure");	/* 未声明过程 */
			exit(1);
		}
		else if (procRecord[(yyvsp[-3].integer)].size != (yyvsp[-1].integer)) {
			yyerror("wrong arguments");	/* 参数列表错误 */
			exit(1);
		}
		gen(cal, (yyvsp[-1].integer), (yyvsp[-3].integer));
	}
#line 1915 "small.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 438 "small.y" /* yacc.c:1646  */
    {
		(yyval.integer) = (yyvsp[0].integer);
	}
#line 1923 "small.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 441 "small.y" /* yacc.c:1646  */
    {
		(yyval.integer) = 0;
	}
#line 1931 "small.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 446 "small.y" /* yacc.c:1646  */
    {
		(yyval.integer) = 1;
		// if ($<integer>1 == 0) {
		// 	yyerror("undeclared variable");	/* 未声明标识符 */
		// 	exit(1);
		// }
		// int i = $<integer>1;
		// if (i%2 == 0) {
		// 	gen(put, 0, globalRecord[i/2].adr); /* 取变量 */
		// }
		// else {
		// 	gen(put, 1, localRecord[(i-1)/2].adr); /* 取变量 */
		// }
	}
#line 1950 "small.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 460 "small.y" /* yacc.c:1646  */
    {
		(yyval.integer) = (yyvsp[-2].integer)+1;
		// if ($<integer>3 == 0) {
		// 	yyerror("undeclared variable");	/* 未声明标识符 */
		// 	exit(1);
		// }
		// int i = $<integer>3;
		// if (i%2 == 0) {
		// 	if (globalRecord[i/2].kind != variable) {
		// 		yyerror("is not a variable");	/* 标识符是过程 */
		// 		exit(1);
		// 	}
		// 	gen(put, 0, globalRecord[i/2].adr); /* 取变量 */
		// }
		// else {
		// 	if (localRecord[(i-1)/2].kind != variable) {
		// 		yyerror("is not a variable");	/* 标识符是过程 */
		// 		exit(1);
		// 	}
		// 	gen(put, 1, localRecord[(i-1)/2].adr); /* 取变量 */
		// }
	}
#line 1977 "small.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 485 "small.y" /* yacc.c:1646  */
    {
		(yyval.integer) = (yyvsp[0].integer)==0 ? 1:0;
	}
#line 1985 "small.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 488 "small.y" /* yacc.c:1646  */
    {
		gen(opr, 0, 6);
		(yyval.integer) = (yyvsp[0].integer)%2;
	}
#line 1994 "small.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 492 "small.y" /* yacc.c:1646  */
    {
		gen(opr, 0, 8);
		(yyval.integer) = (yyvsp[-2].integer)==(yyvsp[0].integer) ? 1:0;
	}
#line 2003 "small.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 496 "small.y" /* yacc.c:1646  */
    {
		gen(opr, 0, 9);
		(yyval.integer) = (yyvsp[-2].integer)!=(yyvsp[0].integer) ? 1:0;
	}
#line 2012 "small.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 500 "small.y" /* yacc.c:1646  */
    {
		gen(opr, 0, 10);
		(yyval.integer) = (yyvsp[-2].integer)<(yyvsp[0].integer) ? 1:0;
	}
#line 2021 "small.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 504 "small.y" /* yacc.c:1646  */
    {
		gen(opr, 0, 11);
		(yyval.integer) = (yyvsp[-2].integer)>(yyvsp[0].integer) ? 1:0;
	}
#line 2030 "small.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 508 "small.y" /* yacc.c:1646  */
    {
		gen(opr, 0, 12);
		(yyval.integer) = (yyvsp[-2].integer)>=(yyvsp[0].integer) ? 1:0;
	}
#line 2039 "small.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 512 "small.y" /* yacc.c:1646  */
    {
		gen(opr, 0, 13);
		(yyval.integer) = (yyvsp[-2].integer)<=(yyvsp[0].integer) ? 1:0;
	}
#line 2048 "small.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 518 "small.y" /* yacc.c:1646  */
    {
		gen(opr, 0, 1);
		(yyval.integer) = -1*(yyvsp[0].integer);
		fprintf(stdout, "%d\n", (yyval.integer));
	}
#line 2058 "small.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 523 "small.y" /* yacc.c:1646  */
    {
		gen(opr, 0, 2);
		(yyval.integer) = (yyvsp[-2].integer)+(yyvsp[0].integer);
		fprintf(stdout, "%d\n", (yyval.integer));
	}
#line 2068 "small.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 528 "small.y" /* yacc.c:1646  */
    {
		gen(opr, 0, 3);
		(yyval.integer) = (yyvsp[-2].integer)-(yyvsp[0].integer);
		fprintf(stdout, "%d\n", (yyval.integer));
	}
#line 2078 "small.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 533 "small.y" /* yacc.c:1646  */
    {
		gen(opr, 0, 4);
		(yyval.integer) = (yyvsp[-2].integer)*(yyvsp[0].integer);
		fprintf(stdout, "%d\n", (yyval.integer));
	}
#line 2088 "small.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 538 "small.y" /* yacc.c:1646  */
    {
		gen(opr, 0, 5);
		(yyval.integer) = (yyvsp[-2].integer)/(yyvsp[0].integer);
		fprintf(stdout, "%d\n", (yyval.integer));
	}
#line 2098 "small.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 543 "small.y" /* yacc.c:1646  */
    {
		gen(opr, 0, 17);
		(yyval.integer) = (yyvsp[-2].integer)%(yyvsp[0].integer);
		fprintf(stdout, "%d\n", (yyval.integer));
	}
#line 2108 "small.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 548 "small.y" /* yacc.c:1646  */
    {
		gen(opr, 0, 18);
		(yyval.integer) = (yyvsp[-2].integer)^(yyvsp[0].integer);
		fprintf(stdout, "%d\n", (yyval.integer));
	}
#line 2118 "small.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 553 "small.y" /* yacc.c:1646  */
    {
		(yyval.integer) = (yyvsp[-1].integer);
		fprintf(stdout, "%d\n", (yyval.integer));
	}
#line 2127 "small.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 557 "small.y" /* yacc.c:1646  */
    {
		gen(get, 0, 0); /* 获取返回值 */
	}
#line 2135 "small.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 560 "small.y" /* yacc.c:1646  */
    {
		gen(lit, 0, (yyvsp[0].integer));
		(yyval.integer) = (yyvsp[0].integer);
		fprintf(stdout, "%d\n", (yyval.integer));
	}
#line 2145 "small.tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 565 "small.y" /* yacc.c:1646  */
    {
		int i = position((yyvsp[0].ident));	/* 查找标识符在符号表中的位置 */
		if (i==0) {
			yyerror("undefined variable");	/* 标识符未声明，只能使用本层的标识符 */
		}
		if (i%2 == 0) {
			i = i/2;
			printf("560:name:%s\n", globalRecord[i].name);
			switch (globalRecord[i].kind) {
				case constant:	/* 标识符为常量 */
					gen(lit, 0, globalRecord[i].val);	/* 直接把常量的值入栈 */
					break;
				case variable:	/* 标识符为变量 */
					gen(lod, 0, globalRecord[i].adr);	/* 找到变量地址并将其值入栈 */
					break;
			}
		}
		else {
			i = (i-1)/2;
			printf("572:name:%s\n", localRecord[i].name);
			switch (localRecord[i].kind) {
				case constant:	/* 标识符为常量 */
					gen(lit, 0, localRecord[i].val);	/* 直接把常量的值入栈 */
					break;
				case variable:	/* 标识符为变量 */
					gen(lod, lev, localRecord[i].adr);	/* 找到变量地址并将其值入栈 */
					break;
			}
		}
	}
#line 2180 "small.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 597 "small.y" /* yacc.c:1646  */
    {
		(yyval.integer) = position((yyvsp[0].ident));
	}
#line 2188 "small.tab.c" /* yacc.c:1646  */
    break;

  case 81:
#line 602 "small.y" /* yacc.c:1646  */
    {
		(yyval.integer) = proc_position((yyvsp[0].ident));
	}
#line 2196 "small.tab.c" /* yacc.c:1646  */
    break;


#line 2200 "small.tab.c" /* yacc.c:1646  */
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
#line 605 "small.y" /* yacc.c:1906  */

int yyerror(char *s) {
	err++;
	printf("line %d error: %s\n", line, s);
	fprintf(stdout, "line %d error: %s\n", line, s);
	return 0;
}
void enter(enum object k) {
	if (globallocalswitch == 1) {
		switch (k) {
			case constant:	/* 常量 */
				globalRecordPtr++;
				strcpy(globalRecord[globalRecordPtr].name, id); /* 符号表的name域记录标识符的名字 */
				globalRecord[globalRecordPtr].kind = k;	
				globalRecord[globalRecordPtr].val = num; /* 登记常数的值 */
				globalRecord[globalRecordPtr].level = 0;
				break;
			case variable:	/* 变量 */
				globalRecordPtr++;
				strcpy(globalRecord[globalRecordPtr].name, id); /* 符号表的name域记录标识符的名字 */
				globalRecord[globalRecordPtr].kind = k;	
				globalRecord[globalRecordPtr].val = 0; /* 变量默认初始值为0 */
				globalRecord[globalRecordPtr].level = 0;
				globalRecord[globalRecordPtr].adr = globalRecordPtr;
				break;
			case procedure:	/* 过程 */
				strcpy(procRecord[procRecordPtr].name, id); /* 符号表的name域记录标识符的名字 */
				procRecord[procRecordPtr].kind = k;	
				procRecord[procRecordPtr].level = lev;
				procRecord[procRecordPtr].adr = procRecordPtr;
				procRecord[procRecordPtr].size = num;
				break;
		}
	}
	else {
		switch (k) {
			case constant:	/* 常量 */
				localRecordPtr++;
				strcpy(localRecord[localRecordPtr].name, id); /* 符号表的name域记录标识符的名字 */
				localRecord[localRecordPtr].kind = k;	
				localRecord[localRecordPtr].val = num; /* 登记常数的值 */
				localRecord[localRecordPtr].level = lev;
				break;
			case variable:	/* 变量 */
				localRecordPtr++;
				strcpy(localRecord[localRecordPtr].name, id); /* 符号表的name域记录标识符的名字 */
				localRecord[localRecordPtr].kind = k;	
				localRecord[localRecordPtr].val = 0; /* 变量默认初始值为0 */
				localRecord[localRecordPtr].level = lev;
				localRecord[localRecordPtr].adr = localRecordPtr;
				break;
			case procedure:	/* 过程 */
				procRecord[num].adr = codeTablePoint;
				break;
		}
	}
	
}
void init() {
	symbolTablePoint = 0;
	codeTablePoint = 0;
	procTablePoint = 0;
	procRecordPtr = 0;
	localRecordPtr = 0;
	globalRecordPtr = 0;
	globallocalswitch = 1;
	lev = 0;
	proctable[0] = 0;
	num = 0;
	procDeclFinished = 0;
	err = 0;
}
int position(char* id) {
	int i = localRecordPtr;
	strcpy(localRecord[0].name, id);
	localRecord[0].level = lev;
	while (strcmp(localRecord[i].name, id)!=0 || localRecord[i].level!=lev) {
        i--;
    }
	if (i == 0) {
		strcpy(globalRecord[0].name, id);
		i = globalRecordPtr;
		while (strcmp(globalRecord[i].name, id) != 0) {
        	i--;
    	}
		return i*2;
	}
	printf("id:%s, lev:%d, i:%d\n", id, lev, i);
	return i*2+1;
}
int proc_position(char* id) {
	int i = procRecordPtr;
	strcpy(procRecord[0].name, id);
	while (strcmp(procRecord[i].name, id) != 0) {
        i--;
    }
	return i;
}
void setdx(int n) {
	int i;
	printf("setdx(%d)\n", n);
	for (i=1; i<=n; i++) {
		localRecord[localRecordPtr-i+1].adr = n-i+3;
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
		{"lit"}, {"opr"}, {"lod"}, {"sto"}, {"cal"}, {"int"}, {"jmp"}, {"jpc"}, {"ret"}, {"res"}, {"put"}, {"get"},
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
		printf("==================Procedure Table================\n");
		fprintf(ftable, "==================Procedure Table================\n");
		for (i = 1; i <= procRecordPtr; i++) {
			printf("    %d proc  %s ", i, procRecord[i].name);
			printf("lev=%d addr=%d size=%d\n", procRecord[i].level, procRecord[i].adr, procRecord[i].size);
			fprintf(ftable,"    %d proc  %s ", i, procRecord[i].name);
			fprintf(ftable,"lev=%d addr=%d size=%d\n", procRecord[i].level, procRecord[i].adr, procRecord[i].size);
		}
		printf("==================Global Table================\n");
		fprintf(ftable, "==================Global Table================\n");
		for (i = 1; i <= globalRecordPtr; i++) {
			switch(globalRecord[i].kind) {
				case constant:
					printf("    %d const %s ", i, globalRecord[i].name);
					printf("val=%d\n", globalRecord[i].val);
					fprintf(ftable, "    %d const %s ", i, globalRecord[i].name);
					fprintf(ftable, "val=%d\n", globalRecord[i].val);
					break;
				case variable:
					printf("    %d var   %s ", i, globalRecord[i].name);
					printf("lev=%d addr=%d\n", globalRecord[i].level, globalRecord[i].adr);
					fprintf(ftable, "    %d var   %s ", i, globalRecord[i].name);
					fprintf(ftable, "lev=%d addr=%d\n", globalRecord[i].level, globalRecord[i].adr);
					break;
			}
		}
		printf("==================Local Table================\n");
		fprintf(ftable, "==================Local Table================\n");
		for (i = 1; i <= localRecordPtr; i++) {
			switch(localRecord[i].kind) {
				case constant:
					printf("    %d const %s ", i, localRecord[i].name);
					printf("val=%d\n", localRecord[i].val);
					fprintf(ftable, "    %d const %s ", i, localRecord[i].name);
					fprintf(ftable, "val=%d\n", localRecord[i].val);
					break;
				case variable:
					printf("    %d var   %s ", i, localRecord[i].name);
					printf("lev=%d addr=%d\n", localRecord[i].level, localRecord[i].adr);
					fprintf(ftable, "    %d var   %s ", i, localRecord[i].name);
					fprintf(ftable, "lev=%d addr=%d\n", localRecord[i].level, localRecord[i].adr);
					break;
			}
		}
		printf("\n");
		fprintf(ftable,"\n");
	}
}
void outputStack(int cnt, int *s, int t) {
	printf("%d:=================================\n", cnt);
	fprintf(ftable, "%d:=================================\n", cnt);
	int i;
	for(i = 0; i <= t; i++) {
		printf("%d: %d\n", i, s[i]);
		fprintf(ftable, "%d: %d\n", i, s[i]);
	}
	printf("==================================\n");
	fprintf(ftable, "=================================\n");
}
void interpret()
{
	int p = 0; /* 指令指针 */
	int b = 1; /* 指令基址 */
	int t = 0; /* 栈顶指针 */
	int argtmpPtr = 0;
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
		int myPtr = p-1;
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
					case 17:/* 取模运算 */
						t = t - 1;
						s[t] = s[t] % s[t + 1];
						break;
					case 18:/* 异或运算 */
						t = t - 1;
						s[t] = s[t] ^ s[t + 1];
						break;

				}
				break;
			case lod:	/* 取相对当前过程的数据基地址为a的内存的值到栈顶 */
				t = t + 1;
				if (i.l == 0) s[t] = globalRecord[i.a].val;
				else s[t] = localRecord[i.a].val;
				break;
			case sto:	/* 栈顶的值存到相对当前过程的数据基地址为a的内存 */
				if (i.l == 0) globalRecord[i.a].val = s[t];
				else localRecord[i.a].val = s[t];
				t = t - 1;
				break;
			case cal:	/* 调用子过程 */
				printf("b:%d, t:%d, i.l:%d\n", b, t, i.l);
				if (i.l != argtmpPtr) yyerror("wrong arguments number");
				s[t + 1] = 0;	/* 将父过程基地址入栈，即建立静态链 */
				s[t + 2] = b;	/* 将本过程基地址入栈，即建立动态链 */
				s[t + 3] = p;	/* 将当前指令指针入栈，即保存返回地址 */
				b = t+1-procRecord[i.a].size;	/* 改变基地址指针值为新过程的基地址 */
				argPtr = procRecord[i.a].size;
				p = procRecord[i.a].adr;	/* 跳转 */
				break;
			case ret:
				s[s[b+1]] = s[t];
				t = t-1;
				break;
			case res:
				if (i.l == 0) globalRecord[i.a].val = s[b];
				else localRecord[i.a].val = s[b];
				break;
			case put:
				argtmpPtr++;
				if (i.l == 0) s[t+3+argtmpPtr] = globalRecord[i.a].val;
				else s[t+3+argtmpPtr] = localRecord[i.a].val;
				break;
			case get:
			    t = t+1;
				s[t] = s[b];
				break;
			case ini:	/* 在数据栈中为被调用的过程开辟a个单元的数据区 */
				s[t+1] = s[t+1+argPtr];
				s[t+2] = s[t+2+argPtr];
				s[t+3] = s[t+3+argPtr];
				t = t+3;	
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
		outputStack(myPtr, s, t);
	} while (p != 0);
	printf("End small\n");
	fprintf(fresult,"End small\n");
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

