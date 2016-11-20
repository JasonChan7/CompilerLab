import java.util.*;
import java.io.*;
enum mySymbol {
    nul,        ident,      number,     plus,       minus,      times,
    slash,      oddsym,     eql,        neq,        lss,        leq,
    gtr,        geq,        lparen,     rparen,     comma,      semicolon,
    period,     becomes,    beginsym,   endsym,     ifsym,      thensym,
    whilesym,   writesym,   readsym,    dosym,      calsym,     constsym,
    varsym,     procsym,    repeatsym,  utilsym,    switchsym,  casesym,
    modsym
}
enum myObject {
    constant,   variable,   procedure
}
public class czpp {
    /* define the possible macros */
    private static final int norw = 17;    // the count of reserved words
    private static final int txmax = 100;  // the size of symbol table
    private static final int nmax = 14;    // the max length of integer
    private static final int al = 10;      // the max length of identifier
    private static final int symnum = 37;  // the count of reserved symbols
    private static final int lmax = 81;    // the max length of line

    /* define the possible variables */
    private static char ch;                             // current character
    private static mySymbol sym;                        // current symbol
    private static char[] id = new char[al+1];          // current identifier
    private static int num;                             // current number
    private static int cc, ll;                          // cc: the position of ch
    private static char[] line = new char[lmax];        // current line
    private static char[] a = new char[al+1];           // temporary symbol
    private static String[] word = new String[norw];    // the reserved words
    private static mySymbol[] wsym = new mySymbol[norw];//the value of reserved words
    private static mySymbol[] ssym = new mySymbol[256]; // the value of single-words

    /* initialize the symbol table */
    private static void init() {

        /* initialize the single-word symbol */
        for (int i = 0; i <= 255; i++) ssym[i] = mySymbol.nul;
        ssym['+'] = mySymbol.plus;
        ssym['-'] = mySymbol.minus;
        ssym['*'] = mySymbol.times;
        ssym['/'] = mySymbol.slash;
        ssym['('] = mySymbol.lparen;
        ssym[')'] = mySymbol.rparen;
        ssym['='] = mySymbol.eql;       // caution! small's eql is "=="!
        ssym[','] = mySymbol.comma;
        ssym['#'] = mySymbol.neq;       // caution! small's neq is "!="!
        ssym[';'] = mySymbol.semicolon;
        ssym['%'] = mySymbol.modsym;

        /* initialize the resevered words */
        word[0] = "begin";
        word[1] = "call";
        word[2] = "const";
        word[3] = "do";
        word[4] = "end";
        word[5] = "if";
        word[6] = "odd";
        word[7] = "procedure";
        word[8] = "read";
        word[9] = "then";
        word[10] = "var";
        word[11] = "while";
        word[12] = "write";
        word[13] = "repeat";
        word[14] = "until";
        word[15] = "switch";
        word[16] = "case";
        
        /* initialize the symbol of the reserved words */
        wsym[0] = mySymbol.beginsym;	
        wsym[1] = mySymbol.calsym;
        wsym[2] = mySymbol.constsym;
        wsym[3] = mySymbol.dosym;
        wsym[4] = mySymbol.endsym;
        wsym[5] = mySymbol.ifsym;
        wsym[6] = mySymbol.oddsym;
        wsym[7] = mySymbol.procsym;
        wsym[8] = mySymbol.readsym;
        wsym[9] = mySymbol.thensym;
        wsym[10] = mySymbol.varsym;  
        wsym[11] = mySymbol.whilesym;
        wsym[12] = mySymbol.writesym;
        wsym[13] = mySymbol.repeatsym;
        wsym[14] = mySymbol.utilsym;  
        wsym[15] = mySymbol.switchsym;
        wsym[16] = mySymbol.casesym;

    }
    public static void main(String[] args) throws IOException {
        System.out.println("Please input the small file:");
        Scanner scanner = new Scanner(System.in);
        
        /* try to open the small file */
        String inFileName = scanner.next();
        File inFile = new File(inFileName);
        if (!inFile.exists()) {
            System.out.println("Cannot open the small file!");
            System.exit(0);
        }
        FileReader fileReader = new FileReader(inFile);
        BufferedReader bufferedReader = new BufferedReader(fileReader);
        /* havent consider the empty file condition */
        /* the empty file does not cause any error */
        
        /* try to open the output file */
        File outFile = new File("result.txt");
        if (outFile.exists()) {
            outFile.delete();
            outFile.createNewFile();
        }
        FileWriter fileWriter = new FileWriter(outFile);
        BufferedWriter bufferedWriter = new BufferedWriter(fileWriter);
        String line;
        while ((line=bufferedReader.readLine()) != null) {
            System.out.println(line);
            bufferedWriter.write(line);
            bufferedWriter.newLine();
        }

        /* initialize the symbol table */
        init();


        /* close the input file and out file */
        bufferedReader.close();
        bufferedWriter.close();
    }
}