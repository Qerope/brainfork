#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAPE_SIZE 30000

enum { OP_ADD, OP_MOVE, OP_IN, OP_OUT, OP_JZ, OP_JNZ, OP_CLEAR };

struct op {
    int kind;
    int arg;
};

static struct op *code;
static int ncode;
static unsigned char tape[TAPE_SIZE];

static int compile(const char *src)
{
    int stack[4096], sp = 0;
    size_t cap = strlen(src) + 1;

    code = malloc(cap * sizeof *code);
    ncode = 0;
    for (const char *s = src; *s; s++) {
        switch (*s) {
        case '+':
        case '-':
        case '>':
        case '<': {
            int kind = (*s == '+' || *s == '-') ? OP_ADD : OP_MOVE;
            int step = (*s == '+' || *s == '>') ? 1 : -1;
            if (ncode && code[ncode - 1].kind == kind)
                code[ncode - 1].arg += step;
            else
                code[ncode++] = (struct op){kind, step};
            break;
        }
        case '.': code[ncode++] = (struct op){OP_OUT, 0}; break;
        case ',': code[ncode++] = (struct op){OP_IN, 0}; break;
        case '[':
            stack[sp++] = ncode;
            code[ncode++] = (struct op){OP_JZ, 0};
            break;
        case ']': {
            if (sp == 0) {
                fprintf(stderr, "unmatched ]\n");
                return -1;
            }
            int open = stack[--sp];
            code[open].arg = ncode;
            code[ncode++] = (struct op){OP_JNZ, open};
            break;
        }
        }
    }
    if (sp != 0) {
        fprintf(stderr, "unmatched [\n");
        return -1;
    }
    return 0;
}

static void run(void)
{
    size_t ptr = 0;

    for (int pc = 0; pc < ncode; pc++) {
        struct op *o = &code[pc];
        switch (o->kind) {
        case OP_ADD: tape[ptr] += o->arg; break;
        case OP_MOVE: ptr = (ptr + o->arg + TAPE_SIZE) % TAPE_SIZE; break;
        case OP_OUT: putchar(tape[ptr]); break;
        case OP_IN: {
            int c = getchar();
            if (c != EOF)
                tape[ptr] = c;
            break;
        }
        case OP_JZ: if (!tape[ptr]) pc = o->arg; break;
        case OP_JNZ: if (tape[ptr]) pc = o->arg; break;
        }
    }
}

int main(int argc, char **argv)
{
    if (argc <= argi) {
        fprintf(stderr, "usage: %s [-d] prog.bf\n", argv[0]);
        return 1;
    }
    FILE *f = fopen(argv[argi], "rb");
    if (!f) {
        perror(argv[argi]);
        return 1;
    }
    char *src = malloc(1 << 20);
    size_t n = fread(src, 1, (1 << 20) - 1, f);
    src[n] = 0;
    fclose(f);

    if (compile(src) != 0)
        return 1;
    run();
    return 0;
}
