compiler
========

A compiler written in C++11 for a functional language

## Current features
- Runs as interpreter
- Only integer type
- infix operators, prefix function calls, lambdas
- Built-in arithmetic functions (+ - * /)
- Parser error handling

## Examples
    >>> 1 + 2 * 3                   # Built-in operators
    7
    >>> let f x y = x * x - y * y   # Function definition
    >>> let succ x = x + 1    
    >>> succ 2 * succ (7 - 4)       # Function calls
    12
    >>> (\x y. x / y) 10 3			# Lambda functions
    3
    
