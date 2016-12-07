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
	char a[al+1];       /* 临时符号，多出的一个字节用于存放0 */
	struct instruction code[cxmax]; /* 存放虚拟机代码的数组 */
	char word[norw][al];        /* 保留字 */
	enum symbol wsym[norw];     /* 保留字对应的符号值 */
	enum symbol ssym[256];      /* 单字符的符号值 */
	char mnemonic[fctnum][5];   /* 虚拟机代码指令名称 */
	bool declbegsys[symnum];    /* 表示声明开始的符号集合 */
	bool statbegsys[symnum];    /* 表示语句开始的符号集合 */
	bool facbegsys[symnum];     /* 表示因子开始的符号集合 */
	int symbolTablePtr = 0; 	/* point the last symbol of table */

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
	extern int line;
	int yylex(void);
	void yyerror(char *);
	void gen(enum fct x, int y, int z);
	int position(char* idt);
	void enter(enum object k, char *currIdent, int currValue);
	void interpret(void);
	int base(int l, int* s, int b);
	void init(void);
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
	identifier BC expr {
		int i = position($<ident>1);
		if (i != 0) {
			yyerror("redeclaration variable");	/* 标识符已声明 */
			exit(1);
		}
		enter(variable, $<ident>1, $<integer>3);
	}
	;
read_stmt:
	READ identifier
	;
write_stmt:
	WRITE expr {
		fprintf(stdout, "%d\n", $<integer>2);
	}
	;
