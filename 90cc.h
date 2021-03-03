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

// 90cc.c
Node* expr(void);
Token* tokenize();
void gen(Node* node);

// main.c
extern Token* token;
extern char* user_input;

#endif  // _90CC_H
