#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdarg.h>

#define SAFEALLOC(var, Type)                          \
    if ((var = (Type *)malloc(sizeof(Type))) == NULL) \
        errorMessage("not enough memory");

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

/*Variabile globale*/
Token *tokens, *lastToken;
char *pCrtCh, *text, *textContainer, text24[100];
int line = 1, onOff = 1;
unsigned counter, numberOfCharacters;

Token *createToken(int code);
Token *addToken(int code);
void errorMessage(const char *fmt, ...);
void tokenError(const Token *tokens, const char *fmt, ...);
char *createString(const char *start, const char *end);
int getNextToken();
void showAtoms(Token *tokens);

int main()
{
    FILE *fileSource;
    char inBuffer[30000];

    if ((fileSource = fopen("test.c", "r")) == NULL)
    {
        printf("eroare deschidere fisier");
        return EXIT_FAILURE;
    }

    int n = fread(inBuffer, 1, 30000, fileSource);
    inBuffer[n] = '\0';

    fclose(fileSource);
    pCrtCh = inBuffer;

    do
    {
        getNextToken();
    } while (onOff);

    printf("showTokens:\n");
    showAtoms(tokens);
    free(tokens);
    free(textContainer);
    return 0;
}

Token *createToken(int code)
{
    Token *token;
    token = (Token *)malloc(sizeof(Token));

    token->code = code;
    token->line = line;
    token->next = NULL;

    if ((code == ID) || (code == CT_STRING))
    {
        token->text = malloc(strlen(textContainer) * sizeof(char));
        strcpy(token->text, textContainer);
    }
    else if (code == CT_CHAR)
    {
        if (textContainer[0] == '\\')
        {
            if (textContainer[1] == 'a')
                token->integer = '\a';
            else if (textContainer[1] == 'b')
                token->integer = '\b';
            else if (textContainer[1] == 'e')
                token->integer = '\e';
            else if (textContainer[1] == 'f')
                token->integer = '\f';
            else if (textContainer[1] == 'n')
                token->integer = '\n';
            else if (textContainer[1] == 'r')
                token->integer = '\r';
            else if (textContainer[1] == 't')
                token->integer = '\t';
            else if (textContainer[1] == 'v')
                token->integer = '\v';
            else if (textContainer[1] == '\\')
                token->integer = '\\';
            else if (textContainer[1] == '\'')
                token->integer = '\'';
            else if (textContainer[1] == '"')
                token->integer = '"';
        }
        else
            token->integer = textContainer[0];
    }
    else if (code == CT_INT)
    {
        if ((textContainer[0] == '0') && (textContainer[1] == 'x'))
        {
            char *junk;
            token->integer = strtol(textContainer + 2, &junk, 16);
        }
        else
            token->integer = atol(textContainer);
    }
    else if (code == CT_REAL)
        token->real = atof(textContainer);

    lastToken = token;
    return token;
}

Token *addToken(int code)
{
    Token *localToken;

    localToken = createToken(code);
    if (tokens == NULL)
        tokens = localToken;
    else
    {
        Token *head = tokens;
        while (head->next != NULL)
        {
            head = head->next;
        }
        head->next = localToken;
    }

    return tokens;
}

void errorMessage(const char *fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    fprintf(stderr, "error: ");
    vprintf(stderr, va);
    fputc('\n', stderr);
    va_end(va);
    exit(-1);
}

void tokenError(const Token *tokens, const char *fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    fprintf(stderr, "error in line %d: ", tokens->line);
    vfprintf(stderr, fmt, va);
    fputc('\n', stderr);
    va_end(va);
    exit(-1);
}

char *createString(const char *start, const char *end)
{
    int size = end - start + 1;

    if (size > 2)
        end--;

    text = (char *)malloc(size + 1);

    char *index = text;

    while (start <= end)
        *(text++) = *(start++);

    *text = '\0';
    return index;
}

