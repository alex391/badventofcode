# 2025
All of the days in C this year use my set of c libraries written with AoC in
mind. All of the day-specific logic is in `main.c`. Because I'm using single
compilation unit, you can compile any of the days with:
```bash
clang -std=c2x main.c -o main
```
but day 2 uses regex, and so will need
```bash
clang -std=c2x -lpcre2-8 main.c -o main
```
Or, if you're on linux, just use the provided `main` binary. gcc probably works also.  
I don't have any idea if this will work at all on Windows, good luck.
