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
    KEY,    // 关键字
    IDENTIFIER, // 标识符
    CONSTANT,   // 常数
    OPERATOR,   // 运算符
    DELIMITER   // 分隔符
} SymbolType;

typedef struct {
    SymbolType type;
    char lexeme[MAX_LEXEME_LENGTH];  // 用于存储标识符或常数的字符串
    int value;
    
} Symbol;

// 符号表项
typedef struct {
    char lexeme[MAX_LEXEME_LENGTH];  // 标识符的名称
    SymbolType type;                  // 标识符的类型
} SymbolTableEntry;

// 符号表

SymbolTableEntry symbolTable[MAX_SYMBOLS];
int symbolTableSize = 0;

// 向符号表添加条目
void addToSymbolTable(char* lexeme, SymbolType type) {
    if (symbolTableSize >= MAX_SYMBOLS) {
        error("Symbol table is full.");
    }

    strcpy(symbolTable[symbolTableSize].lexeme, lexeme);
    symbolTable[symbolTableSize].type = type;
    symbolTableSize++;
}

// 在符号表中查找条目

//-std=c11 or -std=gnu11 to compile your code
SymbolTableEntry* lookup(char* lexeme) {
	int i;
    for (i = 0; i < MAX_SYMBOLS; i++) {
        if (strcmp(symbolTable[i].lexeme, lexeme) == 0) {
            return &symbolTable[i];
        }
    }
    return NULL;  // 未找到匹配的条目
}

