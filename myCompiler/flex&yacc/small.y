%{
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
%}
%union {
	int integer; /* integer value */ 
	char *ident; /* identifier */ 
}
%token <ident> IDENT
%token <integer> INTEGER
%token IF THEN ELSE END REPEAT UNTIL READ WRITE CALL CONST VAR MYBEGIN XOR ODD PROC WHILE DO RET FOR
%nonassoc IFX
%nonassoc ELSE
%token BC GT LT GE LE EQ NE
%token INC DEC
%left '+' '-'
%left '*' '/' '%'
%left XOR
%right ODD
%nonassoc UMINUS
%type <integer> get_code_addr get_table_addr /* 记录本层标识符的初始位置 */
%%
program: 
	get_code_addr {
		gen(jmp, 0, 0);
	} open_globallocalswitch
	my_proc_predecls
  	const_decl
  	var_decl
	close_globallocalswitch
  	proc_decls increase_procRecord {
		code[$<integer>1].a = codeTablePoint;
		procRecord[procRecordPtr].adr = codeTablePoint;
		strcpy(procRecord[procRecordPtr].name, "___main__");
		displaytable();
		procDeclFinished = 1;
		printf("fk: %d\n", $<integer>7);
		gen(ini, 0, 3);
	} stmt_sequence {
		gen(opr, 0, 0);
	}
	;
my_proc_predecls: 
	MYBEGIN proc_predecls END
	|
	;
get_code_addr: {
	$<integer>$ = codeTablePoint; 
};
get_table_addr: {
	$<integer>$ = localRecordPtr;
};
open_globallocalswitch: {
	globallocalswitch = 1;
};
close_globallocalswitch: {
	globallocalswitch = 0;
};
get_globallocalswitch: {
	$<integer>$ = globallocalswitch;
}
proc_predecls:
	proc_predecls increase_procRecord proc_decl decrease_level
	|
	;
const_decl: 
	CONST const_list ';' {
		$<integer>$ = $<integer>2;
	}
	| {
		$<integer>$ = 0;
	}
	;
const_list:
	const_def {
		$<integer>$ = 1;
	}
	| const_list ',' const_def {
		$<integer>$ = $<integer>1+1;
	}
	;
const_def:
	IDENT BC INTEGER {
		strcpy(id, $<ident>1);
		num = $<integer>3;
		enter(constant);
		$<integer>$ = 1;
	}
	;
var_decl: 
	VAR var_list ';' {
		$<integer>$ = $<integer>2;
	}
	| {
		$<integer>$ = 0;
	}
	;
var_list:
	var_def {
		$<integer>$ = $<integer>1;
	}
	| var_list ',' var_def {
		$<integer>$ = $<integer>1+$<integer>3;
	}
	;
var_def:
	IDENT {
		strcpy(id, $1);
		num = 0;
		enter(variable);
		$<integer>$ = 1;
	}
	;
proc_decls: 
	proc_decls proc_decl MYBEGIN const_decl var_decl {
		gen(ini, 0, 3);
	} stmt_sequence ';' return_stmt END decrease_level {
		$<integer>$ = $<integer>1+1;
		gen(opr, 0, 0);
	}
	| {
		$<integer>$ = 0;
	}
	;
return_stmt:
	{
		printf("line:%d\n", line);
	} RET expr ';' {
		
		gen(ret, 0, 0);
	}
	;
proc_decl: 
	PROC IDENT increase_level '(' para_list ')' ';' get_globallocalswitch get_code_addr {
		if ($<integer>8 == 1) { //纯声明阶段
			strcpy(id, $2);
			num = $<integer>5;
			enter(procedure);
		}
		else {
			int i = proc_position($2);
			if (i == 0) {
				yyerror("undeclared procedure!");
				exit(1);
			}
			num = i;
			procRecord[i].adr = $<integer>9-$<integer>5;
		}
		// setdx($<integer>5+3);
		$<integer>$ = $<integer>5;
	}
	;
para_list:
	para_stmt {
		$<integer>$ = $<integer>1;
	}
	| {
		$<integer>$ = 0;
	}
	;
para_stmt:
	IDENT {
		$<integer>$ = 1;
		if (globallocalswitch == 0) {
			strcpy(id, $1);
			enter(variable);
			gen(sto, lev, localRecord[localRecordPtr].adr); /* 存变量 */
		}
	}
	| IDENT ',' para_stmt {
		$<integer>$ = $<integer>3+1;
		if (globallocalswitch == 0) {
			strcpy(id, $1);
			enter(variable);
			gen(sto, lev, localRecord[localRecordPtr].adr); /* 存变量 */
		}
	}
	;
increase_procRecord: {
	procRecordPtr++;
};
decrease_level: {
	lev--;
};
increase_level: {
	lev++;
	if (lev > levmax) {
		yyerror("level overflow");
	}
};

