#include "90cc.h"

void error(char* fmt, ...);
static int count(void);
static void gen_lval(Node* node);


void gen(Node* node) {
    switch (node->kind) {
    case ND_IF: {
        int i = count();
        gen(node->cond);
        printf("  pop rax\n");
        printf("  cmp rax, 0\n");
        printf("  je .Lelse%d\n", i);
        gen(node->then);
        printf("  jmp .Lend%d\n", i);
        printf(".Lelse%d:\n", i);
        if (node->els)
            gen(node->els);
        printf(".Lend%d:\n", i);
        return;
    }
    case ND_FOR: {
        int i = count();
        gen(node->init);
        printf(".Lbegin%d:\n", i);
        gen(node->cond);
        printf("  pop rax\n");
        printf("  cmp rax, 0\n");
        printf("  je .Lend%d\n", i);
        gen(node->then);
        gen(node->inc);
        printf("  jmp .Lbegin%d\n", i);
        printf(".Lend%d:\n", i);
        return;
    }
    case ND_WHILE: {
        int i = count();
        printf(".Lbegin%d:\n", i);
        gen(node->cond);
        printf("  pop rax\n");
        printf("  cmp rax, 0\n");
        printf("  je .Lend%d\n", i);
        gen(node->then);
        printf("  jmp .Lbegin%d\n", i);
        printf(".Lend%d:\n", i);
        return;
    }
    case ND_NUM:
        printf("  push %d\n", node->val);
        return;
    case ND_LVAR:
        gen_lval(node);
        printf("  pop rax\n");
        printf("  mov rax, [rax]\n");
        printf("  push rax\n");
        return;
    case ND_ASSIGN:
        gen_lval(node->lhs);
        gen(node->rhs);

        printf("  pop rdi\n");
        printf("  pop rax\n");
        printf("  mov [rax], rdi\n");
        printf("  push rdi\n");
        return;
    case ND_RETURN:
        gen(node->lhs);
        printf("  pop rax\n");
        printf("  mov rsp, rbp\n");
        printf("  pop rbp\n");
        printf("  ret\n");
        return;
    }

    gen(node->lhs);
    gen(node->rhs);

    printf("  pop rdi\n");
    printf("  pop rax\n");

    switch (node->kind) {
    case ND_ADD:
        printf("  add rax, rdi\n");
        break;
    case ND_EQ:
        printf("  cmp rax, rdi\n");
        printf("  sete al\n");
        printf("  movzb rax, al\n");
        break;
    case ND_GE:
        printf("  cmp rdi, rax\n");
        printf("  setle al\n");
        printf("  movzb rax, al\n");
        break;
    case ND_GT:
        printf("  cmp rdi, rax\n");
        printf("  setl al\n");
        printf("  movzb rax, al\n");
        break;
    case ND_SUB:
        printf("  sub rax, rdi\n");
        break;
    case ND_LE:
        printf("  cmp rax, rdi\n");
        printf("  setle al\n");
        printf("  movzb rax, al\n");
        break;
    case ND_LT:
        printf("  cmp rax, rdi\n");
        printf("  setl al\n");
        printf("  movzb rax, al\n");
        break;
    case ND_MUL:
        printf("  imul rax, rdi\n");
        break;
    case ND_NQ:
        printf("  cmp rax, rdi\n");
        printf("  setne al\n");
        printf("  movzb rax, al\n");
        break;
    case ND_DIV:
        printf("  cqo\n");
        printf("  idiv rdi\n");
        break;
    }

    printf("  push rax\n");
}

void error(char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vfprintf(stderr, fmt, ap);
    fprintf(stderr, "\n");
    exit(1);
}

static int count(void) {
    static int i = 0;
    return i++;
}

static void gen_lval(Node* node) {
    if (node->kind != ND_LVAR)
        error("local variable.");

    printf("  mov rax, rbp\n");
    printf("  sub rax, %d\n", node->offset);
    printf("  push rax\n");
}