// 初始化符号表
void initializeSymbolTable() {
    // 添加基本保留字
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
    // 添加运算符
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

    // 添加分隔符
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
    // 关键字
    IF, ELSE, WHILE,FOR, DO, BREAK, TRUE, FALSE,
    // 标识符和常量
    ID, NUM, REAL, STRING,BASIC,
    // 运算符和分隔符
    PLUS, MINUS,TIMES, MULTIPLY, DIVIDE, NOT, LOGICAL_AND, LOGICAL_OR, EQUAL, NOTEQUAL,
    LESS, LESSEQUAL, GREATER, GREATEREQUAL, ASSIGN, SEMICOLON,
    LPAREN, RPAREN, LBRACE, RBRACE, LBRACKET, RBRACKET,AND,OR,
    // 其他
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
Token *tokenTable;  // Token 表
int tokenCount = 0;  // Token 表中 Token 的数量


void initTokenTable(){
	int initialCapacity = 10; // 初始容量
	tokenTable = (Token*)malloc(initialCapacity * sizeof(Token));
	tokenCount = 0;
} 

void addTokenToTable(Token token) {
    tokenTable = (Token*)realloc(tokenTable, (tokenCount + 1) * sizeof(Token));
    tokenTable[tokenCount] = token;
    tokenCount++;
}

void getToken() {				
    	//getToken()跳过无效符号，获取单个Token，并加入到表中 
	int c;
    								//跳过无效符号(continue)，获取符号后return 
    while ((c = fgetc(inputFile)) != EOF) {
        if (isspace(c)) {
            continue;  				// 跳过空格和空白符号
        } else if (c == '/') {
            						// 检查是否为注释
            if ((c = fgetc(inputFile)) == '/') {
                					// 单行注释，忽略整行
                while ((c = fgetc(inputFile)) != '\n' && c != EOF) {}
            }else if ((c = fgetc(inputFile)) == '*') {
                // 多行注释
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
                ungetc(c, inputFile);  // 不是注释，放回 
                break;
            }
        } else if (isalpha(c)) {
            							// 关键字或识别符号
            
 // 关键字或识别符号
    char buffer[256];
    int length = 0;
    buffer[length++] = (char) c;
    while (isalnum(c = fgetc(inputFile))) {
        buffer[length++] = (char) c;
    }
    buffer[length] = '\0';
    ungetc(c, inputFile);

    // 在符号表中查找关键字或标识符
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
    addTokenToTable(currentToken);  // 将 Token 加入 Token 表
    return;
        } else if (isdigit(c)) {
            // 整数或实数常量
            char buffer[256];
            int length = 0;
            buffer[length++] = (char) c;
            while (isdigit(c = fgetc(inputFile))) {
                buffer[length++] = (char) c;
            }
            if (c == '.') {
                buffer[length++] = '.';  // 记录小数点
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
            addTokenToTable(currentToken);  // 将 Token 加入 Token 表
            return;
        } else if (c == '"') {
            // 字符串常量
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
            addTokenToTable(currentToken);  // 将 Token 加入 Token 表
            return;
        } else if (c == '=') {
            // 等号或等于运算符
            if ((c = fgetc(inputFile)) == '=') {
                currentToken.type = EQUAL;
            } else {
                ungetc(c, inputFile);
                currentToken.type = ASSIGN;
            }
            currentToken.value.c=c;
            
            addTokenToTable(currentToken);  // 将 Token 加入 Token 表
            return;
        } else if (c == '!') {
            // 不等于运算符
            if ((c = fgetc(inputFile)) == '=') {
                currentToken.type = NOTEQUAL;
            } else {
                ungetc(c, inputFile);
                currentToken.type = NOT;
            }
            currentToken.value.c=c;
            
            addTokenToTable(currentToken);  // 将 Token 加入 Token 表
            return;
        } else if (c == '<') {
            // 小于或小于等于运算符
            if ((c = fgetc(inputFile)) == '=') {
                currentToken.type = LESSEQUAL;
            } else {
                ungetc(c, inputFile);
                currentToken.type = LESS;
            }
            currentToken.value.c=c;
            
            addTokenToTable(currentToken);  // 将 Token 加入 Token 表
            return;
        } else if (c == '>') {
            // 大于或大于等于运算符
            if ((c = fgetc(inputFile)) == '=') {
                currentToken.type = GREATEREQUAL;
            } else {
                ungetc(c, inputFile);
                currentToken.type = GREATER;
            }
            currentToken.value.c=c;
            
            addTokenToTable(currentToken);  // 将 Token 加入 Token 表
            return;
        } else if (c == '&') {
            // 逻辑与运算符
            if ((c = fgetc(inputFile)) == '&') {
                currentToken.type =LOGICAL_AND;
            } else {
                error("Invalid character '&'.");
            }
            currentToken.value.c=c;
            
            addTokenToTable(currentToken);  // 将 Token 加入 Token 表
            return;
        } else if (c == '|') {
            // 逻辑或运算符
            if ((c = fgetc(inputFile)) == '|') {
                currentToken.type = LOGICAL_OR;
            } else {
                error("Invalid character '|'.");
            }
            currentToken.value.c=c;
            addTokenToTable(currentToken);  // 将 Token 加入 Token 表
            return;
        } else if (c == '+') {
            // 加法运算符
            currentToken.value.c=c;
            
            currentToken.type = PLUS;
            addTokenToTable(currentToken);  // 将 Token 加入 Token 表
            return;
        } else if (c == '-') {
            // 减法运算符
            currentToken.value.c=c;
            
            currentToken.type = MINUS;
            addTokenToTable(currentToken);  // 将 Token 加入 Token 表
            return;
        } else if (c == '*') {
            // 乘法运算符
            currentToken.value.c=c;
            
            currentToken.type = MULTIPLY;
            addTokenToTable(currentToken);  // 将 Token 加入 Token 表
            return;
        } else if (c == '/') {
            // 除法运算符
            currentToken.value.c=c;
            
            currentToken.type = DIVIDE;
            addTokenToTable(currentToken);  // 将 Token 加入 Token 表
            return;
        } else if (c == '(') {
            // 左括号
            currentToken.value.c=c;
            
            currentToken.type = LPAREN;
            addTokenToTable(currentToken);  // 将 Token 加入 Token 表
            return;
        } else if (c == ')') {
            // 右括号
            currentToken.value.c=c;
            
            currentToken.type = RPAREN;
            addTokenToTable(currentToken);  // 将 Token 加入 Token 表
            return;
          } else if (c == '{') {
            currentToken.value.c=c;
            
            currentToken.type = LBRACE;
            addTokenToTable(currentToken);  // 将 Token 加入 Token 表
            return;
        }else if (c == '}') {
            currentToken.value.c=c;
            
            currentToken.type = RBRACE;
            addTokenToTable(currentToken);  // 将 Token 加入 Token 表
            return;
        }else if (c == ';') {
            currentToken.value.c=c;
            
            currentToken.type = SEMICOLON;
            addTokenToTable(currentToken);  // 将 Token 加入 Token 表
            return;
        }else {
            error("Invalid character");
        }
    }
    // 到达文件末尾
    currentToken.type = END;
    addTokenToTable(currentToken);  // 将 Token 加入 Token 表
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
            return("<;, 界符>");
            break;
        case LPAREN:
            return("<(, 界符>");
            break;
        case RPAREN:
            return("<), 界符>");
            break;
        case LBRACE:
            return("<{, 界符>");
            break;
        case RBRACE:
            return("<}, 界符>");
            break;
        case LBRACKET:
            return("<[, 界符>");
            break;
        case RBRACKET:
            return("<], 界符>");
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
            printf("<=, 赋值>\n");
            break;
        case SEMICOLON:
            printf("<;, 界符>\n");
            break;
        case LPAREN:
            printf("<(, 界符>\n");
            break;
        case RPAREN:
            printf("<), 界符>\n");
            break;
        case LBRACE:
            printf("<{, 界符>\n");
            break;
        case RBRACE:
            printf("<}, 界符>\n");
            break;
        case LBRACKET:
            printf("<[, 界符>\n");
            break;
        case RBRACKET:
            printf("<], 界符>\n");
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
            printf("<=, 赋值>\n");
            break;
        case SEMICOLON:
            printf("<;, 界符>\n");
            break;
        case LPAREN:
            printf("<(, 界符>\n");
            break;
        case RPAREN:
            printf("<), 界符>\n");
            break;
        case LBRACE:
            printf("<{, 界符>\n");
            break;
        case RBRACE:
            printf("<}, 界符>\n");
            break;
        case LBRACKET:
            printf("<[, 界符>\n");
            break;
        case RBRACKET:
            printf("<], 界符>\n");
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