stmt_sequence:
	stmt_sequence ';' statement
	| statement {
		printf("line:%d\n", line);
	}
	;
statement:
	if_stmt 
	| repeat_stmt
	| assign_stmt
	| read_stmt
	| write_stmt
	| while_stmt
	| dowhile_stmt
	| call_stmt
	| return_stmt
	| for_stmt
	;
if_stmt:
	if_stmt_no_else END {
		printf("308:$<integer>1:%d\n", $<integer>1);
		code[$<integer>1].a = codeTablePoint;
		printf("309line:%d\n", line);
	}
	| if_stmt_no_else get_code_addr {
		gen(jmp, 0, 0);
		printf("314:$<integer>1:%d\n", $<integer>1);
		code[$<integer>1].a = codeTablePoint;
		printf("314line:%d\n", line);
	} ELSE stmt_sequence ';' END {
		code[$<integer>2].a = codeTablePoint;
		printf("317line:%d\n", line);
	}
	;
if_stmt_no_else:
	IF expr get_code_addr {
		gen(jpc, 0, 0);
		$<integer>$ = $<integer>3;
		printf("326:$<integer>$:%d\n", $<integer>3);
	} THEN stmt_sequence ';' {
		$<integer>$ = $<integer>3;
	}
	;
repeat_stmt:
	REPEAT get_code_addr stmt_sequence UNTIL expr {
		gen(jpc, 0, $<integer>2);
	}
	;
