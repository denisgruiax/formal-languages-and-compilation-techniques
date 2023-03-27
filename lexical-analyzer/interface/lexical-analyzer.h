typedef enum
{
    COMMA,
    SEMICOLON,
    LPAR,
    RPAR,
    LBRACKET,
    RBRACKET,
    LACC,
    RACC,
    ADD,
    SUB,
    MUL,
    DIV,
    DOT,
    AND,
    OR,
    NOT,
    NOTEQ,
    ASSIGN,
    EQUAL,
    LESS,
    LESSEQ,
    GREATER,
    GREATEREQ,
    ID,
    CT_CHAR,
    CT_INT,
    CT_REAL,
    CT_STRING,
    END,
    BREAK,
    CHAR,
    DOUBLE,
    ELSE,
    FOR,
    IF,
    INT,
    RETURN,
    STRUCT,
    VOID,
    WHILE
} TokenType;

typedef struct
{
    int code;
    union
    {
        char *text;
        long integer;
        double real;
    };
    int line;
    struct Token *next;

} Token;

void readFile(char *path);
Token *createToken(int code);
Token *addToken(int code);
void errorMessage(const char *fmt, ...);
void tokenError(const Token *tokens, const char *fmt, ...);
char *createString(const char *start, const char *end);
int getNextToken();
void showAtoms();