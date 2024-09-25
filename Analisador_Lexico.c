#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

typedef enum {
    TOKEN_KEYWORD,
    TOKEN_IDENTIFIER,
    TOKEN_NUMBER,
    TOKEN_SYMBOL,
    TOKEN_UNKNOWN,
    TOKEN_EOF
} TokenType;

typedef struct {
    TokenType type;
    char lexeme[256];
} Token;

const char* keywords[] = {"begin", "end", "if", "then", "else", "while", "do", "var", "procedure", "function"};

int isKeyword(const char *str) {
    for (int i = 0; i < sizeof(keywords) / sizeof(keywords[0]); i++) {
        if (strcmp(str, keywords[i]) == 0) {
            return 1;
        }
    }
    return 0;
}

char getNextChar(FILE *file) {
    return fgetc(file);
}

void ungetChar(char c, FILE *file) {
    ungetc(c, file);
}

Token getNextToken(FILE *file) {
    Token token;
    char c;
    int i = 0;

    do {
        c = getNextChar(file);
        if (c == EOF) {
            token.type = TOKEN_EOF;
            return token;
        }
    } while (isspace(c));

    if (isalpha(c)) {
        token.lexeme[i++] = c;
        while (isalnum((c = getNextChar(file)))) {
            token.lexeme[i++] = c;
        }
        ungetChar(c, file);
        token.lexeme[i] = '\0';

        if (isKeyword(token.lexeme)) {
            token.type = TOKEN_KEYWORD;
        } else {
            token.type = TOKEN_IDENTIFIER;
        }
        return token;
    }

    if (isdigit(c)) {
        token.lexeme[i++] = c;
        while (isdigit((c = getNextChar(file)))) {
            token.lexeme[i++] = c;
        }
        ungetChar(c, file);
        token.lexeme[i] = '\0';
        token.type = TOKEN_NUMBER;
        return token;
    }

    if (ispunct(c)) {
        token.lexeme[i++] = c;
        token.lexeme[i] = '\0';
        token.type = TOKEN_SYMBOL;
        return token;
    }

    token.lexeme[0] = c;
    token.lexeme[1] = '\0';
    token.type = TOKEN_UNKNOWN;
    return token;
}

int main() {
    FILE *file = fopen("codigo.pas", "r");
    if (!file) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    Token token;
    do {
        token = getNextToken(file);
        switch (token.type) {
            case TOKEN_KEYWORD:
                printf("Palavra reservada: %s\n", token.lexeme);
                break;
            case TOKEN_IDENTIFIER:
                printf("Identificador: %s\n", token.lexeme);
                break;
            case TOKEN_NUMBER:
                printf("Numero: %s\n", token.lexeme);
                break;
            case TOKEN_SYMBOL:
                printf("Simbolo: %s\n", token.lexeme);
                break;
            case TOKEN_UNKNOWN:
                printf("Token desconhecido: %s\n", token.lexeme);
                break;
            case TOKEN_EOF:
                printf("Fim do arquivo.\n");
                break;
        }
    } while (token.type != TOKEN_EOF);

    fclose(file);
    return 0;
}
