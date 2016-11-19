import java.util.*;
import java.io.*;
// import java.io.BufferedReader;
// import java.io.BufferedWriter;
// import java.io.FileReader;
// import java.io.FileWriter;
public class czpp {
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
        bufferedReader.close();
        bufferedWriter.close();
    }
}