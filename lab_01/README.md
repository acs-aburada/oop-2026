# Week 1: C++ Refresher, Debugger, and Memory Model

## Learning Outcomes

By the end of the seminar, students should be able to:

* Create, build, and run a C++ project
* Explain the basic C++ build process
* Distinguish compilation, linking, runtime, and logical errors
* Use breakpoints and step through a program
* Inspect variables during execution
* Inspect the call stack
* Explain local variables and stack frames
* Distinguish a variable's value from its memory address
* Predict program behavior before execution
* Debug and explain a small C++ program

---

# 1. From C++ Source Code to an Executable

The simplified build process is:

```text
┌─────────────────────┐
│   C++ Source Code   │
└──────────┬──────────┘
           │
           ▼
┌─────────────────────┐
│      Compiler       │
└──────────┬──────────┘
           │
           │ produces
           ▼
    ┌──────────────┐
    │  Object Code │
    └──────────────┘
           │
           │ used by
           ▼
┌─────────────────────┐
│       Linker        │
└──────────┬──────────┘
           │
           ▼
┌─────────────────────┐
│     Executable      │
└─────────────────────┘
```

The important stages are:

1. **Source code**
2. **Compiler**
3. **Object code**
4. **Linker**
5. **Executable**

The object code is an intermediate result produced by the compiler. It is useful to understand its place in the process, but the main focus is the difference between the **compiler**, **linker**, and **executable**.

---

## 1.1 Source Code

Source code is the C++ code written by the programmer.

```cpp
#include <iostream>

int main()
{
    int x = 10;
    int y = 20;

    std::cout << x + y << '\n';
}
```

The source file might be:

```text
main.cpp
```

This is human-readable C++ code.

The processor does not directly execute this C++ source code.

---

# 1.2 Compiler

The **compiler** translates C++ source code into lower-level code.

It also checks whether the source code follows the rules of C++.

For example:

```cpp
int main()
{
    int x = ;
}
```

The compiler can detect that this is invalid C++.

This produces a **compilation error**.

### The compiler:

* reads C++ source code
* checks the source code
* performs translation
* produces object code

```text
main.cpp
   │
   ▼
Compiler
   │
   ▼
Object code
```

---

# 1.3 Object Code

Object code is the intermediate result produced by the compiler.

It is much closer to machine code than C++ source code.

For example:

```text
main.cpp
   │
   ▼
Compiler
   │
   ▼
main.o / main.obj
```

An object file is not necessarily a complete executable.

It can contain:

* generated machine-level instructions
* information about functions and variables
* references to code that still needs to be resolved

The linker uses this object code in the next stage.

---

# 1.4 Linker

The **linker** takes the object code and combines it with other required object files and libraries.

Its job is to create the final executable.

For example:

```text
main.cpp
   │
   ▼
Compiler
   │
   ▼
Object code
   │
   ▼
Linker
   │
   ▼
Executable
```

The linker also resolves references between different parts of the program.

---

## Example

```cpp
int calculate(int x);

int main()
{
    int result = calculate(10);
}
```

The compiler can understand:

```cpp
int calculate(int x);
```

This tells the compiler that a function called `calculate` exists.

However, there is no implementation:

```cpp
int calculate(int x)
{
    return x * 2;
}
```

The compiler can therefore produce object code, but the linker cannot find the actual implementation of `calculate()`.

This produces a **linking error**.

---

# 1.5 Executable

The linker produces the final executable.

```text
Source code
    ↓
Compiler
    ↓
Object code
    ↓
Linker
    ↓
Executable
```

Examples:

```text
program.exe
```

on Windows, or commonly:

```text
program
```

on Linux and macOS.

The executable is the final result of the build process.

At this point, the program can be started by the operating system.

---

# 2. Compilation vs Linking

| Stage       | Main responsibility                      | Example problem             |
| ----------- | ---------------------------------------- | --------------------------- |
| Compiler    | Understand and translate C++ source code | Invalid syntax              |
| Object code | Intermediate compiler output             | Not yet a complete program  |
| Linker      | Combine code and resolve references      | Missing function definition |
| Executable  | Final built program                      | Ready to be executed        |

---

# 3. Four Types of Problems

## Compilation Error

The source code violates the rules of C++.

```cpp
int main()
{
    int x = ;
}
```

```text
Source code
     ↓
Compiler
     ↓
❌ Compilation error
```

The program cannot continue through the build process.

---

## Linking Error

The source code can be compiled, but the linker cannot resolve something required to create the executable.

```cpp
int calculate(int x);

int main()
{
    int result = calculate(10);
}
```

```text
Source code
     ↓
Compiler
     ↓
Object code
     ↓
Linker
     ↓
❌ Linking error
```

---

## Runtime Error

The program successfully compiles and links, but something goes wrong while the program is running.

```cpp
int main()
{
    int a = 10;
    int b = 0;

    int result = a / b;
}
```

