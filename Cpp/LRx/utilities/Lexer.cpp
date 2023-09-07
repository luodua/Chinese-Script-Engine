#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#define MAX_LEXEME_LENGTH 1000
#define MAX_SYMBOLS 1000
void error(const char *message) {
    fprintf(stderr, "Error: %s\n", message);
    //exit(1);
}
typedef enum {
    KEY,    // �ؼ���
    IDENTIFIER, // ��ʶ��
    CONSTANT,   // ����
    OPERATOR,   // �����
    DELIMITER   // �ָ���
} SymbolType;

typedef struct {
    SymbolType type;
    char lexeme[MAX_LEXEME_LENGTH];  // ���ڴ洢��ʶ���������ַ���
    int value;
    
} Symbol;

// ���ű���
typedef struct {
    char lexeme[MAX_LEXEME_LENGTH];  // ��ʶ��������
    SymbolType type;                  // ��ʶ��������
} SymbolTableEntry;

// ���ű�

SymbolTableEntry symbolTable[MAX_SYMBOLS];
int symbolTableSize = 0;

// ����ű������Ŀ
void addToSymbolTable(char* lexeme, SymbolType type) {
    if (symbolTableSize >= MAX_SYMBOLS) {
        error("Symbol table is full.");
    }

    strcpy(symbolTable[symbolTableSize].lexeme, lexeme);
    symbolTable[symbolTableSize].type = type;
    symbolTableSize++;
}

// �ڷ��ű��в�����Ŀ

//-std=c11 or -std=gnu11 to compile your code
SymbolTableEntry* lookup(char* lexeme) {
	int i;
    for (i = 0; i < MAX_SYMBOLS; i++) {
        if (strcmp(symbolTable[i].lexeme, lexeme) == 0) {
            return &symbolTable[i];
        }
    }
    return NULL;  // δ�ҵ�ƥ�����Ŀ
}

// ��ʼ�����ű�
void initializeSymbolTable() {
    // ��ӻ���������
    addToSymbolTable((char*)"if", KEY);
    addToSymbolTable((char*)"else", KEY);
    addToSymbolTable((char*)"then", KEY);
    addToSymbolTable((char*)"while", KEY);
    addToSymbolTable((char*)"for", KEY);
    addToSymbolTable((char*)"do", KEY);
    addToSymbolTable((char*)"break", KEY);
    addToSymbolTable((char*)"true", KEY);
    addToSymbolTable((char*)"false", KEY);
	addToSymbolTable((char*)"num", KEY);
	addToSymbolTable((char*)"int", KEY);
	addToSymbolTable((char*)"real", KEY);
	addToSymbolTable((char*)"bool", KEY);
	addToSymbolTable((char*)"or", KEY);
	addToSymbolTable((char*)"and", KEY);
    // ��������
//    addToSymbolTable("+", OPERATOR);
//    addToSymbolTable("-", OPERATOR);
//    addToSymbolTable("*", OPERATOR);
//    addToSymbolTable("/", OPERATOR);
//    addToSymbolTable("!", OPERATOR);
//    addToSymbolTable("&&", OPERATOR);
//    addToSymbolTable("||", OPERATOR);
//    addToSymbolTable("==", OPERATOR);
//    addToSymbolTable("!=", OPERATOR);
//    addToSymbolTable("<", OPERATOR);
//    addToSymbolTable("<=", OPERATOR);
//    addToSymbolTable(">", OPERATOR);
//    addToSymbolTable(">=", OPERATOR);
//    addToSymbolTable("=", OPERATOR);

    // ��ӷָ���
//    addToSymbolTable(";", DELIMITER);
//    addToSymbolTable("(", DELIMITER);
//    addToSymbolTable(")", DELIMITER);
//    addToSymbolTable("{", DELIMITER);
//    addToSymbolTable("}", DELIMITER);
//    addToSymbolTable("[", DELIMITER);
//    addToSymbolTable("]", DELIMITER);
//    addToSymbolTable(",", DELIMITER);
}



