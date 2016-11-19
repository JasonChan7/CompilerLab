/* 
 * PL/0 complier program (syntax analysis only) implemented in C
 *
 * The program has been tested on Visual Studio 2010
 *
 * ʹ�÷�����
 * ���к�����PL/0Դ�����ļ���
 * foutput.txt���Դ�ļ�������ʾ�⣨���д�
 * һ�����������ֹͣ�﷨����
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


void error(int n); 
void getsym();
void getch();
void init();
void block(int tx);
void factor(int* ptx);
void term(int* ptx);
void condition(int* ptx);
void expression(int* ptx);
void statement(int* ptx);
void vardeclaration(int* ptx);
void constdeclaration(int* ptx);
int position(char* idt, int tx);
void enter(enum object k, int* ptx);


/* ������ʼ */
int main()
{
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
	cc = ll = 0;
	ch = ' ';

	getsym();
			
	block(0);	/* ����ֳ��� */
			
	if (sym != period)
    {
        error(9);
    }

	else 
    {
		printf("\n===Parsing success!===\n");
		fprintf(foutput,"\n===Parsing success!===\n");
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
}


/* 
 *	��������ӡ����λ�úʹ������
 *  ����������˳��﷨����
 */	
void error(int n)
{
	char space[81];
	memset(space,32,81);

	space[cc-1]=0; /* ����ʱ��ǰ�����Ѿ����꣬����cc-1 */
	
	printf("%s^%d\n", space, n);
	fprintf(foutput,"%s^%d\n", space, n);
	
	exit(1);
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
		}while ((ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9'));
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
 * �����������
 *
 * tx:     ���ű�ǰβָ�� 
 */
void block(int tx)
{
	int i;

	if (sym == constsym)	/* ���������������ţ���ʼ���������� */
	{
		getsym();
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
	}

	if (sym == varsym)		/* ���������������ţ���ʼ����������� */
	{
		getsym();
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
		
		block(tx); /* �ݹ���� */
            
		if(sym == semicolon)
		{
			getsym();				
		}
		else 
        {
            error(5);	/* ©���˷ֺ� */
        }
	}
	statement(&tx);		
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
		if (sym == eql)
		{
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
void statement(int* ptx)
{
	int i;

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
		        expression(ptx);	/* ����ֵ�����Ҳ���ʽ */		       
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
					
					getsym();

				} while (sym == comma);	/* һ��read���ɶ�������� */
			}
			if(sym != rparen) 
			{
				error(33);	/* ��ʽ����Ӧ�������� */				
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
						expression(ptx);	/* ���ñ��ʽ���� */						
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
						condition(ptx); /* ������������ */
						if (sym == thensym) 
						{
							getsym();
						}
						else
						{
							error(16);	/* ȱ��then */
						}
						statement(ptx);	/* ����then������ */						
					}
					else
					{
						if (sym == beginsym)	/* ׼�����ո�����䴦�� */
						{
							getsym();													
							statement(ptx); /* ��begin��end֮��������з������� */
							/* ���������һ���������俪ʼ����ֺţ���ѭ��������һ����� */
							while (sym == semicolon) 
							{
								getsym();
								statement(ptx);
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
								condition(ptx);	/* ������������ */								
								if (sym == dosym)
								{
									getsym();
								}
								else 
								{
									error(18);	/* ȱ��do */
								}
								statement(ptx);	/* ѭ���� */								
							}                            
						}
					}
				}
			}
		}
	}
}

/*
 * ���ʽ���� 
 */
void expression(int* ptx)
{
	if(sym == plus || sym == minus)	/* ���ʽ��ͷ�������ţ���ʱ��ǰ���ʽ������һ�����Ļ򸺵��� */
	{	
		getsym();		
		term(ptx);	/* ������ */		
	}
	else	/* ��ʱ���ʽ��������ļӼ� */
	{
		term(ptx);	/* ������ */
	}
	while (sym == plus || sym == minus)
	{
		getsym();	
		term(ptx);	/* ������ */		
	}	
}

/*
 * ��� 
 */
void term(int* ptx)
{
	factor(ptx);	/* �������� */
	while(sym == times || sym == slash)
	{
		getsym();
		factor(ptx);		
	}	
}

/* 
 * ���Ӵ��� 
 */
void factor(int* ptx)
{
	int i;
	
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
				expression(ptx);
				if (sym == rparen)
				{
					getsym();
				}
				else 
				{
					error(22);	/* ȱ�������� */
				}
			}	
			else
			{
				error(23);
			}
		}
	}		
}

/* 
 * �������� 
 */
void condition(int* ptx)
{
	if(sym == oddsym)	/* ׼������odd���㴦�� */
	{
		getsym();
		expression(ptx);
	}
	else
	{
		expression(ptx); 
		if (sym != eql && sym != neq && sym != lss && sym != leq && sym != gtr && sym != geq)
		{
			error(20); /* Ӧ��Ϊ��ϵ����� */
		}
		else
		{			
			getsym();
			expression(ptx);			
		}
	}
}
