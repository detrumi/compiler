compiler
========

A compiler written in C++11 for a functional language

Currently it's just an interpreter, but I'm planning to eventually use llvm as a backend.

## Current features
- Runs only as interpreter
- Only integer type, prefix notation
- Built-in arithmetic functions (+ - * /) with arbitrary number of arguments
- Some error handling (some inputs still crash)

## Planned features
- Better lexing and parsing, infix notation
- A type system and (hopefully) type inference
- Things like (multi-line) code blocks and pattern matching
- Code generation for llvm IR

## Examples
    >>> + 1 2 3              # Call built-in function call
    6
    >>> def succ x = + x 1   # Function definition
    >>> * (succ 2) (succ 3)  # Function calls
    12
    