int getNextToken()
{
    int state = 0;
    char character;
    const char *pStartCh;

    while (onOff)
    {
        character = *pCrtCh;
        //printf("%c", character);
        switch (state)
        {
        case 0:
            if (character == ',')
            {
                pCrtCh++;
                state = 1;
            }
            else if (character == ';')
            {
                pCrtCh++;
                state = 2;
            }
            else if (character == '(')
            {
                pCrtCh++;
                state = 3;
            }
            else if (character == ')')
            {
                pCrtCh++;
                state = 4;
            }
            else if (character == '[')
            {
                pCrtCh++;
                state = 5;
            }
            else if (character == ']')
            {
                pCrtCh++;
                state = 6;
            }
            else if (character == '{')
            {
                pCrtCh++;
                state = 7;
            }
            else if (character == '}')
            {
                pCrtCh++;
                state = 8;
            }
            else if (character == '+')
            {
                pCrtCh++;
                state = 9;
            }
            else if (character == '-')
            {
                pCrtCh++;
                state = 10;
            }
            else if (character == '*')
            {
                pCrtCh++;
                state = 11;
            }
            else if (character == '/')
            {
                pCrtCh++;
                state = 32; /*modificare*/
            }
            else if (character == '.')
            {
                pCrtCh++;
                state = 13;
            }
            else if (character == '&')
            {
                pCrtCh++;
                state = 14;
            }
            else if (character == '|')
            {
                pCrtCh++;
                state = 16;
            }
            else if (character == '!')
            {
                pCrtCh++;
                state = 18;
            }
            else if (character == '=')
            {
                pCrtCh++;
                state = 21;
            }
            else if (character == '<')
            {
                pCrtCh++;
                state = 24;
            }
            else if (character == '>')
            {
                pCrtCh++;
                state = 27;
            }
            else if (isalpha(character) || (character == '_'))
            {
                pStartCh = pCrtCh;
                pCrtCh++;
                state = 30;
            }
            else if (character == '\'')
            {
                pCrtCh++;
                pStartCh = pCrtCh;
                state = 36;
            }
            else if (character == '"')
            {
                pCrtCh++;
                *text24 = NULL;
                pStartCh = pCrtCh;
                state = 40;
            }
            else if (character == '0')
            {
                pStartCh = pCrtCh;
                pCrtCh++;
                state = 44;
            }
            else if ((character >= '1') && (character <= '9'))
            {
                pStartCh = pCrtCh;
                pCrtCh++;
                state = 49;
            }
            else if (character == ' ' || character == '\r' || character == '\t')
                pCrtCh++;
            else if (character == '\n')
            {
                line++;
                pCrtCh++;
            }
            else if ((character == EOF) || (character == '\0'))
            {
                addToken(END);
                onOff = 0;
                return END;
            }
            else
                tokenError(addToken(END), "caracter invalid");
            break;

        case 1:
            addToken(COMMA);
            return COMMA;
            break;

        case 2:
            addToken(SEMICOLON);
            return SEMICOLON;
            break;

        case 3:
            addToken(LPAR);
            return LPAR;
            break;

        case 4:
            addToken(RPAR);
            return (RPAR);
            break;

        case 5:
            addToken(LBRACKET);
            return (LBRACKET);
            break;

        case 6:
            addToken(RBRACKET);
            return (RBRACKET);
            break;

        case 7:
            addToken(LACC);
            return (LACC);
            break;

        case 8:
            addToken(RACC);
            return (RACC);
            break;

        case 9:
            addToken(ADD);
            return (ADD);
            break;

        case 10:
            addToken(SUB);
            return (SUB);
            break;

        case 11:
            addToken(MUL);
            return (MUL);
            break;

        case 12:
            addToken(DIV);
            return (DIV);
            break;

        case 13:
            addToken(DOT);
            return (DOT);
            break;

        case 14:
            if (character == '&')
            {
                pCrtCh++;
                state = 15;
            }
            else
                tokenError(addToken(END), "caracter invalid");
            break;

        case 15:
            addToken(AND);
            return (AND);
            break;

        case 16:
            if (character == '|')
            {
                pCrtCh++;
                state = 17;
            }
            else
                tokenError(addToken(END), "caracter invalid");
            break;

        case 17:
            addToken(OR);
            return (OR);
            break;

        case 18:
            if (character == '=')
            {
                pCrtCh++;
                state = 20;
            }
            else
                state = 19;
            break;

        case 19:
            addToken(NOT);
            return (NOT);
            break;

        case 20:
            addToken(NOTEQ);
            return (NOTEQ);
            break;

        case 21:
            if (character == '=')
            {
                pCrtCh++;
                state = 23;
            }
            else
            {
                state = 22;
            }
            break;

        case 22:
            addToken(ASSIGN);
            return (ASSIGN);

        case 23:
            addToken(EQUAL);
            return (EQUAL);

        case 24:
            if (character == '=')
            {
                pCrtCh++;
                state = 26;
            }
            else
            {
                state = 25;
            }
            break;

        case 25:
            addToken(LESS);
            return (LESS);
            break;

        case 26:
            addToken(LESSEQ);
            return (LESSEQ);
            break;

        case 27:
            if (character == '=')
            {
                pCrtCh++;
                state = 29;
            }
            else
                state = 28;
            break;

        case 28:
            addToken(GREATER);
            return (GREATER);
            break;

        case 29:
            addToken(GREATEREQ);
            return (GREATEREQ);
            break;

        case 30:
            if (isalnum(character) || character == '_')
            {
                pCrtCh++;
            }
            else
                state = 31;
            break;

        case 31:
            numberOfCharacters = pCrtCh - pStartCh;
            if ((numberOfCharacters == 5) && !(memcmp(pStartCh, "break", 5)))
                tokens = addToken(BREAK);
            else if ((numberOfCharacters == 4) && !(memcmp(pStartCh, "char", 4)))
                tokens = addToken(CHAR);
            else if ((numberOfCharacters == 6) && !(memcmp(pStartCh, "double", 6)))
                tokens = addToken(DOUBLE);
            else if ((numberOfCharacters == 4) && !(memcmp(pStartCh, "else", 4)))
                tokens = addToken(ELSE);
            else if ((numberOfCharacters == 3) && !(memcmp(pStartCh, "for", 3)))
                tokens = addToken(FOR);
            else if ((numberOfCharacters == 2) && !(memcmp(pStartCh, "if", 2)))
                tokens = addToken(IF);
            else if ((numberOfCharacters == 3) && !(memcmp(pStartCh, "int", 3)))
                tokens = addToken(INT);
            else if ((numberOfCharacters == 6) && !(memcmp(pStartCh, "return", 3)))
                tokens = addToken(RETURN);
            else if ((numberOfCharacters == 6) && !(memcmp(pStartCh, "struct", 6)))
                tokens = addToken(STRUCT);
            else if ((numberOfCharacters == 4) && !(memcmp(pStartCh, "void", 4)))
                tokens = addToken(VOID);
            else if ((numberOfCharacters == 5) && !(memcmp(pStartCh, "while", 5)))
                tokens = addToken(WHILE);
            else
            {
                textContainer = createString(pStartCh, pCrtCh);
                tokens = addToken(ID);
            }

            return tokens->code;

            break;

        case 32:
            if (character == '/')
            {
                pCrtCh++;
                state = 33;
            }
            else if (character == '*')
            {
                pCrtCh++;
                state = 34;
            }
            else
            {
                pCrtCh++;
                state = 12;
            }
            break;

        case 33:
            if ((character != '\n') && (character != '\r') && (character != '\0'))
                pCrtCh++;
            else
            {
                if(character=='\n')
                    line++;
                pCrtCh++;
                state = 0;
            }
            break;

        case 34:
            if (character == '\n')
            {
                pCrtCh++;
                line++;
            }
            else if (character != '*')
                pCrtCh++;
            else if (character == '*')
            {
                pCrtCh++;
                state = 35;
            }
            break;

        case 35:
            if (character == '*')
                pCrtCh++;
            else if ((character != '*') && (character != '/'))
            {
                pCrtCh++;
                state = 34;
            }
            else if (character == '/')
            {
                pCrtCh++;
                state = 0;
            }
            break;

        case 36:
            if (character == '\\')
            {
                pCrtCh++;
                state = 37;
            }
            else if (character != '\'' && character != '\\')
            {
                textContainer = createString(pStartCh, pCrtCh);
                pCrtCh++;
                state = 38;
            }

        case 37:
            if ((character == 'a') || (character == 'b') || (character == 'f') ||
                (character == 'n') || (character == 'r') || (character == 't') || (character == 'v') ||
                (character == '\'') || (character == '?') || (character == '"') || (character == '\\') || (character == '0'))
            {

                textContainer = createString(pStartCh, pCrtCh);
                pCrtCh++;
                state = 38;
            }

            break;

        case 38:
            if (character == '\'')
            {
                pCrtCh++;
                state = 39;
            }
            break;

        case 39:
            addToken(CT_CHAR);
            return (CT_CHAR);
            break;

        case 40:
            if (character == '\\')
            {
                pCrtCh++;
                state = 41;
            }
            else if ((character != '"') && (character != '\\'))
            {
                strncat(text24, &character, 1);
                pCrtCh++;
                state = 42;
            }
            else
            {
                state = 42;
            }
            break;

        case 41:
            if (character == 'a')
            {
                char x = '\a';
                strncat(text24, &x, 1);
            }
            else if (character == 'b')
            {
                char x = '\b';
                strncat(text24, &x, 1);
            }
            else if (character == 'e')
            {
                char x = '\e';
                strncat(text24, &x, 1);
            }
            else if (character == 'f')
            {
                char x = '\f';
                strncat(text24, &x, 1);
            }
            else if (character == 'n')
            {
                char x = '\n';
                strncat(text24, &x, 1);
            }
            else if (character == 'r')
            {
                char x = '\r';
                strncat(text24, &x, 1);
            }
            else if (character == 't')
            {
                char x = '\t';
                strncat(text24, &x, 1);
            }
            else if (character == 'v')
            {
                char x = '\v';
                strncat(text24, &x, 1);
            }
            else if (character == '\\')
            {
                char x = '\\';
                strncat(text24, &x, 1);
            }
            else if (character == '\'')
            {
                char x = '\'';
                strncat(text24, &x, 1);
            }
            else if (character == '"')
            {
                char x = '"';
                strncat(text24, &x, 1);
            }
            pCrtCh++;
            state = 42;

            break;

        case 42:
            if (character == '"')
            {
                textContainer = malloc(strlen(text24));
                strcpy(textContainer, text24);
                pCrtCh++;
                state = 43;
            }
            else
            {
                state = 40;
            }
            break;

        case 43:
            addToken(CT_STRING);
            return (CT_STRING);
            break;

        case 44:
            if (character == 'x')
            {
                pCrtCh++;
                state = 45;
            }
            else
            {
                pCrtCh++;
                state = 48;
            }
            break;

        case 45:
            if (isxdigit(character))
            {
                pCrtCh++;
                state = 46;
            }
            break;

        case 46:
            if (isxdigit(character))
                pCrtCh++;
            else
            {
                state = 47;
            }
            break;

        case 47:
            textContainer = createString(pStartCh, pCrtCh);
            addToken(CT_INT);
            return CT_INT;
            break;

        case 48:
            if ((character >= '0') && (character <= '7'))
                pCrtCh++;
            else if (character == '.')
            {
                pCrtCh++;
                state = 50;
            }
            else if (character == 'e' || character == 'E')
            {
                pCrtCh++;
                state = 53;
            }
            else if (character == '8' || character == '9')
            {
                pCrtCh++;
                state = 52;
            }
            else
            {
                state = 47;
            }
            break;

        case 49:
            if (isdigit(character))
            {
                pCrtCh++;
            }
            else if (character == '.')
            {
                pCrtCh++;
                state = 50;
            }
            else if ((character == 'e') || (character == 'E'))
            {
                pCrtCh++;
                state = 53;
            }
            else
            {
                state = 47;
            }
            break;

        case 50:
            if (isdigit(character))
            {
                state = 51;
            }
            break;

        case 51:
            if (isdigit(character))
            {
                pCrtCh++;
            }
            else if ((character == 'e') || (character == 'E'))
            {
                pCrtCh++;
                state = 53;
            }
            else
                state = 56;
            break;

        case 52:
            if (isdigit(character))
                pCrtCh++;
            else if (character == '.')
            {
                pCrtCh++;
                state = 50;
            }
            else if ((character == 'e') || (character == 'E'))
            {
                pCrtCh++;
                state = 53;
            }
            else 
            {
                state = 47;
            }
            break;

        case 53:
            if (character == '+' || character == '-')
            {
                pCrtCh++;
                state = 54;
            }
            else
            {
                pCrtCh++;
                state = 54;
            }
            break;

        case 54:
            if (isdigit(character))
            {
                pCrtCh++;
                state = 55;
            }
            break;

        case 55:
            if (isdigit(character))
                pCrtCh++;
            else
                state = 56;
            break;

        case 56:
            textContainer = createString(pStartCh, pCrtCh);
            addToken(CT_REAL);
            return (CT_REAL);
            break;

        default:
            printf("Eroare...");
            break;
        }
    }
}

