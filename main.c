#include "90cc.h"

Token* token;
char* user_input;

int main(int argc, char** argv) {
    if (argc != 2) {
        fprintf(stderr, "usage: 90cc command");
        return 1;
    }

    user_input = argv[1];
    token = tokenize();
    program();
  
    printf(".intel_syntax noprefix\n");
    printf(".globl main\n");
    printf("main:\n");

    printf("  push rbp\n");
    printf("  mov rbp, rsp\n");
    printf("  sub rsp, 208\n");

    for (int i = 0; code[i]; i++) {
        gen(code[i]);

        printf("  pop rax\n");
    }

    printf("  mov rsp, rbp\n");
    printf("  pop rbp\n");
    printf("  ret\n");
    return 0;    
}
