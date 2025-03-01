# SIC-assembler
## Overview

This project involves writing a SIC assembler in three stages, with each stage progressively building on the previous one. By the end, the assembler will produce object files from SIC assembly programs. This ReadMe explains the tasks and steps for using the program.

---

## Stage 1: Normalization (exer3)

A program that reads a SIC assembly program from `stdin` and outputs a normalized version to `stdout`. The normalization process involves:

1. **Removing comments.**
2. **Transforming characters to uppercase**, except for lowercase constants (e.g., `c'eof'` or `x'f3'`).
3. **Adjusting the layout** to:
   - Place labels in columns 1-8.
   - Place opcodes in columns 10-15.
   - Place operands in columns 18-35.

### Input/Output:

- Input is read via `stdin`.
- Output is written to `stdout`.

### Compilation and Execution:

```bash
$ g++ exer3.cpp
$ mv a.out my-split
$ ./my-split < SAMPLE-input-1 > k-1
$ diff SAMPLE-split-1 k-1
```

### Tips:

- Use `diff` or `cat -v` to verify output matches the provided sample split files (`SAMPLE-split-1`, `SAMPLE-split-2`, `SAMPLE-split-3`).
- Example: Redirect input/output:
  ```bash
  $ cat SAMPLE-input-1 | ./my-split | cat -v
  ```

---

## Stage 2: Pass 1 (Exercise 4)

### Objective:

A program (`exer4.cpp`, `exer4.cc`, or `exer4.c`) that reads the normalized SIC assembly program from `stdin` and generates two output files:

1. **INTFILE:** Intermediate file, which includes the location counter (LOC) at the beginning of each line.
2. **SYMTAB:** Symbol table, which:
   - Starts with the program name, starting address, and program length (in hexadecimal).
   - Contains all symbols and their addresses (order is flexible).

### Input/Output:

- Input is read via `stdin` (output of the normalization program).
- INTFILE and SYMTAB are written as files.

### Compilation and Execution:

```bash
$ g++ exer4.cpp
$ mv a.out your-pass1
$ cat SAMPLE-split-1 | ./your-pass1
$ diff INTFILE SAMPLE-INTFILE-1
$ ./sort-sym SYMTAB | diff - SAMPLE-SYMTAB-1
```

### Notes:

- If you don’t have a working Exercise 3, use the provided sample split files as input.
- To verify the output:
  ```bash
  $ ./sort-sym SYMTAB | diff - SAMPLE-SYMTAB-1
  ```

---

## Stage 3: Pass 2 (Exercise 5)

### Objective:

Write a program (`exer5.cpp`, `exer5.cc`, or `exer5.c`) that reads the **SYMTAB** and **INTFILE** generated in Exercise 4 and produces the object file as output to `stdout`.

### Input/Output:

- Input files: `SYMTAB` and `INTFILE`.
- Output: Object file printed to `stdout`.

### Compilation and Execution:

```bash
$ g++ exer5.cpp -o pass2
$ cat SAMPLE-split-1 | ./your-pass1
$ ./pass2 > OBJECTFILE
```

### Notes:

- If Exercise 4 is not working, use `SAMPLE-SYMTAB-1` and `SAMPLE-INTFILE-1` as input.
- The object file’s format is not unique. Verify its correctness by running it in the provided simulator.

### Example:

```bash
$ cp SAMPLE-SYMTAB-1 SYMTAB
$ cp SAMPLE-INTFILE-1 INTFILE
$ ./pass2
```

---

## Key Points

- Follow the format strictly for each file (use `diff` for verification).
- Exercises 4 and 5 depend on intermediate files. Ensure outputs from earlier stages are accurate.
- Use memory tables for symbol lookup in Exercise 5, not file-based searches (violations will result in zero points).

---

## Testing and Validation

- Use sample files for testing.
- Redirect outputs to files and use comparison tools like `diff`.
- Verify the object file by running it through SIC simulator.


