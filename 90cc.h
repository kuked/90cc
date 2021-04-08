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

    ND_IF,        // if
    ND_RETURN,    // return
    ND_WHILE,     // while

    ND_NUM,  // integer
} NodeKind;

typedef struct Node Node;
struct Node {
    NodeKind kind;
    Node* lhs;
    Node* rhs;
    int val;
    int offset;

    Node* cond;
    Node* then;
    Node* els;
};

typedef enum {
    TK_RESERVED,
    TK_IDENT,
    TK_NUM,
    TK_KEYWORD,
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

// local variable
typedef struct LVar LVar;
struct LVar {
    LVar* next;
    char* name;
    int len;
    int offset;
};


//
// tokenize.c
//

extern Token* token;
extern LVar* locals;

void error_at(char* loc, char* fmt, ...);
void tokenize(void);

//
// parser.c
//

extern Node* code[100];

void error(char* fmt, ...);
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