typedef enum {
    // �ؼ���
    IF, ELSE, WHILE,FOR, DO, BREAK, TRUE, FALSE,
    // ��ʶ���ͳ���
    ID, NUM, REAL, STRING,BASIC,
    // ������ͷָ���
    PLUS, MINUS,TIMES, MULTIPLY, DIVIDE, NOT, LOGICAL_AND, LOGICAL_OR, EQUAL, NOTEQUAL,
    LESS, LESSEQUAL, GREATER, GREATEREQUAL, ASSIGN, SEMICOLON,
    LPAREN, RPAREN, LBRACE, RBRACE, LBRACKET, RBRACKET,AND,OR,
    // ����
    ERROR, ENDFILE, END 
} TokenType;

typedef struct {
    TokenType type;
    union {
        char *string;
        int integer;
        double real;
        char c;
    } value;
    int code;
    int codevalue;
} Token;

Token currentToken;
FILE *inputFile;
Token *tokenTable;  // Token ��
int tokenCount = 0;  // Token ���� Token ������


void initTokenTable(){
	int initialCapacity = 10; // ��ʼ����
	tokenTable = (Token*)malloc(initialCapacity * sizeof(Token));
	tokenCount = 0;
} 

void addTokenToTable(Token token) {
    tokenTable = (Token*)realloc(tokenTable, (tokenCount + 1) * sizeof(Token));
    tokenTable[tokenCount] = token;
    tokenCount++;
}