assign_stmt:
	identifier BC expr {
		if ($<integer>1 == 0) {
			yyerror("undeclared variable");	/* 未声明标识符 */
			exit(1);
		}
		printf("line:%d\n", line);
		int i = $<integer>1;
		printf("330:i:%d\n", i);
		printf("globallocalswitch:%d, lev:%d\n", globallocalswitch, lev);
		if (i%2 == 0) {
			if (globalRecord[i/2].kind != variable) {
				yyerror("is not a variable");	/* 标识符是非变量 */
				exit(1);
			}
			printf("337:globalRecord[i/2].name:%s\n", globalRecord[i/2].name);
			gen(sto, 0, globalRecord[i/2].adr); /* 存变量 */
			$<integer>$ = $<integer>1;
		}
		else {
			if (localRecord[(i-1)/2].kind != variable) {
				yyerror("is not a variable");	/* 标识符是非变量 */
				exit(1);
			}
			gen(sto, lev, localRecord[(i-1)/2].adr); /* 存变量 */
			$<integer>$ = $<integer>1;
		}
	}
	| INC identifier {
		if ($<integer>2 == 0) {
			yyerror("undeclared variable");	/* 未声明标识符 */
			exit(1);
		}
		printf("371: reco INC!\n");
		int i = $<integer>2;
		if (i%2 == 0) {
			if (globalRecord[i/2].kind != variable) {
				yyerror("is not a variable");	/* 标识符是非变量 */
				exit(1);
			}
			gen(lod, 0, globalRecord[i/2].adr);
			gen(lit, 0, 1);
			gen(opr, 0, 2);
			gen(sto, 0, globalRecord[i/2].adr); /* 存变量 */
			gen(lod, 0, globalRecord[i/2].adr);
		}
		else {
			if (localRecord[(i-1)/2].kind != variable) {
				yyerror("is not a variable");	/* 标识符是非变量 */
				exit(1);
			}
			gen(lod, 0, localRecord[(i-1)/2].adr);
			gen(lit, 0, 1);
			gen(opr, 0, 2);
			gen(sto, lev, localRecord[(i-1)/2].adr); /* 存变量 */
			gen(lod, 0, localRecord[(i-1)/2].adr);
		}
		$<integer>$ = $<integer>2;
	}
	| DEC identifier {
		if ($<integer>2 == 0) {
			yyerror("undeclared variable");	/* 未声明标识符 */
			exit(1);
		}
		printf("399: reco DEC!\n");
		int i = $<integer>2;
		if (i%2 == 0) {
			if (globalRecord[i/2].kind != variable) {
				yyerror("is not a variable");	/* 标识符是非变量 */
				exit(1);
			}
			gen(lod, 0, globalRecord[i/2].adr);
			gen(lit, 0, 1);
			gen(opr, 0, 3);
			gen(sto, 0, globalRecord[i/2].adr); /* 存变量 */
			gen(lod, 0, globalRecord[i/2].adr);
		}
		else {
			if (localRecord[(i-1)/2].kind != variable) {
				yyerror("is not a variable");	/* 标识符是非变量 */
				exit(1);
			}
			gen(lod, 0, localRecord[(i-1)/2].adr);
			gen(lit, 0, 1);
			gen(opr, 0, 3);
			gen(sto, lev, localRecord[(i-1)/2].adr); /* 存变量 */
			gen(lod, 0, localRecord[(i-1)/2].adr);
		}
		$<integer>$ = $<integer>1;
	}
	| identifier INC {
		if ($<integer>1 == 0) {
			yyerror("undeclared variable");	/* 未声明标识符 */
			exit(1);
		}
		printf("371: reco INC!\n");
		int i = $<integer>1;
		printf("437:%d\n", i);
		if (i%2 == 0) {
			if (globalRecord[i/2].kind != variable) {
				yyerror("is not a variable");	/* 标识符是非变量 */
				exit(1);
			}
			gen(lod, 0, globalRecord[i/2].adr);
			gen(lod, 0, globalRecord[i/2].adr);
			gen(lit, 0, 1);
			gen(opr, 0, 2);
			gen(sto, 0, globalRecord[i/2].adr); /* 存变量 */
		}
		else {
			if (localRecord[(i-1)/2].kind != variable) {
				yyerror("is not a variable");	/* 标识符是非变量 */
				exit(1);
			}
			gen(lod, 0, localRecord[(i-1)/2].adr);
			gen(lod, 0, localRecord[(i-1)/2].adr);
			gen(lit, 0, 1);
			gen(opr, 0, 2);
			gen(sto, lev, localRecord[(i-1)/2].adr); /* 存变量 */
		}
		$<integer>$ = $<integer>2;
	}
	| identifier DEC {
		if ($<integer>1 == 0) {
			yyerror("undeclared variable");	/* 未声明标识符 */
			exit(1);
		}
		printf("399: reco DEC!\n");
		int i = $<integer>1;
		if (i%2 == 0) {
			if (globalRecord[i/2].kind != variable) {
				yyerror("is not a variable");	/* 标识符是非变量 */
				exit(1);
			}
			gen(lod, 0, globalRecord[i/2].adr);
			gen(lod, 0, globalRecord[i/2].adr);
			gen(lit, 0, 1);
			gen(opr, 0, 3);
			gen(sto, 0, globalRecord[i/2].adr); /* 存变量 */
		}
		else {
			if (localRecord[(i-1)/2].kind != variable) {
				yyerror("is not a variable");	/* 标识符是非变量 */
				exit(1);
			}
			gen(lod, 0, localRecord[(i-1)/2].adr);
			gen(lod, 0, localRecord[(i-1)/2].adr);
			gen(lit, 0, 1);
			gen(opr, 0, 3);
			gen(sto, lev, localRecord[(i-1)/2].adr); /* 存变量 */
		}
		$<integer>$ = $<integer>1;
	}
	// | identifier BC call_stmt {
	// 	if ($<integer>1 == 0) {
	// 		yyerror("undeclared variable");	/* 未声明标识符 */
	// 		exit(1);
	// 	}
	// 	int i = $<integer>1;
	// 	if (i%2 == 0) {
	// 		if (globalRecord[i/2].kind != variable) {
	// 			yyerror("is not a variable");	/* 标识符是非变量 */
	// 			exit(1);
	// 		}
	// 		gen(res, 0, globalRecord[i/2].adr); /* 存变量 */
	// 	}
	// 	else {
	// 		if (localRecord[(i-1)/2].kind != variable) {
	// 			yyerror("is not a variable");	/* 标识符是非变量 */
	// 			exit(1);
	// 		}
	// 		gen(res, lev, localRecord[(i-1)/2].adr); /* 存变量 */
	// 	}
	// }
	;
