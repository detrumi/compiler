compiler
========

A compiler written in C++11 for a functional language

## Current features
- Runs as interpreter
- Only integer type
- infix operators, prefix function calls
- Built-in arithmetic functions (+ - * /)
- Some syntax error handling (some inputs still crash)

## Planned features
- Better lexing and parsing, infix notation
- A type system and (hopefully) type inference
- Things like (multi-line) code blocks and pattern matching
- Optimizations

## Examples
    >>> 1 + 2 * 3                   # Built-in operators
    7
    >>> def f x y = x * x - y * y   # Function definition
    >>> def succ x = x + 1    
    >>> succ 2 * succ (7 - 4)       # Function calls
    12
    
