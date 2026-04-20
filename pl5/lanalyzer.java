import java.io.*;

public class lanalyzer {

    // Variables
    static int charClass;
    static char[] lexeme = new char[100];
    static int nextChar;
    static int lexLen;
    static int token;
    static int nextToken;
    static Reader in_fp;

    // Character classes
    static final int LETTER = 0;
    static final int DIGIT = 1;
    static final int UNKNOWN = 99;
    static final int EOF = -1;

    // token codes
    static final int INT_LIT = 10;
    static final int IDENT = 11;
    static final int ASSIGN_OP = 20;
    static final int ADD_OP = 21;
    static final int SUB_OP = 22;
    static final int MULT_OP = 23;
    static final int DIV_OP = 24;
    static final int LEFT_PAREN = 25;
    static final int RIGHT_PAREN = 26;

    public static void main(String[] args) throws Exception {
        int argc = args.length;
        if (argc == 0) {
            File f = new File("front.in");
            if (!f.canRead()) {
                System.out.println("ERROR - cannot open front.in");
                System.exit(1);
            }
            in_fp = new FileReader(f);
        } else if (argc == 1) {
            if ("-f".equals(args[0])) {
                System.out.println("ERROR - missing filename after -f");
                System.exit(1);
            }
            in_fp = new StringReader(args[0] + "\n");
        } else {
            if ("-f".equals(args[0]) && argc >= 2) {
                File f = new File(args[1]);
                if (!f.canRead()) {
                    System.out.println("ERROR - cannot open " + args[1]);
                    System.exit(1);
                }
                in_fp = new FileReader(f);
            } else {
                System.out.println("Usage: java lanalyzer [\"expression\"] | -f filename");
                System.exit(1);
            }
        }

        getChar();
        do {
            lex();
        } while (nextToken != EOF);
    }

    static int lookup(int ch) {
        switch (ch) {
            case '(':
                addChar();
                nextToken = LEFT_PAREN;
                break;
            case ')':
                addChar();
                nextToken = RIGHT_PAREN;
                break;
            case '+':
                addChar();
                nextToken = ADD_OP;
                break;
            case '-':
                addChar();
                nextToken = SUB_OP;
                break;
            case '*':
                addChar();
                nextToken = MULT_OP;
                break;
            case '/':
                addChar();
                nextToken = DIV_OP;
                break;
            default:
                addChar();
                nextToken = EOF;
        }
        return nextToken;
    }

    static void addChar() {
        if (lexLen <= 98) {
            lexeme[lexLen++] = (char) nextChar;
        } else {
            System.out.println("Error - lexeme is too long");
        }
    }

    static void getChar() throws IOException {
        if (in_fp == null) {
            nextChar = EOF;
            charClass = EOF;
            return;
        }
        int c = in_fp.read();
        if (c != -1) {
            nextChar = c;
            if (Character.isLetter((char) nextChar))
                charClass = LETTER;
            else if (Character.isDigit((char) nextChar))
                charClass = DIGIT;
            else
                charClass = UNKNOWN;
        } else {
            nextChar = EOF;
            charClass = EOF;
        }
    }

    static void getNonBlank() throws IOException {
        while (nextChar != EOF && Character.isWhitespace((char) nextChar))
            getChar();
    }

    static int lex() throws IOException {
        lexLen = 0;
        getNonBlank();
        switch (charClass) {
            case LETTER:
                addChar();
                getChar();
                while (charClass == LETTER || charClass == DIGIT) {
                    addChar();
                    getChar();
                }
                nextToken = IDENT;
                break;
            case DIGIT:
                addChar();
                getChar();
                while (charClass == DIGIT) {
                    addChar();
                    getChar();
                }
                nextToken = INT_LIT;
                break;
            case UNKNOWN:
                lookup(nextChar);
                getChar();
                break;
            case EOF:
                nextToken = EOF;
                lexeme[0] = 'E';
                lexeme[1] = 'O';
                lexeme[2] = 'F';
                lexLen = 3;
                break;
            default:
                nextToken = EOF;
                break;
        }
        String lexStr = new String(lexeme, 0, Math.max(0, lexLen));
        System.out.println("Next token is: " + nextToken + ", Next lexeme is " + lexStr);
        return nextToken;
    }
}
