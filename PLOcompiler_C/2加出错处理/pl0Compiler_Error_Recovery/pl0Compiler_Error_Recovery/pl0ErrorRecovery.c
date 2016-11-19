/* 
 * PL/0 complier program (with syntax error recovery) implemented in C
 *
 * The program has been tested on Visual Studio 2010
 *
 * ʹ�÷�����
 * ���к�����PL/0Դ�����ļ���
 * foutput.txt���Դ�ļ�������ʾ�⣨���д�
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define bool int
#define true 1
#define false 0

#define norw 13       /* �����ָ��� */
#define txmax 100     /* ���ű����� */
#define nmax 14       /* ���ֵ����λ�� */
#define al 10         /* ��ʶ������󳤶� */
#define maxerr 30     /* ������������� */


/* ���� */
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

/* ���ű��е����� */
enum object {
    constant, 
    variable, 
    procedure,
};


char ch;            /* ��ŵ�ǰ��ȡ���ַ���getch ʹ�� */
enum symbol sym;    /* ��ǰ�ķ��� */
char id[al+1];      /* ��ǰident�������һ���ֽ����ڴ��0 */
int num;            /* ��ǰnumber */
int cc, ll;         /* getchʹ�õļ�������cc��ʾ��ǰ�ַ�(ch)��λ�� */
char line[81];      /* ��ȡ�л����� */
char a[al+1];       /* ��ʱ���ţ������һ���ֽ����ڴ��0 */
char word[norw][al];        /* ������ */
enum symbol wsym[norw];     /* �����ֶ�Ӧ�ķ���ֵ */
enum symbol ssym[256];      /* ���ַ��ķ���ֵ */
bool declbegsys[symnum];    /* ��ʾ������ʼ�ķ��ż��� */
bool statbegsys[symnum];    /* ��ʾ��俪ʼ�ķ��ż��� */
bool facbegsys[symnum];     /* ��ʾ���ӿ�ʼ�ķ��ż��� */

/* ���ű�ṹ */
struct tablestruct
{
	char name[al];	    /* ���� */
	enum object kind;	/* ���ͣ�const��var��procedure */
};

struct tablestruct table[txmax]; /* ���ű� */

FILE* fin;      /* ����Դ�ļ� */
FILE* foutput;  /* ����ļ�������ʾ�⣨���д� */
char fname[al];
int err; /* ��������� */

void error(int n); 
void getsym();
void getch();
void init();
void test(bool* s1, bool* s2, int n);
int inset(int e, bool* s);
int addset(bool* sr, bool* s1, bool* s2, int n);
int subset(bool* sr, bool* s1, bool* s2, int n);
int mulset(bool* sr, bool* s1, bool* s2, int n);
void block(int tx, bool* fsys);
void factor(bool* fsys, int* ptx);
void term(bool* fsys, int* ptx);
void condition(bool* fsys, int* ptx);
void expression(bool* fsys, int* ptx);
void statement(bool* fsys, int* ptx);
void vardeclaration(int* ptx);
void constdeclaration(int* ptx);
int position(char* idt, int tx);
void enter(enum object k, int* ptx);


/* ������ʼ */
int main()
{
    bool nxtlev[symnum];
    
	printf("Input pl/0 file?   ");
	scanf("%s", fname);		/* �����ļ��� */

	if ((fin = fopen(fname, "r")) == NULL)
	{
		printf("Can't open the input file!\n");
		exit(1);
	}

	ch = fgetc(fin);
	if (ch == EOF)    /* �ļ�Ϊ�� */
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

    init();		/* ��ʼ�� */
	err = 0;
	cc = ll = 0;
	ch = ' ';

	getsym();
			
	addset(nxtlev, declbegsys, statbegsys, symnum);
	nxtlev[period] = true;
	block(0, nxtlev);	/* ����ֳ��� */
			
	if (sym != period)
    {
        error(9);
    }

    if (err == 0)
    {
		printf("\n===Parsing success!===\n");
		fprintf(foutput,"\n===Parsing success!===\n");
    }
	else
	{
		printf("\n===%d errors in pl/0 program!===\n",err);
		fprintf(foutput,"\n===%d errors in pl/0 program!===\n",err);
	}
		
    fclose(foutput);    
	fclose(fin);
	

	return 0;
}