The problematic operation occurs during execution.

---

## Logical Error

The program compiles, links, and runs, but the logic is incorrect.

```cpp
int width = 5;
int height = 10;

int area = width + height;
```

The program can execute successfully while producing the wrong result.

---

# 4. Error Classification

| Problem           | Compiles | Links |       Runs      | Correct Result |
| ----------------- | :------: | :---: | :-------------: | :------------: |
| Compilation error |     ❌    |   ❌   |        ❌        |                |
| Linking error     |     ✅    |   ❌   |        ❌        |                |
| Runtime error     |     ✅    |   ✅   | ❌ / problematic |                |
| Logical error     |     ✅    |   ✅   |        ✅        |        ❌       |

---

# 5. Predict Before Running

Consider:

```cpp
#include <iostream>

int getSum(int a, int b)
{
    int result = a + b;
    return result;
}

int main()
{
    int x = 5;
    int y = 7;

    int sum = getSum(x, y);

    std::cout << sum << '\n';
}
```

Before running:

| Variable | Predicted value |
| -------- | --------------: |
| `x`      |               5 |
| `y`      |               7 |
| `a`      |               5 |
| `b`      |               7 |
| `result` |              12 |
| `sum`    |              12 |

The objective is to understand what happens during execution, not simply predict the final output.

---

# 6. Functions and the Stack
<p align="center">
  <img src="https://github.com/acs-aburada/oop-2026/blob/main/lab_01/cpp_memory_1.jpg" alt="C++ Memory 1" width="300">
  <img src="https://github.com/acs-aburada/oop-2026/blob/main/lab_01/cpp_memory_2.jpg" alt="C++ Memory 2" width="300">
</p>

<p align="center">
  <img src="https://github.com/acs-aburada/oop-2026/blob/main/lab_01/memory-layout.png" alt="C++ Memory 3" width="600">
</p>

When `main()` starts:

```text
main()
 ├── x
 ├── y
 └── sum
```

When `getSum()` is called:

```text
getSum()
 ├── a
 ├── b
 └── result

main()
 ├── x
 ├── y
 └── sum
```

Each function call has its own execution context.

A simplified representation is a **stack frame**.

```text
┌─────────────────────────┐
│ getSum()                │
│                         │
│ a                       │
│ b                       │
│ result                  │
├─────────────────────────┤
│ main()                  │
│                         │
│ x                       │
│ y                       │
│ sum                     │
└─────────────────────────┘
```

When `getSum()` returns:

```text
getSum()
    ↓
return
    ↓
main()
```

The local variables belonging to `getSum()` are no longer active.

> [!NOTE]
> The stack model is a conceptual model. Compiler optimizations can change how variables are physically stored.

---

# 7. What Is the Stack?

The **stack** is a region of memory used to manage function calls and local execution information.

It is commonly associated with:

* function calls
* local variables
* parameters
* return information
* nested function calls

For example:

```text
main()
   ↓
calculate()
   ↓
helper()
```

The call stack represents the currently active chain of functions.

---

# 8. Debugger

A debugger allows program execution to be observed step by step.

### Breakpoint

Pauses execution at a selected line.

```cpp
int result = a + b;
```

The program stops before executing that line.

### Continue

Continues execution until another breakpoint or the end of the program.

### Step Over

Executes the current line without entering a called function.

```cpp
int result = getSum(x, y);
```

The function executes, but the debugger does not enter it.

### Step Into

Enters the called function.

```text
main()
   ↓
getSum()
```

### Step Out

Finishes the current function and returns to the caller.

```text
getSum()
   ↓
return
   ↓
main()
```

### Variables / Locals

The debugger displays variables that are available at the current execution point.

Example:

```text
x      5
y      7
result 12
```

### Call Stack

The call stack shows the active function calls.

Example:

```text
getSum()
main()
```

This means:

```text
main()
   ↓
getSum()
```

---

# 9. Debugger Example

```cpp
int calculate(int x)
{
    int a = x + 5;
    int b = a * 2;

    return b;
}

int main()
{
    int value = 10;
    int result = calculate(value);

    std::cout << result << '\n';
}
```

### Prediction

```text
value  = 10
x      = 10
a      = 15
b      = 30
result = 30
```

---

# 10. Variables and Memory

A variable has a **value** and occupies a **location in memory**.

```cpp
int x = 42;
```

Conceptually:

```text
             x
             │
       ┌─────┴─────┐
       │           │
     value       address
       │           │
      42          &x
                   │
                   ▼
            memory location
```

> [!IMPORTANT]
> The value:
> ```text
> 42
> ```
> is different from the address where `x` is stored.

---

# 11. Individual Exercise + Evaluation

### 30 Minutes

-> [broken.cpp](https://github.com/acs-aburada/oop-2026/blob/main/lab_01/broken.cpp)

**Understand → Predict → Implement → Compile → Test → Debug → Explain → Refactor**