expr:
	simple_expr
	| simple_expr EQ simple_expr {
		gen(opr, 0, 8);
	}
	| simple_expr NE simple_expr {
		gen(opr, 0, 9);
	}
	| simple_expr LT simple_expr {
		gen(opr, 0, 10);
	}
	| simple_expr GE simple_expr {
		gen(opr, 0, 11);
	}
	| simple_expr GT simple_expr {
		gen(opr, 0, 12);
	}
	| simple_expr LE simple_expr {
		gen(opr, 0, 13);
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
		if (i == 0) {
			yyerror("undefined variable");	/* 标识符未声明 */
		}
		else {
			switch (table[i].kind) {
				case constant:	/* 标识符为常量 */
					gen(lit, 0, table[i].val);	/* 直接把常量的值入栈 */
					$<integer>$ = table[i].val;
					break;
				case variable:	/* 标识符为变量 */
					gen(lod, table[i].level, table[i].adr);	/* 找到变量地址并将其值入栈 */
					$<integer>$ = table[i].val;
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
		strcpy($<ident>$, $<ident>1);
	}
	;
%%
void yyerror(char *s) {
	fprintf(stdout, "line %d error: %s\n", line, s);
}
void gen(enum fct x, int y, int z) {
	if (cx >= cxmax) {
		yyerror("Program is too long!");	/* 生成的虚拟机代码程序过长 */
		exit(1);
	}
	if (z >= amax) {
		yyerror("Displacement address is too big!");	/* 地址偏移越界 */
		exit(1);
	}
	code[cx].f = x;
	code[cx].l = y;
	code[cx].a = z;
	cx++;
}
/* 
 * 查找标识符在符号表中的位置，从tx开始倒序查找标识符
 * 找到则返回在符号表中的位置，否则返回0
 * 
 * id:    要查找的名字
 */
int position(char* id) {
	int i = symbolTablePtr;
	strcpy(table[0].name, id);
	while (i>0 && strcmp(table[i].name, id) != 0) {
        i--;
    }
	return i;
}
/* 
 * 在符号表中加入一项 
 *
 * k:      标识符的种类为const，var或procedure
 * ptx:    符号表尾指针的指针，为了可以改变符号表尾指针的值
 * lev:    标识符所在的层次
 * pdx:    dx为当前应分配的变量的相对地址，分配后要增加1
 * 
 */
void enter(enum object k, char *currIdent, int currValue) {
	symbolTablePtr++;
	strcpy(table[(symbolTablePtr)].name, currIdent); /* 符号表的name域记录标识符的名字 */
	table[(symbolTablePtr)].kind = k;	
	switch (k) {
		case constant:	/* 常量 */
			if (currValue > amax) {
				yyerror("constant out of bound");	/* 常数越界 */
				currValue = 0;
			}
			table[(symbolTablePtr)].val = currValue; /* 登记常数的值 */
			break;
		case variable:	/* 变量 */
			table[(symbolTablePtr)].val = currValue; /* 登记变量的值 */
			break;
		case procedure:	/* 过程 */
			break;
	}
}
/*
 * 解释程序 
 */
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
/* 通过过程基址求上l层过程的基址 */
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
/*
 * 初始化 
 */
void init()
{
	int i;

	/* 设置单字符符号 */
	for (i=0; i<=255; i++)
	{
	    ssym[i] = nul;
	}
	ssym['+'] = plus;
	ssym['-'] = minus;
	ssym['*'] = times;
	ssym['/'] = slash;
	ssym['('] = lparen;
	ssym[')'] = rparen;
	ssym['='] = eql;
	ssym[','] = comma;
	ssym['.'] = period;
	ssym['#'] = neq;
	ssym[';'] = semicolon;

	/* 设置保留字名字,按照字母顺序，便于二分查找 */
	strcpy(&(word[0][0]), "begin");
	strcpy(&(word[1][0]), "call");
	strcpy(&(word[2][0]), "const");
	strcpy(&(word[3][0]), "do");
	strcpy(&(word[4][0]), "end");
	strcpy(&(word[5][0]), "if");
	strcpy(&(word[6][0]), "odd");
	strcpy(&(word[7][0]), "procedure");
	strcpy(&(word[8][0]), "read");
	strcpy(&(word[9][0]), "then");
    strcpy(&(word[10][0]), "var");
    strcpy(&(word[11][0]), "while");
    strcpy(&(word[12][0]), "write");

	/* 设置保留字符号 */
	wsym[0] = beginsym;	
	wsym[1] = callsym;
	wsym[2] = constsym;
	wsym[3] = dosym;
    wsym[4] = endsym;
	wsym[5] = ifsym;
	wsym[6] = oddsym;
	wsym[7] = procsym;
	wsym[8] = readsym;
    wsym[9] = thensym;
    wsym[10] = varsym;  
	wsym[11] = whilesym;
	wsym[12] = writesym;

	/* 设置指令名称 */
	strcpy(&(mnemonic[lit][0]), "lit");
	strcpy(&(mnemonic[opr][0]), "opr");
	strcpy(&(mnemonic[lod][0]), "lod");
	strcpy(&(mnemonic[sto][0]), "sto");
	strcpy(&(mnemonic[cal][0]), "cal");
	strcpy(&(mnemonic[ini][0]), "int");
	strcpy(&(mnemonic[jmp][0]), "jmp");
	strcpy(&(mnemonic[jpc][0]), "jpc");

    /* 设置符号集 */
	for (i=0; i<symnum; i++)
	{
		declbegsys[i] = false;
		statbegsys[i] = false;
		facbegsys[i] = false;
	}

	/* 设置声明开始符号集 */
	declbegsys[constsym] = true;
	declbegsys[varsym] = true;
	declbegsys[procsym] = true;

	/* 设置语句开始符号集 */
	statbegsys[beginsym] = true;
	statbegsys[callsym] = true;
	statbegsys[ifsym] = true;
	statbegsys[whilesym] = true;

	/* 设置因子开始符号集 */
	facbegsys[ident] = true;
	facbegsys[number] = true;
	facbegsys[lparen] = true;
}
/* 主程序开始 */
int main()
{
	bool nxtlev[symnum];
    
	printf("Input small test file?   ");
	scanf("%s", fname);		/* 输入文件名 */

	if ((fin = fopen(fname, "r")) == NULL)
	{
		printf("Can't open the input file!\n");
		exit(1);
	}

	ch = fgetc(fin);
	if (ch == EOF)
	{
		printf("The input file is empty!\n");
		fclose(fin);
		exit(1);
	}
	rewind(fin);

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

    init();		/* 初始化 */
	err = 0;
	cc = ll = cx = 0;
	ch = ' ';

	getsym();
		
	addset(nxtlev, declbegsys, statbegsys, symnum);
	nxtlev[period] = true;
	block(0, 0, nxtlev);	/* 处理分程序 */
		
	if (sym != period)
    {
		error(9);
    }
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

