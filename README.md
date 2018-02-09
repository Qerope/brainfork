# brainfork

Small Brainfuck interpreter in C.
## Usage

    make
    ./bf prog.bf

`-d` prints the compiled op stream instead of running the program.

Tests: `make test`
## Notes

Programs are compiled to a flat op array. Runs of `+ - < >` are folded into
one op and `[-]` is turned into a single clear op. With gcc or clang the main
loop uses computed goto (`-DUSE_COMPUTED_GOTO`, on by default in the Makefile).
