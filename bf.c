#include <stdio.h>
#include <string.h>

#define TAPE_SIZE 30000

static unsigned char tape[TAPE_SIZE];

static void run(const char *prog)
{
    int ptr = 0;
    for (int pc = 0; prog[pc]; pc++) {
        switch (prog[pc]) {
        case '>': ptr++; break;
        case '<': ptr--; break;
        case '+': tape[ptr]++; break;
        case '-': tape[ptr]--; break;
        case '.': putchar(tape[ptr]); break;
        case ',': tape[ptr] = getchar(); break;
        case '[':
            if (!tape[ptr]) {
                int d = 1;
                while (d) {
                    pc++;
                    if (prog[pc] == '[') d++;
                    else if (prog[pc] == ']') d--;
                }
            }
            break;
        case ']':
            if (tape[ptr]) {
                int d = 1;
                while (d) {
                    pc--;
                    if (prog[pc] == ']') d++;
                    else if (prog[pc] == '[') d--;
                }
            }
            break;
        }
    }
}

int main(int argc, char **argv)
{
    run("++++++++[>++++[>++>+++>+++>+<<<<-]>+>+>->>+[<]<-]>>.>---.+++++++..+++.>>.<-.<.+++.------.--------.>>+.");
    return 0;
}