/*
 * ��ʼ�� 
 */
void init()
{
	int i;

	/* ���õ��ַ����� */
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

	/* ���ñ���������,������ĸ˳�򣬱��ڶ��ֲ��� */
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

	/* ���ñ����ַ��� */
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



    /* ���÷��ż� */
	for (i=0; i<symnum; i++)
	{
		declbegsys[i] = false;
		statbegsys[i] = false;
		facbegsys[i] = false;
	}

	/* ����������ʼ���ż� */
	declbegsys[constsym] = true;
	declbegsys[varsym] = true;
	declbegsys[procsym] = true;

	/* ������俪ʼ���ż� */
	statbegsys[beginsym] = true;
	statbegsys[callsym] = true;
	statbegsys[ifsym] = true;
	statbegsys[whilesym] = true;

	/* �������ӿ�ʼ���ż� */
	facbegsys[ident] = true;
	facbegsys[number] = true;
	facbegsys[lparen] = true;
}

/*
 * ������ʵ�ּ��ϵļ������� 
 */
int inset(int e, bool* s)
{
	return s[e];
}

int addset(bool* sr, bool* s1, bool* s2, int n)
{
	int i;
	for (i=0; i<n; i++)
	{
		sr[i] = s1[i]||s2[i];
	}
	return 0;
}

int subset(bool* sr, bool* s1, bool* s2, int n)
{
	int i;
	for (i=0; i<n; i++)
	{
		sr[i] = s1[i]&&(!s2[i]);
	}
	return 0;
}

int mulset(bool* sr, bool* s1, bool* s2, int n)
{
	int i;
	for (i=0; i<n; i++)
	{
		sr[i] = s1[i]&&s2[i];
	}
	return 0;
}

/* 
 *	��������ӡ����λ�úʹ������
 */	
void error(int n)
{
	char space[81];
	memset(space,32,81);

	space[cc-1]=0; /* ����ʱ��ǰ�����Ѿ����꣬����cc-1 */
	
	printf("%s^%d\n", space, n);
	fprintf(foutput,"%s^%d\n", space, n);
	
	err = err + 1;
	if (err > maxerr)
	{
		exit(1);
	}
}

/*
 * ���˿ո񣬶�ȡһ���ַ�
 * ÿ�ζ�һ�У�����line��������line��getsymȡ�պ��ٶ�һ��
 * ������getsym����
 */
void getch()
{
	if (cc == ll) /* �жϻ��������Ƿ����ַ��������ַ����������һ���ַ����������� */
	{
		if (feof(fin))
		{
			printf("Program incomplete!\n");
			exit(1);
		}
		ll = 0;
		cc = 0;
	
		ch = ' ';
		while (ch != 10)
		{
            if (EOF == fscanf(fin,"%c", &ch))   
            {               
                line[ll] = 0;
                break;
            }                                   
            
			printf("%c", ch);
			fprintf(foutput, "%c", ch);
			line[ll] = ch;
			ll++;
		}
	}
	ch = line[cc];
	cc++;
}

/* 
 * �ʷ���������ȡһ������
 */
