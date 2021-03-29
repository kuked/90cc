#include "90cc.h"

static bool is_alnum(char c);
static Token* new_token(TokenKind kind, Token* cur, char* str, int len);

Token* token;
LVar* locals;

void error_at(char* loc, char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);

    int pos = loc - user_input;
    fprintf(stderr, "%s\n", user_input);
    fprintf(stderr, "%*s", pos, " ");
    fprintf(stderr, "^ ");
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");

    exit(1);
}

void tokenize(void) {
    char* p = user_input;
    Token head;
    head.next = NULL;
    Token* cur = &head;
    locals = calloc(1, sizeof(LVar));

    while (*p) {
        if (isspace(*p)) {
            p++;
            continue;
        }

        if (memcmp(p, "==", 2) == 0 || memcmp(p, "!=", 2) == 0 || memcmp(p, "<=", 2) == 0 || memcmp(p, ">=", 2) == 0) {
            cur = new_token(TK_RESERVED, cur, p, 2);
            p += 2;
            continue;
        }

        if (strchr("+-*/()<>;=", *p)) {
            cur = new_token(TK_RESERVED, cur, p++, 1);
            continue;
        }

        if (strncmp(p, "return", 6) == 0 && !is_alnum(p[6])) {
            cur = new_token(TK_RETURN, cur, p, 6);
            p += 6;
            continue;
        } else if (strncmp(p, "if", 2) == 0 && !is_alnum(p[2])) {
            cur = new_token(TK_IF, cur, p, 2);
            p += 2;
            continue;
        } else if (strncmp(p, "else", 4) == 0 && !is_alnum(p[4])) {
            cur = new_token(TK_ELSE, cur, p, 4);
            p += 4;
            continue;
        }

        if (isalpha(*p)) {
            cur = new_token(TK_IDENT, cur, p, 0);
            char *q = p;
            while (is_alnum(*p)) p++;
            cur->len = p - q;
            continue;
        }

        if (isdigit(*p)) {
            cur = new_token(TK_NUM, cur, p, 0);
            char *q = p;
            cur->val = strtol(p, &p, 10);
            cur->len = p - q;
            continue;
        }

        error_at(p, "I can't tokenize it any more.");
    }

    new_token(TK_EOF, cur, p, 0);
    token = head.next;
}

// Determine whether alphanumeric characters or underscores are used.
static bool is_alnum(char c) {
    return isalnum(c) != 0 || c == '_';
}

static Token* new_token(TokenKind kind, Token* cur, char* str, int len) {
    Token* tok = calloc(1, sizeof(Token));
    tok->kind = kind;
    tok->str = str;
    tok->len = len;
    cur->next = tok;
    return tok;
}
