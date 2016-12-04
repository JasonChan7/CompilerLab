%{
	#include<stdio.h>
	#include<stdlib.h>
	#include<string.h>

	#define bool int
	#define true 1
	#define false 0

	#define norw 13       /* 保留字个数 */
	#define txmax 100     /* 符号表容量 */
	#define nmax 14       /* 数字的最大位数 */
	#define al 10         /* 标识符的最大长度 */
	#define maxerr 30     /* 允许的最多错误数 */
	#define amax 2048     /* 地址上界*/
	#define levmax 3      /* 最大允许过程嵌套声明层数*/
	#define cxmax 200     /* 最多的虚拟机代码数 */
	#define stacksize 500 /* 运行时数据栈元素最多为500个 */
	/* 符号 */
	enum symbol {
		nul,         ident,     number,     plus,      minus, 
		times,       slash,     oddsym,     eql,       neq, 
		lss,         leq,       gtr,        geq,       lparen, 
		rparen,      comma,     semicolon,  period,    becomes, 
		beginsym,    endsym,    ifsym,      thensym,   whilesym, 
		writesym,    readsym,   dosym,      callsym,   constsym, 
		varsym,      procsym,   
	};
	#define symnum 32

	/* 符号表中的类型 */
	enum object {
		constant, 
		variable, 
		procedure,
	};

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

	bool listswitch ;   /* 显示虚拟机代码与否 */
	bool tableswitch ;  /* 显示符号表与否 */
	char ch;            /* 存放当前读取的字符，getch 使用 */
	enum symbol sym;    /* 当前的符号 */
	char id[al+1];      /* 当前ident，多出的一个字节用于存放0 */
	int num;            /* 当前number */
	int cc, ll;         /* getch使用的计数器，cc表示当前字符(ch)的位置 */
	int cx;             /* 虚拟机代码指针, 取值范围[0, cxmax-1]*/
	char line[81];      /* 读取行缓冲区 */
	char a[al+1];       /* 临时符号，多出的一个字节用于存放0 */
	struct instruction code[cxmax]; /* 存放虚拟机代码的数组 */
	char word[norw][al];        /* 保留字 */
	enum symbol wsym[norw];     /* 保留字对应的符号值 */
	enum symbol ssym[256];      /* 单字符的符号值 */
	char mnemonic[fctnum][5];   /* 虚拟机代码指令名称 */
	bool declbegsys[symnum];    /* 表示声明开始的符号集合 */
	bool statbegsys[symnum];    /* 表示语句开始的符号集合 */
	bool facbegsys[symnum];     /* 表示因子开始的符号集合 */

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

	struct tablestruct table[txmax]; /* 符号表 */

	FILE* fin;      /* 输入源文件 */
	FILE* ftable;	/* 输出符号表 */
	FILE* fcode;    /* 输出虚拟机代码 */
	FILE* foutput;  /* 输出文件及出错示意（如有错）、各行对应的生成代码首地址（如无错） */
	FILE* fresult;  /* 输出执行结果 */
	char fname[al];
	int err;        /* 错误计数器 */
	int yylex(void);
	void yyerror(char *);
	
%}
%union {
	int integer; /* integer value */ 
	char ident[15]; /* identifier */ 
}
%token <ident> IDENT
%token <integer> INTEGER
%token IF THEN ELSE END REPEAT UNTIL READ WRITE
%nonassoc IFX
%nonassoc ELSE
%token BC GT LT GE LE EQ NE
%left '+' '-'
%left '*' '/'
%nonassoc UMINUS
%%
program:
	stmt_sequence
	;
stmt_sequence:
	stmt_sequence ';' statement
	| statement
	;
statement:
	if_stmt
	| repeat_stmt
	| assign_stmt
	| read_stmt
	| write_stmt
	;
if_stmt:
	IF expr THEN stmt_sequence END %prec IFX
	| IF expr THEN stmt_sequence ELSE stmt_sequence END
	;
repeat_stmt:
	REPEAT stmt_sequence UNTIL expr
	;
assign_stmt:
	identifier BC expr
	;
read_stmt:
	READ identifier
	;
write_stmt:
	WRITE expr
	;
expr:
	simple_expr
	| simple_expr EQ simple_expr {
		gen(opr, 0, 8)
	}
	| simple_expr NE simple_expr {
		gen(opr, 0, 9)
	}
	| simple_expr LT simple_expr {
		gen(opr, 0, 10)
	}
	| simple_expr GE simple_expr {
		gen(opr, 0, 11)
	}
	| simple_expr GT simple_expr {
		gen(opr, 0, 12)
	}
	| simple_expr LE simple_expr {
		gen(opr, 0, 13)
	}
	;
simple_expr:
	simple_expr '+' simple_expr {
		gen(opr, 0, 2)
	}
	| simple_expr '-' simple_expr {
		gen(opr, 0, 3)
	}
	| simple_expr '*' simple_expr {
		gen(opr, 0, 4)
	}
	| simple_expr '/' simple_expr {
		gen(opr, 0, 5)
	}
	| '(' simple_expr ')'
	| term
	;
term:
	INTEGER {
		gen(lit, 0, $1)
	}
	| IDENT {
		
	}

/* 主程序开始 */
int main()
{
    yyparse();
	return 0;
}