void getsym()
{
	int i,j,k;

	while (ch == ' ' || ch == 10 || ch == 9)	/* ���˿ո񡢻��к��Ʊ�� */
	{
		getch();
	}
	if ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) /* ��ǰ�ĵ����Ǳ�ʶ�����Ǳ����� */
	{			
		k = 0;
		do {
			if(k < al)
			{
				a[k] = ch;
				k++;
			}
			getch();
		} while ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9'));
		a[k] = 0;
		strcpy(id, a);
		i = 0;
		j = norw - 1;
		do {    /* ������ǰ�����Ƿ�Ϊ�����֣�ʹ�ö��ַ����� */
			k = (i + j) / 2;
			if (strcmp(id,word[k]) <= 0)
			{
			    j = k - 1;
			}
			if (strcmp(id,word[k]) >= 0)
			{
			    i = k + 1;
			}
		} while (i <= j);
		if (i-1 > j) /* ��ǰ�ĵ����Ǳ����� */
		{
		    sym = wsym[k];
		}
		else /* ��ǰ�ĵ����Ǳ�ʶ�� */
		{
		    sym = ident; 
		}
	}
	else
	{
		if (ch >= '0' && ch <= '9') /* ��ǰ�ĵ��������� */
		{			
			k = 0;
			num = 0;
			sym = number;
			do {
				num = 10 * num + ch - '0';
				k++;
				getch();
			} while (ch >= '0' && ch <= '9'); /* ��ȡ���ֵ�ֵ */
			k--;
			if (k > nmax) /* ����λ��̫�� */
			{
			    error(30);
			}
		}
		else
		{
			if (ch == ':')		/* ��⸳ֵ���� */
			{
				getch();
				if (ch == '=')
				{
					sym = becomes;
					getch();
				}
				else
				{
					sym = nul;	/* ����ʶ��ķ��� */
				}
			}
			else
			{
				if (ch == '<')		/* ���С�ڻ�С�ڵ��ڷ��� */
				{
					getch();
					if (ch == '=')
					{
						sym = leq;
						getch();
					}
					else
					{
						sym = lss;
					}
				}
				else
				{
					if (ch == '>')		/* �����ڻ���ڵ��ڷ��� */
					{
						getch();
						if (ch == '=')
						{
							sym = geq;
							getch();
						}
						else
						{
							sym = gtr;
						}
					}
					else
					{
						sym = ssym[ch];		/* �����Ų�������������ʱ��ȫ�����յ��ַ����Ŵ��� */                   
                        if (sym != period)  
                        {
                            getch();        
                        }
                   
					}
				}
			}
		}
	}
}



/* 
 * ���Ե�ǰ�����Ƿ�Ϸ�
 *
 * ���﷨�����������ںͳ��ڴ����ò��Ժ���test��
 * ��鵱ǰ���ʽ�����˳����﷨��λ�ĺϷ���
 *
 * s1:	��Ҫ�ĵ��ʼ���
 * s2:	���������Ҫ�ĵ��ʣ���ĳһ����״̬ʱ��
 *      �ɻָ��﷨�����������������Ĳ��䵥�ʷ��ż���
 * n:  	�����
 */
void test(bool* s1, bool* s2, int n)
{
	if (!inset(sym, s1))
	{
		error(n);
		/* ����ⲻͨ��ʱ����ͣ��ȡ���ţ�ֱ����������Ҫ�ļ��ϻ򲹾ȵļ��� */
		while ((!inset(sym,s1)) && (!inset(sym,s2)))
		{
			getsym();
		}
	}	
}

/* 
 * �����������
 *
 * tx:     ���ű�ǰβָ��
 * fsys:   ��ǰģ���̷��ż���
 */
void block(int tx, bool* fsys)
{
	int i;	
	bool nxtlev[symnum];    /* ���¼������Ĳ����У����ż��Ͼ�Ϊֵ�Σ�������ʹ������ʵ�֣�
	                           ���ݽ�������ָ�룬Ϊ��ֹ�¼������ı��ϼ������ļ��ϣ������µĿռ�
	                           ���ݸ��¼�����*/
	do {

		if (sym == constsym)	/* ���������������ţ���ʼ���������� */
		{
			getsym();

			do {
			    constdeclaration(&tx);	
		        while (sym == comma)  /* �������ż������峣�� */
				{
					getsym();
					constdeclaration(&tx);
				}
				if (sym == semicolon) /* �����ֺŽ������峣�� */
				{
					getsym();
				}
				else 
                {
                    error(5);   /* ©���˷ֺ� */
                }
			} while (sym == ident);
		}

		if (sym == varsym)		/* ���������������ţ���ʼ����������� */
		{
			getsym();

			do {
			    vardeclaration(&tx);
				while (sym == comma) 
				{
					getsym();
					vardeclaration(&tx);
				}
				if (sym == semicolon)
				{
					getsym();
				}
				else
                {
                    error(5); /* ©���˷ֺ� */
                }
			} while (sym == ident);
		}

		while (sym == procsym) /* ���������������ţ���ʼ����������� */
		{
			getsym();

			if (sym == ident)
			{
				enter(procedure, &tx);	/* ��д���ű� */
				getsym();
			}
            else 
            {
                error(4);	/* procedure��ӦΪ��ʶ�� */
            }

			if (sym == semicolon)
			{
				getsym();
			}
			else 
            {
                error(5);	/* ©���˷ֺ� */
            }

			memcpy(nxtlev, fsys, sizeof(bool) * symnum);
			nxtlev[semicolon] = true;
			block(tx, nxtlev); /* �ݹ���� */
            
			if(sym == semicolon)
			{
				getsym();
				memcpy(nxtlev, statbegsys, sizeof(bool) * symnum);
				nxtlev[ident] = true;
				nxtlev[procsym] = true;
				test(nxtlev, fsys, 6);
			}
			else 
            {
                error(5);	/* ©���˷ֺ� */
            }
		}
		memcpy(nxtlev, statbegsys, sizeof(bool) * symnum);
		nxtlev[ident] = true;
		test(nxtlev, declbegsys, 7);
	} while (inset(sym, declbegsys));	/* ֱ��û���������� */

	

	/* ����̷���Ϊ�ֺŻ�end */
	memcpy(nxtlev, fsys, sizeof(bool) * symnum);	/* ÿ����̷��ż��϶������ϲ��̷��ż��ϣ��Ա㲹�� */
	nxtlev[semicolon] = true;
	nxtlev[endsym] = true;
	statement(nxtlev, &tx);	
	memset(nxtlev, 0, sizeof(bool) * symnum);	/* �ֳ���û�в��ȼ��� */
	test(fsys, nxtlev, 8);            	/* ����̷�����ȷ�� */
}