read_stmt:
	READ identifier {
		if ($<integer>2 == 0) {
			yyerror("undeclared variable");	/* 未声明标识符 */
			exit(1);
		}
		int i = $<integer>2;
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
	;
write_stmt:
	WRITE expr {
		printf("line:%d\n", line);
		gen(opr, 0, 14); /* 写操作 */
		gen(opr, 0, 15);
		$<integer>$ = $<integer>2;
	}
	;
while_stmt:
	WHILE get_code_addr expr get_code_addr {
		gen(jpc, 0, 0);
	} DO MYBEGIN stmt_sequence END {
		gen(jmp, 0, $<integer>2);
		code[$<integer>4].a = codeTablePoint;
	}
	;
dowhile_stmt:
	DO get_code_addr stmt_sequence WHILE expr {
    	gen(jpc, 0, codeTablePoint+2);
    	gen(jmp, 0, $<integer>2);
    }
    ;
for_stmt:
	FOR '(' assign_stmt ';' get_code_addr expr get_code_addr {
		gen(jpc, 0, 0);
		gen(jmp, 0, 0);
	}';' assign_stmt {
		gen(jmp, 0, 0);
	}')' get_code_addr MYBEGIN stmt_sequence ';' END get_code_addr {
		printf("441:$<integer>5:%d\n",$<integer>5);
		printf("442:$<integer>7:%d\n",$<integer>7);
		printf("443:$<integer>13:%d\n",$<integer>13);
		printf("444:$<integer>17:%d\n",$<integer>18);
		gen(jmp, 0, $<integer>7+2);
		code[$<integer>7].a = $<integer>18+1;
		code[$<integer>7+1].a = $<integer>13;
		code[$<integer>13-1].a = $<integer>5;
	}
call_stmt:
	CALL get_table_addr proc_identifier '(' arg_list ')' {
		printf("417:line:%d\n", line);
		if ($<integer>3 == 0) {
			yyerror("undeclared procedure");	/* 未声明过程 */
			exit(1);
		}
		else if (procRecord[$<integer>3].size != $<integer>5) {
			yyerror("wrong arguments");	/* 参数列表错误 */
			exit(1);
		}
		gen(cal, $<integer>5, $<integer>3);
	}
	;
arg_list:
	arg_stmt {
		$<integer>$ = $<integer>1;
	}
	| {
		$<integer>$ = 0;
	}
	;
arg_stmt:
	expr {
		$<integer>$ = 1;
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
	| arg_stmt ',' expr {
		$<integer>$ = $<integer>1+1;
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
	;

expr:
	simple_expr {
		$<integer>$ = $<integer>1==0 ? 1:0;
	}
	| ODD simple_expr {
		gen(opr, 0, 6);
		$<integer>$ = $<integer>2%2;
	}
	| simple_expr EQ simple_expr {
		gen(opr, 0, 8);
		$<integer>$ = $<integer>1==$<integer>3 ? 1:0;
	}
	| simple_expr NE simple_expr {
		gen(opr, 0, 9);
		$<integer>$ = $<integer>1!=$<integer>3 ? 1:0;
	}
	| simple_expr LT simple_expr {
		gen(opr, 0, 10);
		$<integer>$ = $<integer>1<$<integer>3 ? 1:0;
	}
	| simple_expr GE simple_expr {
		gen(opr, 0, 11);
		$<integer>$ = $<integer>1>$<integer>3 ? 1:0;
	}
	| simple_expr GT simple_expr {
		gen(opr, 0, 12);
		$<integer>$ = $<integer>1>=$<integer>3 ? 1:0;
	}
	| simple_expr LE simple_expr {
		gen(opr, 0, 13);
		$<integer>$ = $<integer>1<=$<integer>3 ? 1:0;
	}
	;
simple_expr:
	'-' simple_expr %prec UMINUS {
		gen(opr, 0, 1);
		$<integer>$ = -1*$<integer>2;
		fprintf(stdout, "%d\n", $<integer>$);
	}
	| simple_expr '+' simple_expr {
		gen(opr, 0, 2);
		$<integer>$ = $<integer>1+$<integer>3;
		fprintf(stdout, "%d\n", $<integer>$);
	}
	| simple_expr '-' simple_expr {
		gen(opr, 0, 3);
		$<integer>$ = $<integer>1-$<integer>3;
		fprintf(stdout, "%d\n", $<integer>$);
	}
	| simple_expr '*' simple_expr {
		gen(opr, 0, 4);
		$<integer>$ = $<integer>1*$<integer>3;
		fprintf(stdout, "%d\n", $<integer>$);
	}
	| simple_expr '/' simple_expr {
		gen(opr, 0, 5);
		$<integer>$ = $<integer>1/$<integer>3;
		fprintf(stdout, "%d\n", $<integer>$);
	} 
	| simple_expr '%' simple_expr {
		gen(opr, 0, 17);
		$<integer>$ = $<integer>1%$<integer>3;
		fprintf(stdout, "%d\n", $<integer>$);
	} 
	| simple_expr XOR simple_expr {
		gen(opr, 0, 18);
		$<integer>$ = $<integer>1^$<integer>3;
		fprintf(stdout, "%d\n", $<integer>$);
	} 
	| '(' expr ')' {
		$<integer>$ = $<integer>2;
		fprintf(stdout, "%d\n", $<integer>$);
	}
	| call_stmt {
		gen(get, 0, 0); /* 获取返回值 */
	}
	| '(' assign_stmt ')' 
	| INTEGER {
		gen(lit, 0, $<integer>1);
		$<integer>$ = $<integer>1;
		fprintf(stdout, "%d\n", $<integer>$);
	}
	| IDENT {
		int i = position($<ident>1);	/* 查找标识符在符号表中的位置 */
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
	;
identifier:
	IDENT {
		$<integer>$ = position($<ident>1);
	}
	;
proc_identifier:
	IDENT {
		$<integer>$ = proc_position($<ident>1);
	}
%%
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
				// if (i.l != argtmpPtr) yyerror("wrong arguments number");
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
			    t = t+1;
				s[t]=i.a;
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
		// outputStack(myPtr, s, t);
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

