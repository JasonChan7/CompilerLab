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
%}
%union {
	bool tf; /* boolean value */
	int integer; /* integer value */ 
	char *ident; /* identifier */ 
}
%token <ident> IDENT
%token <integer> INTEGER
%token IF THEN ELSE END REPEAT UNTIL READ WRITE CALL CONST VAR BEGIN XOR ODD PROC WHILE DO
%nonassoc IFX
%nonassoc ELSE
%token BC GT LT GE LE EQ NE
%left '+' '-'
%left '*' '/' '%'
%nonassoc UMINUS
%type <integer> get_table_addr get_code_addr /* 记录本层标识符的初始位置 */
%%
program:
	block
	;

block: {
	table[symbolTablePoint].adr = codeTablePoint;
	$<integer>$ = codeTablePoint;
	gen(jmp, 0, 0);
} get_table_addr
  const_decl
  var_decl {
	  setdx($<integer>2+$<integer>3);
  }
  proc_decls {
	code[$<integer>1].a = codeTablePoint;

	// table[$2].adr = codeTablePoint;
	// table[$2].size = $4+3;
	// gen(ini, 0, $4+3);
	displaytable();
} stmt_sequence;

get_table_addr: {
	$<integer>$ = symbolTablePoint;
};
get_code_addr: {
	$<integer>$ = codeTablePoint; 
};

/* 常量相关定义 */
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
		$<integer>$ = $<integer>1;
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
	
/* 变量相关定义 */
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
		enter(variable);
		$<integer>$ = 1;
	}
	;

/* 函数相关定义 */
proc_decls: 
	proc_decls proc_decl BEGIN proc_body END {
		$<integer>$ = $<integer>$1+1;
	}
	| {
		$<integer>$ = 0;
	}
	;
proc_decl: 
	increase_procTablePoint PROC IDENT increase_level '(' para_list ')' ';' {
		strcpy(id, $3);
		enter(procedure);
		proctable[procTablePoint] = symbolTablePoint;
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
		strcpy(id, $1);
		enter(parameter);
		gen(sto, lev-table[symbolTablePoint].level, table[symbolTablePoint].adr); /* 存变量 */
	}
	| para_stmt ',' IDENT {
		$<integer>$ = $<integer>1+1;
		strcpy(id, $3);
		enter(parameter);
		gen(sto, lev-table[symbolTablePoint].level, table[symbolTablePoint].adr); /* 存变量 */
	}
	;
proc_body:
	block decrease_level ';'
	;
increase_procTablePoint: {
	procTablePoint++;
};
decrease_level: {
	lev--;
};
decrease_procTablePoint: {
	procTablePoint--;
};
increase_level: {
	lev++;
	if (lev > levmax) {
		yyerror("level overflow");
	}
};

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
	| while_stmt
	| call_stmt
	;
if_stmt:
	IF expr get_code_addr {
		gen(jpc, 0, 0);
	} THEN stmt_sequence END %prec IFX {
		code[$3].a = codeTablePoint;
	}
	| IF expr get_code_addr {
		gen(jpc, 0, 0);
	} THEN stmt_sequence get_code_addr {
		gen(jmp, 0, 0);
	} ELSE {
		code[$<integer>3].a = codeTablePoint;
	} stmt_sequence END {
		code[$<integer>6].a = codeTablePoint;
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
		else if (table[$<integer>1].kind != variable) {
			yyerror("%s is not a variable", table[$<ident>1].name);	/* 标识符非变量 */
			exit(1);
		}
		gen(sto, lev-table[$<integer>1].level, table[$<integer>1].adr);
	}
	;
read_stmt:
	READ identifier {
		if ($<integer>2 == 0) {
			yyerror("undeclared variable");	/* 未声明标识符 */
			exit(1);
		}
		else if (table[$<integer>2].kind != variable) {
			yyerror("%s is not a variable", table[$<ident>2].name);	/* 标识符非变量 */
			exit(1);
		}
		gen(opr, 0, 16); /* 读操作 */
		gen(sto, lev-table[$<integer>2].level, table[$<integer>2].adr); /* 存变量 */
	}
	;
write_stmt:
	WRITE expr {
		gen(opr, 0, 14); /* 写操作 */
		gen(opr, 0, 15);
		$<integer>$ = $<integer>2;
	}
	;
while_stmt:
	WHILE get_code_addr '(' expr ')' get_code_addr {
		gen(jpc, 0, 0);
	} DO BEGIN stmt_sequence END {
		gen(jmp, 0, $<integer>2)
		code[$<integer>6].a = codeTablePoint;
	}
	;
call_stmt:
	CALL proc_identifier '(' arg_list ')' {
		if ($<integer>2 == 0) {
			yyerror("undeclared procedure");	/* 未声明标识符 */
			exit(1);
		}
		else if (table[$<integer>2].size != $<integer>4) {
			yyerror("wrong arguments");	/* 参数列表错误 */
			exit(1);
		}
		gen(cal, lev-table[$<integer>2].level, table[$<integer>2].adr);
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
	identifier {
		$<integer>$ = 1;
		if ($<integer>1 == 0) {
			yyerror("undeclared variable");	/* 未声明标识符 */
			exit(1);
		}
		else if (table[$<integer>1].kind != variable) {
			yyerror("%s is not a variable", table[$<ident>1].name);	/* 标识符是过程 */
			exit(1);
		}
		gen(lod, lev-table[$<integer>1].level, table[$<integer>1].adr); /* 取变量 */
	}
	| arg_stmt ',' identifier {
		$<integer>$ = $<integer>1+1;
		if ($<integer>3 == 0) {
			yyerror("undeclared variable");	/* 未声明标识符 */
			exit(1);
		}
		else if (table[$<integer>3].kind != variable) {
			yyerror("%s is not a variable", table[$<ident>3].name);	/* 标识符是过程 */
			exit(1);
		}
		gen(lod, lev-table[$<integer>3].level, table[$<integer>3].adr); /* 取变量 */
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
	| '(' simple_expr ')' {
		$<integer>$ = $<integer>2;
		fprintf(stdout, "%d\n", $<integer>$);
	}
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
		else {
			switch (table[i].kind) {
				case constant:	/* 标识符为常量 */
					gen(lit, 0, table[i].val);	/* 直接把常量的值入栈 */
					$<integer>$ = table[i].val;
					break;
				case variable:	/* 标识符为变量 */
					gen(lod, lev-table[i].level, table[i].adr);	/* 找到变量地址并将其值入栈 */
					$<integer>$ = getValue(i); /* 表达式的值即identifier的值 */
					break;
				case procedure:	/* 标识符为过程 */
					yyerror("cannot be procedure");	/* 不能为过程 */
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

