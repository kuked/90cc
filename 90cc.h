#ifndef _90CC_H
#define _90CC_H

#include <ctype.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node Node;
typedef struct Token Token;

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

    ND_NUM,  // integer
} NodeKind;

struct Node {
    NodeKind kind;
    Node* lhs;
    Node* rhs;
    int val;
};

// 90cc.c
Node* expr(void);
Token* tokenize();

// codegen.c
void gen(Node* node);

// main.c
extern Token* token;
extern char* user_input;

#endif  // _90CC_H