/* 
 * �ڷ��ű��м���һ�� 
 *
 * k:      ��ʶ��������Ϊconst��var��procedure
 * ptx:    ���ű�βָ���ָ�룬Ϊ�˿��Ըı���ű�βָ���ֵ 
 * 
 */
void enter(enum object k, int* ptx)
{
	(*ptx)++;
	strcpy(table[(*ptx)].name, id); /* ���ű��name���¼��ʶ�������� */
	table[(*ptx)].kind = k;	
	
}

/* 
 * ���ұ�ʶ���ڷ��ű��е�λ�ã���tx��ʼ������ұ�ʶ��
 * �ҵ��򷵻��ڷ��ű��е�λ�ã����򷵻�0
 * 
 * id:    Ҫ���ҵ�����
 * tx:     ��ǰ���ű�βָ��
 */
int position(char* id, int tx)
{
	int i;
	strcpy(table[0].name, id);
	i = tx;
	while (strcmp(table[i].name, id) != 0)
    {
        i--;
    }
	return i;
}

/*
 * ������������ 
 */
void constdeclaration(int* ptx)
{
	if (sym == ident)
	{
		getsym();
		if (sym == eql || sym == becomes)
		{
			if (sym == becomes)
            {
                error(1);	/* ��=д����:= */
            }
			getsym();
			if (sym == number)
			{
				enter(constant, ptx);
				getsym();
			}
			else
            {
                error(2);	/* ���������е�=��Ӧ������ */
            }
		}
		else 
        {
            error(3);	/* ���������еı�ʶ����Ӧ��= */
        }
	}
	else 
    {
        error(4);	/* const��Ӧ�Ǳ�ʶ�� */
    }
}

/*
 * ������������ 
 */
void vardeclaration(int* ptx)
{
	if (sym == ident)
	{
		enter(variable, ptx);	// ��д���ű�
		getsym();
	}
    else 
	{
		error(4);	/* var����Ӧ�Ǳ�ʶ�� */
	}	
}


/*
 * ��䴦�� 
 */
