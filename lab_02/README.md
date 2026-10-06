# Week 2: Pointers, References, Dynamic Memory, and Ownership

> [!IMPORTANT]
>
> ## 🎯 Central Question
>
> **Who owns this memory, and how long does it exist?**
>
> This question will help you reason about pointers, references, dynamic memory, memory leaks, dangling pointers, and ownership throughout this seminar.

---

## Table of Contents

1. [From Week 1 to Week 2](#1-from-week-1-to-week-2)
2. [What Is a Pointer?](#2-what-is-a-pointer)
3. [The Two Important Operators](#3-the-two-important-operators)
4. [Changing a Value Through a Pointer](#4-changing-a-value-through-a-pointer)
5. [`nullptr`](#5-nullptr)
6. [Pointers and Function Parameters](#6-pointers-and-function-parameters)
7. [References in C++](#7-references-in-c)
8. [Pointer vs Reference](#8-pointer-vs-reference)
9. [Reference Parameters](#9-reference-parameters)
10. [Stack Memory and Dynamic Memory](#10-stack-memory-and-dynamic-memory)
11. [`new` and `delete`](#11-new-and-delete)
12. [Dynamic Arrays](#12-dynamic-arrays)
13. [Memory Leaks](#13-memory-leaks)
14. [Dangling Pointers](#14-dangling-pointers)
15. [Memory Lifetime](#15-memory-lifetime)
16. [Ownership](#16-ownership)
17. [Common Pointer Problems](#17-common-pointer-problems)
18. [Debugger Investigation](#18-debugger-investigation)
19. [Common Exercise: Grades](#19-common-exercise-grades)
20. [Individual Exercise](#20-individual-exercise)
21. [Individual Exercise: Dynamic Array](#21-individual-exercise-dynamic-array)
22. [Debugging Challenge](#22-debugging-challenge)
23. [Memory Diagrams](#23-memory-diagrams)

---

# 1. From Week 1 to Week 2

In Week 1, we established that a variable has:

* a **value**;
* a **location in memory**.

For example:

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

The `&` operator gives us the address of a variable.

This week, we will use those addresses directly.

---

# 2. What Is a Pointer?

A pointer is a variable that stores a memory address.

```cpp
int x = 42;
int* p = &x;
```

Conceptually:

```text
STACK

x = 42

p ──────────────────► x
                      42
```

Here:

* `x` is an `int`;
* `&x` is the address of `x`;
* `p` is a pointer to an `int`;
* `p` stores the address of `x`.

The type of the pointer tells us what type of object it points to.

```cpp
int* p;
double* d;
char* c;
```

These are pointers to different types.

---

# 3. The Two Important Operators

There are two operators that are especially important when working with pointers.

## Address-of: `&`

The `&` operator obtains the address of a variable.

```cpp
int x = 42;

int* p = &x;
```

Read this as:

> `p` stores the address of `x`.

---

## Dereference: `*`

The `*` operator can be used to access the object located at the address stored in a pointer.

```cpp
int x = 42;
int* p = &x;

std::cout << *p;
```

Output:

```text
42
```

Conceptually:

```text
p ───────────────► x
                  42
                  ▲
                  │
                 *p
```

`p` is the address.

`*p` is the value stored at that address.

---

# 4. Changing a Value Through a Pointer

A pointer can also be used to modify the object it points to.

```cpp
int x = 42;
int* p = &x;

*p = 100;
```

After this:

```text
x = 100
```

The pointer did not change which object it points to.

It changed the value of that object.

```text
p ───────────────► x
                  100
```

### Predict Before Running

What is printed?

```cpp
int x = 10;
int* p = &x;

*p = *p + 5;

std::cout << x << '\n';
```

Write your prediction before executing the program.

---

# 5. `nullptr`

A pointer does not have to point to an object.

It can represent the absence of an address:

```cpp
int* p = nullptr;
```

This means:

> `p` currently points to no object.

You can test for this:

```cpp
if (p == nullptr)
{
    std::cout << "No object";
}
```

A null pointer must **not** be dereferenced.

This is invalid:

```cpp
int* p = nullptr;

std::cout << *p;
```

There is no valid object for `*p` to access.

---

# 6. Pointers and Function Parameters

Consider:

```cpp
void change(int x)
{
    x = 100;
}
```

Calling:

```cpp
int value = 10;

change(value);

std::cout << value;
```

prints:

```text
10
```

The function receives a separate value.

A pointer parameter allows the function to access the original object:

```cpp
void change(int* x)
{
    *x = 100;
}
```

Calling:

```cpp
int value = 10;

change(&value);

std::cout << value;
```

prints:

```text
100
```

The important distinction is:

```text
value       → the value
&value      → the address of value
x           → the address received by the function
*x          → the original value
```

---

# 7. References in C++

> [!IMPORTANT]
>
> ## References are different from pointers
>
> If you already know C, you have worked with pointers and addresses before. C++ also provides **references**, which give another way to work with an existing object.
>
> A reference is an **alias**, or another name, for an existing variable.
>
> ```cpp
> int x = 42;
> int& ref = x;
> ```
>
> Here, `ref` refers to the same object as `x`.
>
> ```text
>        x
>        │
>        ▼
>       42
>        ▲
>        │
>       ref
> ```
>
> Changing `ref` changes `x`:
>
> ```cpp
> ref = 100;
> ```
>
> Now:
>
> ```text
> x == 100
> ```
>
> A reference is **not a separate copy** of the variable.
>
> Compare this with a pointer:
>
> ```cpp
> int x = 42;
> int* p = &x;
> ```
>
> With the pointer, `p` stores the address of `x`, and `*p` accesses the object.
>
> With the reference:
>
> ```cpp
> int& ref = x;
> ```
>
> `ref` directly refers to `x`.
>
> This gives a useful mental model:
>
> ```text
> Pointer:
>
> p ───────────────► x
>                    42
>
> Reference:
>
> ref ──────────────► x
>                    42
> ```
>
> The syntax and rules are different, but both allow code to work with an existing object rather than making a separate copy.
>
> One important difference is that a pointer can be `nullptr` and can later point somewhere else:
>
> ```cpp
> int* p = &a;
> p = &b;
> ```
>
> A reference is bound to an object when it is initialized:
>
> ```cpp
> int& ref = a;
> ```
>
> `ref` does not later become a reference to `b`.

---

# 8. Pointer vs Reference

Compare:

```cpp
int x = 42;

int* p = &x;
int& r = x;
```

|                                           | Pointer        | Reference                        |
| ----------------------------------------- | -------------- | -------------------------------- |
| Stores an address                         | Yes            | Conceptually refers to an object |
| Can represent no object                   | Yes, `nullptr` | No                               |
| Can be changed to refer to another object | Yes            | No                               |
| Access original object                    | `*p`           | `r`                              |
| Can be used with `nullptr`                | Yes            | No                               |

Example:

```cpp
int a = 10;
int b = 20;

int* p = &a;
p = &b;
```

Now `p` points to `b`.

With a reference:

```cpp
int& r = a;
```

`r` continues to refer to `a`.

---

# 9. Reference Parameters

References are commonly used when a function should modify an existing variable.

```cpp
void increase(int& value)
{
    value++;
}
```

Then:

```cpp
int x = 10;

increase(x);

std::cout << x;
```

Output:

```text
11
```

The function works directly with the original variable.

This is different from pass-by-value:

```cpp
void increase(int value)
{
    value++;
}
```

Here, the original variable is not modified.

---

# 10. Stack Memory and Dynamic Memory

<p align="center">
  <img src="https://github.com/acs-aburada/oop-2026/blob/main/lab_01/cpp_memory_1.jpg" alt="C++ Memory 1" width="300">
  <img src="https://github.com/acs-aburada/oop-2026/blob/main/lab_01/cpp_memory_2.jpg" alt="C++ Memory 2" width="300">
</p>

So far, the variables we have used are created automatically as part of normal program execution.

For example:

```cpp
void function()
{
    int x = 42;
}
```

`x` has a lifetime associated with the execution of `function()`.

When the function finishes, `x` no longer exists.

Dynamic memory works differently.

We can request memory while the program is running:

```cpp
int* p = new int;
```

Now `p` is a pointer to dynamically allocated memory.

Conceptually:

```text
STACK                         DYNAMIC MEMORY

p ──────────────────────────► [    ]
```

We can store a value there:

```cpp
*p = 42;
```

Now:

```text
STACK                         DYNAMIC MEMORY

p ──────────────────────────► [ 42 ]
```

The dynamically allocated object continues to exist until it is released.

---

# 11. `new` and `delete`

Memory allocated using:

```cpp
new
```

must eventually be released.

For a single object:

```cpp
int* p = new int;

*p = 42;

delete p;
```

After `delete`, the dynamically allocated object no longer exists.

The pointer variable itself still exists.

Therefore, a useful practice is:

```cpp
delete p;
p = nullptr;
```

This makes it explicit that `p` should no longer be used to access the deleted object.

---

# 12. Dynamic Arrays

A dynamic array can be created using `new[]`.

```cpp
int n = 4;

int* values = new int[n];
```

Conceptually:

```text
STACK                         DYNAMIC MEMORY

n = 4

values ─────────────────────► [ ][ ][ ][ ]
```

The elements can be accessed using normal indexing:

```cpp
values[0] = 7;
values[1] = 9;
values[2] = 10;
values[3] = 6;
```

The array must eventually be released using `delete[]`:

```cpp
delete[] values;
values = nullptr;
```

### Important

Use:

```cpp
delete
```

for memory created with:

```cpp
new
```

Use:

```cpp
delete[]
```

for memory created with:

```cpp
new[]
```

---

# 13. Memory Leaks

A memory leak occurs when dynamically allocated memory is no longer accessible but has not been released.

Example:

```cpp
void createMemory()
{
    int* values = new int[100];
}
```

When `createMemory()` finishes, the local pointer `values` disappears.

But the dynamically allocated array is still there.

There is now no pointer through which the program can access that memory.

The memory cannot be properly released.

That is a memory leak.

Conceptually:

```text
STACK                         DYNAMIC MEMORY

values ────────X────────────► [ ][ ][ ][ ][ ... ]

pointer disappears
memory remains allocated
```

### Rule

Every dynamic allocation needs a clear plan for who is responsible for releasing it.

---

# 14. Dangling Pointers

A dangling pointer is a pointer that refers to an object that no longer exists.

For example:

```cpp
int* createValue()
{
    int value = 10;
    return &value;
}
```

The function attempts to return the address of `value`.

But `value` is a local variable.

When `createValue()` finishes, `value` no longer exists.

The returned address therefore does not refer to a valid `int` object.

The problem is not that obtaining the address was invalid.

The problem is that the **object's lifetime ends**.

### Key question

> **Does the object still exist when the pointer is used?**

This question is central to understanding dangling pointers.

---

# 15. Memory Lifetime

When working with pointers, always consider two separate things:

1. **Where is the pointer stored?**
2. **What object does the pointer refer to, and does that object still exist?**

For example:

```cpp
void example()
{
    int* p = new int(42);
}
```

There are two different objects involved:

```text
STACK                         DYNAMIC MEMORY

p ──────────────────────────► [42]
```

The pointer `p` is a local variable.

The dynamically allocated `int` is a separate object.

When `example()` ends:

```text
p
```

ceases to exist.

But unless the memory was deleted, the dynamically allocated `int` does not automatically disappear.

This is why ownership matters.

---

# 16. Ownership

Ownership answers the question:

> **Who is responsible for releasing this dynamically allocated memory?**

Consider:

```cpp
int* values = new int[5];
```

Someone must eventually execute:

```cpp
delete[] values;
```

A program becomes easier to reason about when ownership is clear.

For example:

```text
createGrades()
       │
       ▼
  creates array
       │
       ▼
caller owns array
       │
       ▼
caller releases array
```

Ownership is not simply about knowing where memory is.

It is about knowing **who is responsible for its lifetime**.

---

# 17. Common Pointer Problems

## Null pointer dereference

```cpp
int* p = nullptr;

std::cout << *p;
```

Problem:

> `p` does not point to an object.

---

## Dangling pointer

```cpp
int* p = new int(42);

delete p;

std::cout << *p;
```

Problem:

> The object has already been deleted.

---

## Memory leak

```cpp
int* p = new int(42);
```

with no corresponding:

```cpp
delete p;
```

Problem:

> The allocated memory is never released.

---

## Wrong form of deletion

```cpp
int* values = new int[5];

delete values;
```

Problem:

> Memory allocated with `new[]` must be released with `delete[]`.

Correct:

```cpp
delete[] values;
```

---

# 18. Debugger Investigation

The debugger is especially useful when working with pointers.

Consider:

```cpp
int x = 42;
int* p = &x;

*p = 100;
```

Set a breakpoint before:

```cpp
*p = 100;
```

Inspect:

* `x`
* `p`
* `&x`
* `*p`

Before the assignment, reason about:

```text
x  = ?
p  = ?
*p = ?
```

After the assignment:

```text
x  = ?
p  = ?
*p = ?
```

The important observation is that changing `*p` changes `x`.

---

# 19. Common Exercise: Grades

Build a small program that dynamically stores a set of grades.

The program should contain functions similar to:

```cpp
int* createGrades(int n);

void addGrade(int* grades, int index, int value);

double average(int* grades, int n);

void deleteGrades(int* grades);
```

The exact implementation is part of the exercise.

The intended memory model is:

```text
STACK                         DYNAMIC MEMORY

grades ─────────────────────► [7][9][10][6]
```

The program should:

1. ask for the number of grades;
2. allocate enough dynamic memory;
3. add values to the array;
4. calculate the average;
5. display the result;
6. release the allocated memory.

---

# 20. Individual Exercise

## Part A: Predict

Before running the program, determine what each variable contains.

```cpp
int value = 10;
int* p = &value;

*p = 20;

std::cout << value << '\n';
std::cout << *p << '\n';
```

Record:

| Expression    | Prediction |
| ------------- | ---------: |
| `value`       |            |
| `p`           |            |
| `*p`          |            |
| first output  |            |
| second output |            |

Then run the program and compare your prediction with the actual behavior.

---

## Part B: Debug

Investigate the following program:

```cpp
#include <iostream>

int* createValue()
{
    int value = 10;
    return &value;
}

int main()
{
    int* p = createValue();

    std::cout << *p << '\n';

    return 0;
}
```

Do not immediately change the code.

First determine:

1. What does the compiler report?
2. Where is `value` created?
3. When does `value` stop existing?
4. What does `p` contain?
5. Does `p` point to a valid object when `*p` is evaluated?
6. What kind of problem does this program contain?

Use the debugger to investigate the lifetime of `value`.

---

# 21. Individual Exercise: Dynamic Array

Create a program that dynamically allocates an array of integers.

The program should:

* read the array size;
* allocate the array dynamically;
* read the values;
* calculate the average;
* display the values;
* release the memory.

The program must work for different array sizes.

For example:

```text
Number of values: 4

Values:
7
9
10
6

Average: 8
```

Test at least three different input sets.

---

# 22. Debugging Challenge

Start with a working dynamic-array program.

Then investigate these changes one at a time.

### Challenge 1

Remove the `delete[]`.

What happens to the allocated memory?

---

### Challenge 2

Move `delete[]` before the last use of the array.

What happens?

---

### Challenge 3

Set the pointer to `nullptr` after deleting the array.

What changes?

---

### Challenge 4

Try to use the array after it has been deleted.

Is the pointer itself valid?

Is the object it used to point to still valid?

---

# 23. Memory Diagrams

For each example, draw a diagram containing:

* stack variables;
* pointers;
* dynamically allocated objects;
* arrows showing what each pointer refers to.

Example:

```cpp
int x = 5;
int* p = &x;
```

Draw:

```text
STACK

x = 5
 ▲
 │
 p
```

For:

```cpp
int* p = new int(5);
```

draw:

```text
STACK                         DYNAMIC MEMORY

p ──────────────────────────► [5]
```

For:

```cpp
int* p = new int(5);

delete p;
```

explain what happened to the object that `p` used to refer to.

---

<p align="center">
  <img src="https://github.com/acs-aburada/oop-2026/blob/main/lab_01/memory-layout.png" alt="C++ Memory 3" width="600">
</p>

This version is now directly usable as the `.md` content, with **no Check Your Understanding, Definition of Done, or Week 2 Summary**.