void getToken() {				
    	//getToken()������Ч���ţ���ȡ����Token�������뵽���� 
	int c;
    								//������Ч����(continue)����ȡ���ź�return 
    while ((c = fgetc(inputFile)) != EOF) {
        if (isspace(c)) {
            continue;  				// �����ո�Ϳհ׷���
        } else if (c == '/') {
            						// ����Ƿ�Ϊע��
            if ((c = fgetc(inputFile)) == '/') {
                					// ����ע�ͣ���������
                while ((c = fgetc(inputFile)) != '\n' && c != EOF) {}
            }else if ((c = fgetc(inputFile)) == '*') {
                // ����ע��
                int prev_c = 0,line=0;
        			while (1) {
            			c = fgetc(inputFile);
            			if (c == EOF) {
                			error("Unexpected end of file in comment.");
                				break;
            			} else if (prev_c == '*' && c == '/') {
                			break;
            			} else if (c == '\n') {
                			line++;
            }
            prev_c = c;
        }
            }else {
                ungetc(c, inputFile);  // ����ע�ͣ��Ż� 
                break;
            }
        } else if (isalpha(c)) {
            							// �ؼ��ֻ�ʶ�����
            
 // �ؼ��ֻ�ʶ�����
    char buffer[256];
    int length = 0;
    buffer[length++] = (char) c;
    while (isalnum(c = fgetc(inputFile))) {
        buffer[length++] = (char) c;
    }
    buffer[length] = '\0';
    ungetc(c, inputFile);

    // �ڷ��ű��в��ҹؼ��ֻ��ʶ��
    SymbolTableEntry* entry = lookup(buffer);
if (strcmp(buffer, "if") == 0) {
                currentToken.type = IF;
            } else if (strcmp(buffer, "else") == 0) {
                currentToken.type = ELSE;
            } else if (strcmp(buffer, "for") == 0) {
                currentToken.type = FOR;
			} else if (strcmp(buffer, "while") == 0) {
                currentToken.type = WHILE;
            } else if (strcmp(buffer, "do") == 0) {
                currentToken.type = DO;
            } else if (strcmp(buffer, "break") == 0) {
                currentToken.type = BREAK;
            } else if (strcmp(buffer, "true") == 0) {
                currentToken.type = TRUE;
            } else if (strcmp(buffer, "false") == 0) {
                currentToken.type = FALSE;
            } else if (strcmp(buffer, "int") == 0) {
                currentToken.type = BASIC;
            } else {
                currentToken.type = ID;
                currentToken.value.string = strdup(buffer);
            }
    
    currentToken.value.string = strdup(buffer);
    addTokenToTable(currentToken);  // �� Token ���� Token ��
    return;
        } else if (isdigit(c)) {
            // ������ʵ������
            char buffer[256];
            int length = 0;
            buffer[length++] = (char) c;
            while (isdigit(c = fgetc(inputFile))) {
                buffer[length++] = (char) c;
            }
            if (c == '.') {
                buffer[length++] = '.';  // ��¼С����
                while (isdigit(c = fgetc(inputFile))) {
                    buffer[length++] = (char) c;
                }
                buffer[length] = '\0';
                currentToken.type = REAL;
                currentToken.value.real = atof(buffer);
            } else {
                buffer[length] = '\0';
                currentToken.type = NUM;
                currentToken.value.integer = atoi(buffer);
            }
            ungetc(c, inputFile);
            addTokenToTable(currentToken);  // �� Token ���� Token ��
            return;
        } else if (c == '"') {
            // �ַ�������
            char buffer[256];
            int length = 0;
            while ((c = fgetc(inputFile)) != '"') {
                if (c == EOF) {
                    error("Unexpected end of file inside string constant.");
                }
                buffer[length++] = (char) c;
            }
            buffer[length] = '\0';
            currentToken.type = STRING;
            currentToken.value.string = strdup(buffer);
            addTokenToTable(currentToken);  // �� Token ���� Token ��
            return;
        } else if (c == '=') {
            // �ȺŻ���������
            if ((c = fgetc(inputFile)) == '=') {
                currentToken.type = EQUAL;
            } else {
                ungetc(c, inputFile);
                currentToken.type = ASSIGN;
            }
            currentToken.value.c=c;
            
            addTokenToTable(currentToken);  // �� Token ���� Token ��
            return;
        } else if (c == '!') {
            // �����������
            if ((c = fgetc(inputFile)) == '=') {
                currentToken.type = NOTEQUAL;
            } else {
                ungetc(c, inputFile);
                currentToken.type = NOT;
            }
            currentToken.value.c=c;
            
            addTokenToTable(currentToken);  // �� Token ���� Token ��
            return;
        } else if (c == '<') {
            // С�ڻ�С�ڵ��������
            if ((c = fgetc(inputFile)) == '=') {
                currentToken.type = LESSEQUAL;
            } else {
                ungetc(c, inputFile);
                currentToken.type = LESS;
            }
            currentToken.value.c=c;
            
            addTokenToTable(currentToken);  // �� Token ���� Token ��
            return;
        } else if (c == '>') {
            // ���ڻ���ڵ��������
            if ((c = fgetc(inputFile)) == '=') {
                currentToken.type = GREATEREQUAL;
            } else {
                ungetc(c, inputFile);
                currentToken.type = GREATER;
            }
            currentToken.value.c=c;
            
            addTokenToTable(currentToken);  // �� Token ���� Token ��
            return;
        } else if (c == '&') {
            // �߼��������
            if ((c = fgetc(inputFile)) == '&') {
                currentToken.type =LOGICAL_AND;
            } else {
                error("Invalid character '&'.");
            }
            currentToken.value.c=c;
            
            addTokenToTable(currentToken);  // �� Token ���� Token ��
            return;
        } else if (c == '|') {
            // �߼��������
            if ((c = fgetc(inputFile)) == '|') {
                currentToken.type = LOGICAL_OR;
            } else {
                error("Invalid character '|'.");
            }
            currentToken.value.c=c;
            addTokenToTable(currentToken);  // �� Token ���� Token ��
            return;
        } else if (c == '+') {
            // �ӷ������
            currentToken.value.c=c;
            
            currentToken.type = PLUS;
            addTokenToTable(currentToken);  // �� Token ���� Token ��
            return;
        } else if (c == '-') {
            // ���������
            currentToken.value.c=c;
            
            currentToken.type = MINUS;
            addTokenToTable(currentToken);  // �� Token ���� Token ��
            return;
        } else if (c == '*') {
            // �˷������
            currentToken.value.c=c;
            
            currentToken.type = MULTIPLY;
            addTokenToTable(currentToken);  // �� Token ���� Token ��
            return;
        } else if (c == '/') {
            // ���������
            currentToken.value.c=c;
            
            currentToken.type = DIVIDE;
            addTokenToTable(currentToken);  // �� Token ���� Token ��
            return;
        } else if (c == '(') {
            // ������
            currentToken.value.c=c;
            
            currentToken.type = LPAREN;
            addTokenToTable(currentToken);  // �� Token ���� Token ��
            return;
        } else if (c == ')') {
            // ������
            currentToken.value.c=c;
            
            currentToken.type = RPAREN;
            addTokenToTable(currentToken);  // �� Token ���� Token ��
            return;
          } else if (c == '{') {
            currentToken.value.c=c;
            
            currentToken.type = LBRACE;
            addTokenToTable(currentToken);  // �� Token ���� Token ��
            return;
        }else if (c == '}') {
            currentToken.value.c=c;
            
            currentToken.type = RBRACE;
            addTokenToTable(currentToken);  // �� Token ���� Token ��
            return;
        }else if (c == ';') {
            currentToken.value.c=c;
            
            currentToken.type = SEMICOLON;
            addTokenToTable(currentToken);  // �� Token ���� Token ��
            return;
        }else {
            error("Invalid character");
        }
    }
    // �����ļ�ĩβ
    currentToken.type = END;
    addTokenToTable(currentToken);  // �� Token ���� Token ��
}
std::string returntoken(Token token) {
    switch (token.type) {
        case BASIC:
            return(token.value.string);
            break;
		case IF:
            return("<if>");
            break;
        case ELSE:
           return(token.value.string);
            break;
        case FOR:
            return(token.value.string);
            break;
		case WHILE:
            return(token.value.string);
            break;
        case DO:
           return(token.value.string);
            break;
        case BREAK:
            return(token.value.string);
        case TRUE:
            return(token.value.string);
        case FALSE:
            return(token.value.string);
        case ID:
            return(token.value.string);
            break;
        case NUM:
           return("<num>");

        case REAL:
           return("<num>");
        case PLUS:
            return("<+>");
          
        case MINUS:
             return("<->");
        
		case MULTIPLY:
            return("<*>");
        case DIVIDE:
            return("</>");
        case NOT:
            return("<not>");

        case AND:
            return("<&&, op>");
            break;
        case OR:
            return("<||, op>");
            break;
        case EQUAL:
            return("<==, relop>");
            break;
        case NOTEQUAL:
            return("<!=, relop>");
            break;
        case LESS:
            return("< <, relop>");
            break;
        case LESSEQUAL:
            return("< <=, relop>");
            break;
        case GREATER:
            return("< >, relop>");
            break;
        case GREATEREQUAL:
            return("< >=, relop>");
            break;
        case ASSIGN:
            return("<=>");
            break;
        case SEMICOLON:
            return("<;, ���>");
            break;
        case LPAREN:
            return("<(, ���>");
            break;
        case RPAREN:
            return("<), ���>");
            break;
        case LBRACE:
            return("<{, ���>");
            break;
        case RBRACE:
            return("<}, ���>");
            break;
        case LBRACKET:
            return("<[, ���>");
            break;
        case RBRACKET:
            return("<], ���>");
            break;
        case ERROR:
            return("<ERROR>\n");
            break;
        case ENDFILE:
            return("<EOF>\n");
            break;
    }
    return ("<ERROR>");
}