void statement(bool* fsys, int* ptx)
{
	int i;
	bool nxtlev[symnum];

	if (sym == ident)	/* ׼�����ո�ֵ��䴦�� */
	{
		i = position(id, *ptx);/* ���ұ�ʶ���ڷ��ű��е�λ�� */
		if (i == 0)
		{
			error(11);	/* ��ʶ��δ���� */
		}
		else
		{
			if(table[i].kind != variable)
            {
				error(12);	/* ��ֵ����У���ֵ���󲿱�ʶ��Ӧ���Ǳ��� */
				i = 0;
			}
            else
            {
                getsym();
		        if(sym == becomes) 
		        {
			        getsym();
		        }
		        else 
		        {
			        error(13);	/* û�м�⵽��ֵ���� */
		        }
		        memcpy(nxtlev, fsys, sizeof(bool) * symnum);
		        expression(nxtlev, ptx);	/* ����ֵ�����Ҳ���ʽ */
		       
            }
        }
	}
	else
	{
		if (sym == readsym)	/* ׼������read��䴦�� */
		{
			getsym();
			if (sym != lparen)
			{
				error(34);	/* ��ʽ����Ӧ�������� */
			}
			else
			{
				do {
					getsym();
					if (sym == ident)
					{
						i = position(id, *ptx);	/* ����Ҫ���ı��� */
					}
					else
					{
						i = 0;
					}

					if (i == 0)
					{
						error(35);	/* read��������еı�ʶ��Ӧ�����������ı��� */
					}
					else
					{
					
					}
					getsym();

				} while (sym == comma);	/* һ��read���ɶ�������� */
			}
			if(sym != rparen) 
			{
				error(33);	/* ��ʽ����Ӧ�������� */
				while (!inset(sym, fsys))	/* �����ȣ�ֱ�������ϲ㺯���ĺ�̷��� */
				{
					getsym();
				}
			}
			else
			{
				getsym();
			}
		}
		else
		{
			if (sym == writesym)	/* ׼������write��䴦�� */
			{
				getsym();
				if (sym == lparen)
				{
					do {
						getsym();
						memcpy(nxtlev, fsys, sizeof(bool) * symnum);
						nxtlev[rparen] = true;
						nxtlev[comma] = true;		
						expression(nxtlev, ptx);	/* ���ñ��ʽ���� */						
					} while (sym == comma);  /* һ��write��������������ֵ */
					if (sym != rparen)
					{
						error(33);	/* ��ʽ����Ӧ�������� */
					}
					else
					{
						getsym();
					}
				}				
			}
			else
			{
				if (sym == callsym)	/* ׼������call��䴦�� */
				{
					getsym();
					if (sym != ident)
					{
						error(14);	/* call��ӦΪ��ʶ�� */
					}
					else
					{
						i = position(id, *ptx);
						if (i == 0)
						{
							error(11);	/* ������δ�ҵ� */
						}
						else
						{
							if (table[i].kind != procedure)
							{
								error(15);	/* call���ʶ������ӦΪ���� */
							}
						}
						getsym();
					}
				}
				else
				{
					if (sym == ifsym)	/* ׼������if��䴦�� */
					{
						getsym();
						memcpy(nxtlev, fsys, sizeof(bool) * symnum);
						nxtlev[thensym] = true;
						nxtlev[dosym] = true;	/* ��̷���Ϊthen��do */
						condition(nxtlev, ptx); /* �������������߼����㣩���� */
						if (sym == thensym) 
						{
							getsym();
						}
						else
						{
							error(16);	/* ȱ��then */
						}
						statement(fsys, ptx);	/* ����then������ */
						
					}
					else
					{
						if (sym == beginsym)	/* ׼�����ո�����䴦�� */
						{
							getsym();
							memcpy(nxtlev, fsys, sizeof(bool) * symnum);
							nxtlev[semicolon] = true;
							nxtlev[endsym] = true;	/* ��̷���Ϊ�ֺŻ�end */							
							statement(nxtlev, ptx); /* ��begin��end֮��������з������� */
							/* ���������һ���������俪ʼ����ֺţ���ѭ��������һ����� */
							while (inset(sym, statbegsys) || sym == semicolon) 
							{
								if (sym == semicolon)
								{
									getsym();
								}
								else 
								{
									error(10);	/* ȱ�ٷֺ� */
								}
								statement(nxtlev, ptx);
							}
							if(sym == endsym) 
							{
								getsym();
							}
							else 
							{
								error(17);	/* ȱ��end */
							}
						}
						else
						{
							if (sym == whilesym)	/* ׼������while��䴦�� */
							{
								getsym();
								memcpy(nxtlev, fsys, sizeof(bool) * symnum);
								nxtlev[dosym] = true;	/* ��̷���Ϊdo */
								condition(nxtlev, ptx);	/* ������������ */								
								if (sym == dosym)
								{
									getsym();
								}
								else 
								{
									error(18);	/* ȱ��do */
								}
								statement(fsys, ptx);	/* ѭ���� */
								
							}                            
						}
					}
				}
			}
		}
	}
	memset(nxtlev, 0, sizeof(bool) * symnum);	/* �������޲��ȼ��� */
	test(fsys, nxtlev, 19);	/* �������������ȷ�� */
}

