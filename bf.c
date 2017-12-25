#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAPE_SIZE 30000

static unsigned char tape[TAPE_SIZE];
static int jump[1 << 16];

static int build_jumps(const char *prog)
{
    int stack[4096], sp = 0;
    for (int pc = 0; prog[pc]; pc++) {
        if (prog[pc] == '[') {
            stack[sp++] = pc;
        } else if (prog[pc] == ']') {
            if (sp == 0) {
                fprintf(stderr, "unmatched ] at %d\n", pc);
                return -1;
            }
            int open = stack[--sp];
            jump[open] = pc;
            jump[pc] = open;
        }
    }
    if (sp != 0) {
        fprintf(stderr, "unmatched [ at %d\n", stack[sp - 1]);
        return -1;
    }
    return 0;
}

static void run(const char *prog)
{
    int ptr = 0;
    for (int pc = 0; prog[pc]; pc++) {
        switch (prog[pc]) {
        case '>': ptr = (ptr + 1) % TAPE_SIZE; break;
        case '<': ptr = (ptr + TAPE_SIZE - 1) % TAPE_SIZE; break;
        case '+': tape[ptr]++; break;
        case '-': tape[ptr]--; break;
        case '.': putchar(tape[ptr]); break;
        case ',': tape[ptr] = getchar(); break;
        case '[': if (!tape[ptr]) pc = jump[pc]; break;
        case ']': if (tape[ptr]) pc = jump[pc]; break;
        }
    }
}

int main(int argc, char **argv)
{
    if (argc < 2) {
        fprintf(stderr, "usage: %s prog.bf\n", argv[0]);
        return 1;
    }
    FILE *f = fopen(argv[1], "rb");
    if (!f) {
        perror(argv[1]);
        return 1;
    }
    char *src = malloc(1 << 20);
    size_t n = fread(src, 1, (1 << 20) - 1, f);
    src[n] = 0;
    fclose(f);

    if (build_jumps(src) != 0)
        return 1;
    run(src);
    return 0;
}
