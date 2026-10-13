# Week 3: From Requirements to Classes and Encapsulation

> [!IMPORTANT]
> ## 🎯 Central Question for the Semester
>
> **What is this class responsible for?**
>
> Throughout the semester, we will return to this question when deciding what data an object should contain, what operations it should provide, and which responsibilities belong elsewhere.

## Learning Outcomes

By the end of this seminar, you should be able to:

- identify objects from requirements;
- distinguish **state** from **behavior**;
- define a class and create objects;
- understand the purpose of `private` and `public`;
- explain what encapsulation means;
- identify and protect class invariants;
- use constructors to initialize objects;
- implement meaningful member functions;
- use `const` where appropriate;
- avoid designing classes as collections of getters and setters.

## Table of Contents

1. [From Requirements to Objects](#1-from-requirements-to-objects)
2. [First Activity: Identify State and Behavior](#2-first-activity-identify-state-and-behavior)
3. [Introducing Classes and Objects](#3-introducing-classes-and-objects)
4. [Member Variables and Member Functions](#4-member-variables-and-member-functions)
5. [The Problem with Unrestricted Access](#5-the-problem-with-unrestricted-access)
6. [Encapsulation: Private and Public](#6-encapsulation-private-and-public)
7. [Constructors](#7-constructors)
8. [Class Invariants](#8-class-invariants)
9. [Meaningful Methods vs Getters and Setters](#9-meaningful-methods-vs-getters-and-setters)
10. [`const` Member Functions](#10-const-member-functions)
11. [Objects and Memory](#11-objects-and-memory)
12. [Common Exercise: Design a Product Class](#12-common-exercise-design-a-product-class)
13. [Individual Exercise: Implement and Test](#13-individual-exercise-implement-and-test)
14. [Debugging and Refactoring Challenge](#14-debugging-and-refactoring-challenge)

---

# 1. From Requirements to Objects

Until now, our programs have mainly used variables and functions.

Consider the following program:

```cpp
#include <iostream>
#include <string>

int main()
{
    int id = 1;
    std::string name = "Notebook";
    int stock = 10;

    stock += 5;
    stock -= 3;

    std::cout << name << ": " << stock << '\n';
}
```

This works, but imagine an application managing hundreds of products.

Each product would need its own identifier, name, stock quantity, and operations.

How should we organize this information?

Object-oriented programming provides a way to represent related data and operations together.

Before writing a class, however, we first need to identify what it should represent.

---

# 2. First Activity: Identify State and Behavior

**Approximately 10 minutes — no coding.**

Consider these requirements:

> A store manages products. Every product has a unique identifier, a name, and an available stock quantity.
>
> The store can receive additional units of a product and sell existing units.
>
> A sale should only succeed if enough units are available.
>
> A product's stock quantity must never become negative.

## Part A: Identify the Object

What real-world concept are we trying to represent?

What information would we need to describe one instance of this concept?

## Part B: State vs Behavior

We can divide the requirements into two categories.

- **State:** information describing the current condition of an object.
- **Behavior:** actions an object can perform or operations that can affect its state.

Complete the table:

| Requirement | State or behavior? |
|---|---|
| Product identifier | |
| Product name | |
| Available stock | |
| Receive additional units | |
| Sell a quantity | |
| Read the available stock | |

## Part C: Identify Rules

Consider the following situations:

1. A product has 10 units available. A customer buys 3.
2. A product has 5 units available. Someone attempts to buy 8.
3. Someone attempts to receive `-4` units.
4. Someone tries to assign `-100` directly to the stock quantity.

Which situations should be allowed?

Which should be rejected?

**Keep your answers.** We will use them when designing our first class.

---

# 3. Introducing Classes and Objects

A **class** defines a new type that groups related data and operations.

An **object** is an instance of that class.

For example:

```cpp
class Counter
{
public:
    int value = 0;
};
```

We can create objects of this type:

```cpp
Counter first;
Counter second;
```

Each object has its own state:

```cpp
first.value = 10;
second.value = 20;
```

Conceptually:

```text
       Counter class
             │
        creates objects
             │
      ┌──────┴──────┐
      ▼             ▼
    first         second
  ┌────────┐    ┌────────┐
  │value=10│    │value=20│
  └────────┘    └────────┘
```

A class is the description of a type. The objects are individual instances of that type.

For now, `public` simply allows code outside the class to access `value`. We will explore access control shortly.

---

# 4. Member Variables and Member Functions

A class can contain:

- **Member variables**, which represent the state of an object.
- **Member functions**, which represent operations associated with the object.

For example:

```cpp
#include <iostream>

class Counter
{
public:
    int value = 0;

    void increase()
    {
        value++;
    }

    void decrease()
    {
        value--;
    }

    void print()
    {
        std::cout << value << '\n';
    }
};

int main()
{
    Counter counter;

    counter.increase();
    counter.increase();
    counter.decrease();

    counter.print();
}
```

The `.` operator is used to access members of an object.

```cpp
counter.increase();
```

Unlike an ordinary function that receives a counter as an argument, `increase()` is a member function called on a particular object.

## Predict the Output

```cpp
Counter a;
Counter b;

a.increase();
a.increase();
b.increase();

a.print();
b.print();
```

Before running it, answer:

1. How many objects exist?
2. Which object is modified by each call?
3. Do the objects share the same `value` variable?

---

# 5. The Problem with Unrestricted Access

Return to the product requirements.

Suppose we write:

```cpp
class Product
{
public:
    int id;
    int stock;
};
```

External code can now do this:

```cpp
Product product;

product.id = 1;
product.stock = 10;
```

But it can also do this:

```cpp
product.stock = -100;
```

The program compiles, even though the object's state violates our requirements.

The problem is that any part of the program can change the data without respecting the rules.

**Discussion:** If a product's stock must never be negative, should other parts of the program be allowed to change it directly?

This motivates one of the central concepts of object-oriented programming: **encapsulation**.

---

# 6. Encapsulation: Private and Public

Encapsulation means controlling how an object's internal state can be accessed and modified.

C++ provides access specifiers:

| Access specifier | Meaning |
|---|---|
| `private` | Members are accessible within the class and by other specially authorized code, not by ordinary external code |
| `public` | Members can be accessed by external code |

Consider:

```cpp
class Counter
{
private:
    int value = 0;

public:
    void increase()
    {
        value++;
    }

    int getValue() const
    {
        return value;
    }
};
```

Now:

```cpp
Counter counter;

counter.increase();
```

is allowed.

But:

```cpp
counter.value = -100;
```

is not allowed, because `value` is private.

The class decides which operations are permitted.

Conceptually:

```text
       Outside the class
               │
               ▼
     ┌───────────────────┐
     │ PUBLIC INTERFACE  │
     │                   │
     │ increase()        │
     │ getValue()        │
     ├───────────────────┤
     │ PRIVATE STATE     │
     │                   │
     │ value             │
     └───────────────────┘
```

The public interface defines how the rest of the program interacts with an object.

The private implementation contains details that should not be manipulated directly.

> [!IMPORTANT]
> **Making data private is not enough by itself.**
>
> A class is well-encapsulated when its public operations also respect the rules of the object.

## Revisit the First Activity

For our `Product`:

1. Which member variables should be private?
2. Which operations should be public?
3. What problems could arise if stock could be modified directly?
4. What should happen when an operation receives an invalid quantity?

---

# 7. Constructors

When creating an object, we often need to provide initial values.

A **constructor** is a special member function used to initialize an object.

It has the same name as the class and no return type.

```cpp
#include <string>

class Product
{
private:
    int id;
    std::string name;
    int stock;

public:
    Product(int productId, const std::string& productName, int initialStock)
        : id(productId), name(productName), stock(initialStock)
    {
    }

    int getStock() const
    {
        return stock;
    }
};
```

We can create a product using:

```cpp
Product notebook(1, "Notebook", 10);
```

The constructor receives the values needed to initialize the object.

The syntax:

```cpp
: id(productId), name(productName), stock(initialStock)
```

is called a **member initializer list**.

It directly initializes the member variables.

## Important Question

What happens if we write:

```cpp
Product notebook(1, "Notebook", -10);
```

The constructor above accepts this value.

Although the fields are private, we have still created an invalid product.

We therefore need to think about the **rules that must always hold for an object**.

---

# 8. Class Invariants

A **class invariant** is a condition that must hold whenever an object is in a valid, externally observable state.

For our `Product`, an invariant is:

```text
stock >= 0
```

This rule should hold:

- after construction;
- after receiving new stock;
- after selling units;
- after any other public operation.

Consider a method for selling products:

```cpp
bool sell(int quantity)
{
    if (quantity <= 0 || quantity > stock)
    {
        return false;
    }

    stock -= quantity;
    return true;
}
```

This method only changes the stock when the quantity is valid.

For example:

```text
Before sale: stock = 10

sell(3)
    ↓
stock = 7
```

But:

```text
Before sale: stock = 5

sell(8)
    ↓
operation rejected
    ↓
stock remains 5
```

Notice that validation is part of the object's behavior, not something that every caller must remember to implement.

The constructor also needs a policy for invalid input. One possible approach is rejecting it with an exception; another is using a factory that reports failure. The important point is that the class must not silently allow invalid state.

> [!IMPORTANT]
> A class should establish valid state when an object is created and preserve that state through its public operations.

---

# 9. Meaningful Methods vs Getters and Setters

A common approach when first learning classes is to create a getter and setter for every member variable.

For example:

```cpp
void setStock(int newStock)
{
    stock = newStock;
}
```

But this permits:

```cpp
product.setStock(-100);
```

Even if we add validation, a more important design question remains:

**Why is the stock changing?**

Compare:

```cpp
product.setStock(15);
```

with:

```cpp
product.restock(5);
```

and:

```cpp
product.sell(3);
```

The second design communicates the purpose of each operation.

It also allows different rules to be enforced for different actions.

## Discussion

For the `Product` class, consider:

- Should the identifier have a setter?
- Should the name be changeable?
- Should stock have a general-purpose setter?
- Is `sell()` a more meaningful operation than `setStock()`?
- Does every private variable necessarily need a getter?

Getters and setters can be useful. The goal is to avoid adding them mechanically without considering the object's responsibilities.

---

# 10. `const` Member Functions

Some member functions modify the object.

Others only read information.

Consider:

```cpp
class Counter
{
private:
    int value = 0;

public:
    void increase()
    {
        value++;
    }

    int getValue() const
    {
        return value;
    }
};
```

The `const` after the function declaration indicates that the function cannot normally modify the object's non-`mutable` state.

```cpp
int getValue() const
```

We can also call it on a constant object:

```cpp
const Counter counter;

std::cout << counter.getValue();
```

But:

```cpp
counter.increase();
```

will not compile because `increase()` is not a `const` member function.

## Identify the Correct Methods

Which of these should be `const`?

```cpp
int getStock();
bool sell(int quantity);
bool restock(int quantity);
std::string getName();
```

Explain your choices.

---

# 11. Objects and Memory

A class definition does not automatically create an object.

Objects are created when we instantiate the class.

For example:

```cpp
Counter a;
Counter b;
```

These are two distinct objects.

For ordinary local variables, we can draw:

```text
STACK

┌──────────────────────┐
│ main() frame         │
│                      │
│ a                    │
│ └── value = 0        │
│                      │
│ b                    │
│ └── value = 0        │
└──────────────────────┘
```

Each object contains its own non-static member state.

An object can also be dynamically allocated:

```cpp
Counter* p = new Counter;

p->increase();

delete p;
```

Conceptually:

```text
STACK                         HEAP

┌────────────┐               ┌────────────────┐
│ p ─────────┼──────────────►│ Counter        │
└────────────┘               │ value = 1      │
                             └────────────────┘
```

The `->` operator accesses a member through a pointer.

A class does not automatically mean heap allocation.

**The storage location depends on how the object is created.**

---

# 12. Common Exercise: Design a Product Class

Now return to the requirements from the beginning of the seminar.

A product has:

- an identifier;
- a name;
- an available stock quantity.

The program must allow products to be restocked and sold without ever creating negative stock.

## Step 1: Design the State

Identify the member variables and their types.

Which should be private?

## Step 2: Design the Interface

Decide which operations should be public.

Consider:

```cpp
bool sell(int quantity);
bool restock(int quantity);
int getStock() const;
```

Do you need other methods?

## Step 3: Design the Constructor

How should the product be initialized?

What happens if the initial stock is negative?

## Step 4: Identify Invariants

Write down the rules that the class must always preserve.

## Step 5: Write the Class Declaration

Start from:

```cpp
#include <string>

class Product
{
private:

public:

};
```

Fill in the members and methods based on your design decisions.

---

# 13. Individual Exercise: Implement and Test

Implement your `Product` class.

Your program should:

1. Create at least two products.
2. Display their initial state through public methods.
3. Restock one product.
4. Sell several units of a product.
5. Attempt to sell more units than are available.
6. Attempt to use a negative quantity.
7. Verify that invalid operations leave the object unchanged.

## Test Cases

| Operation | Expected behavior |
|---|---|
| Create a product with valid data | Object has the correct initial state |
| Restock 5 units | Stock increases by 5 |
| Sell 3 available units | Stock decreases by 3 |
| Sell more than available | Operation fails, stock unchanged |
| Sell a negative quantity | Operation fails, stock unchanged |
| Restock a negative quantity | Operation fails, stock unchanged |

Test your implementation with more than one object.

Use the debugger to inspect the state before and after method calls.

---

# 14. Debugging and Refactoring Challenge

Consider this implementation:

```cpp
#include <iostream>

class Product
{
public:
    int id;
    int stock;
};

int main()
{
    Product product{1, 10};

    product.stock = -5;

    std::cout << product.stock << '\n';
}
```

The code compiles successfully.

However, it violates one of the requirements.

## Part A: Analyze

1. What rule has been violated?
2. Why does the compiler allow this code?
3. What design decision makes this possible?

## Part B: Refactor

Modify the class so that:

- its internal state is protected;
- it can be constructed in a valid state;
- stock can only change through meaningful operations;
- invalid operations cannot violate the class invariant.

## Part C: Explain

After refactoring, be prepared to answer:

1. What is the responsibility of your class?
2. Which members are private, and why?
3. Which methods are public?
4. What invariant does the class maintain?
5. How does each method preserve that invariant?
6. Why is this design preferable to exposing all member variables publicly?

The objective is not just to make the program compile. It is to design a class whose interface reflects the requirements and protects its own state.
