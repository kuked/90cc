#ifndef _90CC_H
#define _90CC_H

#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {
    ND_ADD,  // +
    ND_SUB,  // -
    ND_MUL,  // *
    ND_DIV,  // /

    ND_EQ,   // ==
    ND_NQ,   // !=
    ND_LT,   // <
    ND_LE,   // <=
    ND_GT,   // >
    ND_GE,   // >=

    ND_LVAR,      // local variable
    ND_ASSIGN,    // =

    ND_RETURN,    // return

    ND_NUM,  // integer
} NodeKind;

typedef struct Node Node;
struct Node {
    NodeKind kind;
    Node* lhs;
    Node* rhs;
    int val;
    int offset;
};

typedef enum {
    TK_RESERVED,
    TK_IDENT,
    TK_NUM,
    TK_RETURN,
    TK_EOF,
} TokenKind;

typedef struct Token Token;
struct Token {
    TokenKind kind;
    Token* next;
    int val;
    char* str;
    int len;
};

//
// parser.c
//

// local variable
typedef struct LVar LVar;
struct LVar {
    LVar* next;
    char* name;
    int len;
    int offset;
};

Node* code[100];
void error(char* fmt, ...);
void tokenize(void);
Node* primary(void);
Node* unary(void);
Node* mul(void);
Node* add(void);
Node* relational(void);
Node* equality(void);
Node* assign(void);
Node* expr(void);
Node* stmt(void);
void program();

//
// codegen.c
//

void gen(Node* node);

//
// main.c
//

extern char* user_input;

#endif  // _90CC_H
