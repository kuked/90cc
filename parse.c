#include "90cc.h"

Token* token;
Node* code[100];
LVar* locals;

void error(char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}

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

bool consume(char *op) {
    if ((token->kind != TK_RESERVED && token->kind != TK_RETURN) ||
        (int)strlen(op) != token->len || memcmp(token->str, op, token->len))
        //if (token->kind != TK_RESERVED || (int)strlen(op) != token->len || memcmp(token->str, op, token->len))
        return false;
    token = token->next;
    return true;
}

Token* consume_ident(void) {
    if (token->kind == TK_IDENT) {
        Token *tok = token;
        token = token->next;
        return tok;
    }
    return NULL;
}


void expect(char* op) {
    if (token->kind != TK_RESERVED || (int)strlen(op) != token->len || memcmp(token->str, op, token->len))
        error_at(token->str, "It's not '%s'.", op);
    token = token->next;
}

int expect_number() {
    if (token->kind != TK_NUM)
        error_at(token->str, "It's not a number");
    int val = token->val;
    token = token->next;
    return val;
}

bool at_eof() {
    return token->kind == TK_EOF;
}

// Determine whether alphanumeric characters or underscores are used.
bool is_alnum(char c) {
    return isalnum(c) != 0 || c == '_';
}

Token* new_token(TokenKind kind, Token* cur, char* str, int len) {
    Token* tok = calloc(1, sizeof(Token));
    tok->kind = kind;
    tok->str = str;
    tok->len = len;
    cur->next = tok;
    return tok;
}

void tokenize() {
    char*p = user_input;
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

Node* new_node(NodeKind kind, Node* lhs, Node* rhs) {
    Node* node = calloc(1, sizeof(Node));
    node->kind = kind;
    node->lhs = lhs;
    node->rhs = rhs;
    return node;
}

Node* new_node_num(int val) {
    Node* node = calloc(1, sizeof(Node));
    node->kind = ND_NUM;
    node->val = val;
    return node;
}

// Searchs for variables by name. If not found, returns NULL.
LVar* find_lvar(Token *tok) {
    for (LVar *var = locals; var; var = var->next)
        if (var->len == tok->len && !memcmp(tok->str, var->name, var->len))
            return var;
    return NULL;
}

Node* primary(void) {
    if (consume("(")) {
        Node* node = expr();
        expect(")");
        return node;
    }

    Token* tok = consume_ident();
    if (tok) {
        Node* node = calloc(1, sizeof(Node));
        node->kind = ND_LVAR;

        LVar* lvar = find_lvar(tok);
        if (lvar) {
            node->offset = lvar->offset;
        } else {
            lvar = calloc(1, sizeof(LVar));
            lvar->next = locals;
            lvar->name = tok->str;
            lvar->len = tok->len;
            lvar->offset = locals->offset + 8;
            node->offset = lvar->offset;
            locals = lvar;
        }
        return node;
    }

    return new_node_num(expect_number());
}

Node* unary(void) {
    if (consume("+"))
        return unary();
    if (consume("-"))
        return new_node(ND_SUB, new_node_num(0), unary());
    return primary();
}

Node* mul(void) {
    Node* node = unary();

    for (;;) {
        if (consume("*"))
            node = new_node(ND_MUL, node, unary());
        else if (consume("/"))
            node = new_node(ND_DIV, node, unary());
        else
            return node;
    }
}

Node* add(void) {
    Node* node = mul();

    for (;;) {
        if (consume("+"))
            node = new_node(ND_ADD, node, mul());
        else if (consume("-"))
            node = new_node(ND_SUB, node, mul());
        else
            return node;
    }
}

Node* relational(void) {
    Node* node = add();

    for (;;) {
        if (consume("<"))
            node = new_node(ND_LT, node, add());
        else if (consume("<="))
            node = new_node(ND_LE, node, add());
        else if (consume(">"))
            node = new_node(ND_GT, node, add());
        else if (consume(">="))
            node = new_node(ND_GE, node, add());
        else
            return node;
    }
    return node;
}

Node* equality(void) {
    Node* node = relational();

    for (;;) {
        if (consume("=="))
            node = new_node(ND_EQ, node, relational());
        if (consume("!="))
            node = new_node(ND_NQ, node, relational());
        else
            return node;
    }
}

Node* assign(void) {
    Node *node = equality();
    if (consume("="))
        node = new_node(ND_ASSIGN, node, assign());
    return node;
}

Node* expr(void) {
    return assign();
}

Node* stmt() {
    Node* node;

    if (consume("return")) {
        node = calloc(1, sizeof(Node));
        node->kind = ND_RETURN;
        node->lhs = expr();
    } else {
        node = expr();
    }

    expect(";");
    return node;
}

void program(void) {
    int i = 0;
    while (!at_eof())
        code[i++] = stmt();
    code[i] = NULL;
}
