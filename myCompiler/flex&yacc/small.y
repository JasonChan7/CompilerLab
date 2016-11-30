%{
	#include <stdio.h>
	#include <stdlib.h>
	#define fctnum 8
	/* 虚拟机代码指令 */
	enum fct {
		lit,     opr,     lod, 
		sto,     cal,     ini, 
		jmp,     jpc,     
	};
	int yylex(void);
	void yyerror(char *);
	void gen(enum fct x, int y, int z);
%}
%union {
	int integer; /* integer value */ 
	char ident[15]; /* identifier */ 
}
%token <ident> IDENT
%token <integer> INTEGER
%token IF THEN ELSE END REPEAT UNTIL READ WRITE
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
	IF expr THEN stmt_sequence END
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