/*
 * ���ʽ���� 
 */
void expression(bool* fsys, int* ptx)
{
	bool nxtlev[symnum];

	if(sym == plus || sym == minus)	/* ���ʽ��ͷ�������ţ���ʱ��ǰ���ʽ������һ�����Ļ򸺵��� */
	{	
		getsym();
		memcpy(nxtlev, fsys, sizeof(bool) * symnum);
		nxtlev[plus] = true;
		nxtlev[minus] = true;
		term(nxtlev, ptx);	/* ������ */
		
	}
	else	/* ��ʱ���ʽ��������ļӼ� */
	{
		memcpy(nxtlev, fsys, sizeof(bool) * symnum);
		nxtlev[plus] = true;
		nxtlev[minus] = true;
		term(nxtlev, ptx);	/* ������ */
	}
	while (sym == plus || sym == minus)
	{

		getsym();
		memcpy(nxtlev, fsys, sizeof(bool) * symnum);
		nxtlev[plus] = true;
		nxtlev[minus] = true;
		term(nxtlev, ptx);	/* ������ */
		
	}	
}

/*
 * ��� 
 */
void term(bool* fsys, int* ptx)
{
	bool nxtlev[symnum];
	memcpy(nxtlev, fsys, sizeof(bool) * symnum);
	nxtlev[times] = true;
	nxtlev[slash] = true;
	factor(nxtlev, ptx);	/* �������� */
	while(sym == times || sym == slash)
	{
		getsym();
		factor(nxtlev, ptx);		
	}	
}

/* 
 * ���Ӵ��� 
 */
void factor(bool* fsys, int* ptx)
{
	int i;
	bool nxtlev[symnum];
	test(facbegsys, fsys, 24);	/* ������ӵĿ�ʼ���� */
	while(inset(sym, facbegsys)) 	/* ѭ���������� */
	{

		if(sym == ident)	/* ����Ϊ��������� */
		{
			i = position(id, *ptx);	/* ���ұ�ʶ���ڷ��ű��е�λ�� */
			if (i == 0)
			{
				error(11);	/* ��ʶ��δ���� */
			}
			else
			{
				if (table[i].kind == procedure)
				{
					error(21);	/* ����Ϊ���� */						
				}
			}
			getsym();
		}
		else
		{
			if(sym == number)	/* ����Ϊ�� */
			{
				getsym();
			}
			else
			{
				if (sym == lparen)	/* ����Ϊ���ʽ */
				{
					getsym();
					memcpy(nxtlev, fsys, sizeof(bool) * symnum);
					nxtlev[rparen] = true;
					expression(nxtlev, ptx);
					if (sym == rparen)
					{
						getsym();
					}
					else 
					{
						error(22);	/* ȱ�������� */
					}
				}				
			}
		}
		memset(nxtlev, 0, sizeof(bool) * symnum);
		nxtlev[lparen] = true;
		test(fsys, nxtlev, 23); /* һ�����Ӵ�����ϣ������ĵ���Ӧ��fsys������ */
		                        /* ������ǣ������ҵ���һ�����ӵĿ�ʼ��ʹ�﷨�������Լ���������ȥ */
	}
}

/* 
 * �������� 
 */
void condition(bool* fsys, int* ptx)
{
	bool nxtlev[symnum];
	
	if(sym == oddsym)	/* ׼������odd���㴦�� */
	{
		getsym();
		expression(fsys, ptx);

	}
	else
	{
		/* �߼����ʽ���� */
		memcpy(nxtlev, fsys, sizeof(bool) * symnum);
		nxtlev[eql] = true;
		nxtlev[neq] = true;
		nxtlev[lss] = true;
		nxtlev[leq] = true;
		nxtlev[gtr] = true;
		nxtlev[geq] = true;
		expression(nxtlev, ptx); 
		if (sym != eql && sym != neq && sym != lss && sym != leq && sym != gtr && sym != geq)
		{
			error(20); /* Ӧ��Ϊ��ϵ����� */
		}
		else
		{			
			getsym();
			expression(fsys, ptx);			
		}
	}
}
