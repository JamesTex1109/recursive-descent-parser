# Recursive Descent Parser

A lexical analyzer and recursive descent syntax parser written in C for **Cooke**, a small teaching language with assignments, I/O, conditionals, and loops. It reads a source file, breaks it into tokens, and checks it against the language grammar, reporting the exact line and token where a syntax error happens.

## How It Works

Source code goes through two stages:

1. **Lexer (`front.c`)** reads characters one at a time and groups them into tokens like `IDENT`, `INT_LITERAL`, `ASSIGNMENT_OP` (`:=`), and `RANGE_OP` (`..`). It uses one character of lookahead to tell apart pairs like `<` / `<=` / `<>` / `<<`, and it makes sure `1..10` becomes a range instead of the float `1.`.
2. **Parser (`parser.c`)** has one function per grammar rule. Each function calls the functions for the rules it contains, so the call stack ends up mirroring the structure of the program. If the next token isn't what the grammar expects, it stops and reports the error.

## Grammar

```
P  → S { ; S }
S  → V := E  |  cin >> V  |  cout << E
   |  if C : S { ; S } M
   |  for V in E .. E : S { ; S }
   |  ++V  |  --V
M  → elif C : S { ; S } M  |  else : S { ; S }  |  ε
C  → A { or A }
A  → R { and R }
R  → not R  |  E [ (< | > | == | <> | <= | >=) E ]
E  → T { (+ | -) T }
T  → F { (* | / | %) F }
F  → V  |  ++V  |  --V  |  N  |  ( E )
```

Operator precedence comes from how the rules nest: `F` binds tighter than `T`, which binds tighter than `E`, so `*` is evaluated before `+`.

## Build and Run

Requires `gcc` and `make`.

```bash
make
./cooke_parser program.cooke
```

**Example input:**
```
cin >> x;
if x > 10 :
    cout << x * 2
else :
    cout << x
```

**Valid program:**
```
Cooke Parser
Syntax Validated
```

**Program with an error:**
```
Cooke Parser
Error encounter on line 3: The next lexeme was ) and the next token was RIGHT_PAREN
```

| Exit code | Meaning |
|---|---|
| 0 | Syntax is valid |
| 1 | Syntax error |
| 2 | No input file given |
| 3 | Input file not found |

## Files
| File | Purpose |
|---|---|
| `front.c` / `front.h` | Lexer and token codes |
| `parser.c` / `parser.h` | Recursive descent parser |
| `Makefile` | Build script (`make`, `make clean`) |
