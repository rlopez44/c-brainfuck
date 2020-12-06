# C-Brainfuck

An interpreter for the Brainfuck language

## Compiling the Interpreter

To compile the interpreter, simply run `make` in the project's root
directory. Alternatively, to compile the interpreter with debug
symbols, run `make debug` in the project's root directory. After
running either of these commands, the Brainfuck interpreter will be
found in the `bin/` subdirectory.

## Clean Up

The `make` and `make debug` commands create `obj/` and `bin/`
subdirectories into which object files and the Brainfuck interpreter
are output during compilation and linking, respectively. To remove
these directories and their contents, simply run `make clean` (to
remove the `obj/` directory) or `make full-clean` (to remove both the
`obj/` and the `bin/` directory). Sometimes one would like to compile
the Brainfuck interpreter from a complete fresh start, in which case
these two commands are useful.