void showAtoms(Token *tokens)
{
    Token *head = tokens;
    while (head->next)
    {
        switch (head->code)
        {
        case COMMA:
            printf("%d COMMA\n", head->line);
            break;
        case SEMICOLON:
            printf("%d SEMICOLON\n", head->line);
            break;
        case LPAR:
            printf("%d LPAR\n", head->line);
            break;
        case RPAR:
            printf("%d RPAR\n", head->line);
            break;
        case LBRACKET:
            printf("%d LBRACKET\n", head->line);
            break;
        case RBRACKET:
            printf("%d RBRACKET\n", head->line);
            break;
        case LACC:
            printf("%d LACC\n", head->line);
            break;
        case RACC:
            printf("%d RACC\n", head->line);
            break;
        case ADD:
            printf("%d ADD\n", head->line);
            break;
        case SUB:
            printf("%d SUB\n", head->line);
            break;
        case MUL:
            printf("%d MUL\n", head->line);
            break;
        case DIV:
            printf("%d DIV\n", head->line);
            break;
        case DOT:
            printf("%d DOT\n", head->line);
            break;
        case AND:
            printf("%d AND\n", head->line);
            break;
        case OR:
            printf("%d OR\n", head->line);
            break;
        case NOT:
            printf("%d NOT\n", head->line);
            break;
        case NOTEQ:
            printf("%d NOTEQ\n", head->line);
            break;
        case ASSIGN:
            printf("%d ASSIGN\n", head->line);
            break;
        case EQUAL:
            printf("%d EQUAL\n", head->line);
            break;
        case LESS:
            printf("%d LESS\n", head->line);
            break;
        case LESSEQ:
            printf("%d LESSEQ\n", head->line);
            break;
        case GREATER:
            printf("%d GREATER\n", head->line);
            break;
        case GREATEREQ:
            printf("%d GREATEREQ\n", head->line);
            break;
        case ID:
            printf("%d ID:%s\n", head->line, head->text);
            break;
        case CT_CHAR:
            printf("%d CT_CHAR:%c\n", head->line, head->integer);
            break;
        case CT_INT:
            printf("%d CT_INT:%d\n", head->line, head->integer);
            break;
        case CT_REAL:
            printf("%d CT_REAL:%lf\n", head->line, head->real);
            break;
        case CT_STRING:
            printf("%d CT_STRING:%s\n", head->line, head->text);
            break;
        case END:
            printf("%d END\n", head->line);
            break;
        case BREAK:
            printf("%d BREAK\n", head->line);
            break;
        case CHAR:
            printf("%d CHAR\n", head->line);
            break;
        case DOUBLE:
            printf("%d DOUBLE\n", head->line);
            break;
        case ELSE:
            printf("%d ELSE\n", head->line);
            break;
        case FOR:
            printf("%d FOR\n", head->line);
            break;
        case IF:
            printf("%d IF\n", head->line);
            break;
        case INT:
            printf("%d INT\n", head->line);
            break;
        case RETURN:
            printf("%d RETURN\n", head->line);
            break;
        case STRUCT:
            printf("%d STRUCT\n", head->line);
            break;
        case VOID:
            printf("%d VOID\n", head->line);
            break;
        case WHILE:
            printf("%d WHILE\n", head->line);
            break;

        default:
            break;
        }

        head = head->next;
    }

    printf("%d END\n", head->line);
}