void printToken(Token token) {
    switch (token.type) {
        case BASIC:
            printf("<%s, key>\n", token.value.string);
            break;
		case IF:
            printf("<%s, key>\n", token.value.string);
            break;
        case ELSE:
            printf("<%s, key>\n", token.value.string);
            break;
        case FOR:
            printf("<%s, key>\n", token.value.string);
            break;
		case WHILE:
            printf("<%s, key>\n", token.value.string);
            break;
        case DO:
            printf("<%s, key>\n", token.value.string);
            break;
        case BREAK:
            printf("<%s, key>\n", token.value.string);
            break;
        case TRUE:
            printf("<%s, key>\n", token.value.string);
            break;
        case FALSE:
            printf("<%s, key>\n", token.value.string);
            break;
        case ID:
            printf("<%s, id>\n", token.value.string);
            break;
        case NUM:
            printf("<%d, num>\n", token.value.integer);
            break;
        case REAL:
            printf("<%f, num>\n", token.value.real);
            break;
        case PLUS:
            
            printf("<%c, op>\n", token.value.c);
            break;
        case MINUS:
            printf("<%c, op>\n", token.value.c);
            break;
        
		case MULTIPLY:
            printf("<%c, op>\n", token.value.c);
            break;
        case DIVIDE:
            printf("<%c, op>\n", token.value.c);
            break;
        case NOT:
            printf("<!, op>\n");
            break;
        case AND:
            printf("<&&, op>\n");
            break;
        case OR:
            printf("<||, op>\n");
            break;
        case EQUAL:
            printf("<==, relop>\n");
            break;
        case NOTEQUAL:
            printf("<!=, relop>\n");
            break;
        case LESS:
            printf("< <, relop>\n");
            break;
        case LESSEQUAL:
            printf("< <=, relop>\n");
            break;
        case GREATER:
            printf("< >, relop>\n");
            break;
        case GREATEREQUAL:
            printf("< >=, relop>\n");
            break;
        case ASSIGN:
            printf("<=, ��ֵ>\n");
            break;
        case SEMICOLON:
            printf("<;, ���>\n");
            break;
        case LPAREN:
            printf("<(, ���>\n");
            break;
        case RPAREN:
            printf("<), ���>\n");
            break;
        case LBRACE:
            printf("<{, ���>\n");
            break;
        case RBRACE:
            printf("<}, ���>\n");
            break;
        case LBRACKET:
            printf("<[, ���>\n");
            break;
        case RBRACKET:
            printf("<], ���>\n");
            break;
        case ERROR:
            printf("<ERROR>\n");
            break;
        case ENDFILE:
            printf("<EOF>\n");
            break;
    }
}
void token2int(Token token) {
    switch (token.type) {
        case BASIC:
            printf("<%s, key>\n", token.value.string);
            break;
		case IF:
            printf("<%s, key>\n", token.value.string);
            break;
        case ELSE:
            printf("<%s, key>\n", token.value.string);
            break;
        case FOR:
            printf("<%s, key>\n", token.value.string);
            break;
		case WHILE:
            printf("<%s, key>\n", token.value.string);
            break;
        case DO:
            printf("<%s, key>\n", token.value.string);
            break;
        case BREAK:
            printf("<%s, key>\n", token.value.string);
            break;
        case TRUE:
            printf("<%s, key>\n", token.value.string);
            break;
        case FALSE:
            printf("<%s, key>\n", token.value.string);
            break;
        case ID:
            printf("<%s, id>\n", token.value.string);
            break;
        case NUM:
            printf("<%d, num>\n", token.value.integer);
            break;
        case REAL:
            printf("<%f, num>\n", token.value.real);
            break;
        case PLUS:
            
            printf("<%c, op>\n", token.value.c);
            break;
        case MINUS:
            printf("<%c, op>\n", token.value.c);
            break;
        
		case MULTIPLY:
            printf("<%c, op>\n", token.value.c);
            break;
        case DIVIDE:
            printf("<%c, op>\n", token.value.c);
            break;
        case NOT:
            printf("<!, op>\n");
            break;
        case AND:
            printf("<&&, op>\n");
            break;
        case OR:
            printf("<||, op>\n");
            break;
        case EQUAL:
            printf("<==, relop>\n");
            break;
        case NOTEQUAL:
            printf("<!=, relop>\n");
            break;
        case LESS:
            printf("< <, relop>\n");
            break;
        case LESSEQUAL:
            printf("< <=, relop>\n");
            break;
        case GREATER:
            printf("< >, relop>\n");
            break;
        case GREATEREQUAL:
            printf("< >=, relop>\n");
            break;
        case ASSIGN:
            printf("<=, ��ֵ>\n");
            break;
        case SEMICOLON:
            printf("<;, ���>\n");
            break;
        case LPAREN:
            printf("<(, ���>\n");
            break;
        case RPAREN:
            printf("<), ���>\n");
            break;
        case LBRACE:
            printf("<{, ���>\n");
            break;
        case RBRACE:
            printf("<}, ���>\n");
            break;
        case LBRACKET:
            printf("<[, ���>\n");
            break;
        case RBRACKET:
            printf("<], ���>\n");
            break;
        case ERROR:
            printf("<ERROR>\n");
            break;
        case ENDFILE:
            printf("<EOF>\n");
            break;
    }
}
Token NextToken()
{
    getToken();

    
    return currentToken;
}
//int main() {
//    initializeSymbolTable();
//	inputFile = fopen("input.txt", "r");
//    if (inputFile == NULL) {
//        error("Cannot open input file.");
//    }
//
//    getToken();
//    while (currentToken.type != ENDFILE) {
//        printToken(currentToken);
//        getToken();
//    }
//
//    fclose(inputFile);
//
//    return 0;
//}
