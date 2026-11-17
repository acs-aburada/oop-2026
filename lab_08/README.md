# Week 8: Copying, Moving, and Operators

Start with [temperatures_start.cpp](temperatures_start.cpp). The completed program is in [temperatures.cpp](temperatures.cpp). Use C++11 or later.

## 1. Monthly Temperatures

Store a location, thermometer brand, heating price, and daily temperatures:

```cpp
enum class Brand { GENERIC, OMRON, BEURER };

// Inside the class:
std::string location = "Unknown";
Brand brand = Brand::GENERIC;
float heatingPrice = 0.0F;
double* temperatures = nullptr;
int dayCount = 0;
static unsigned int objectCount;
```

The class owns the array. It must allocate, copy, and release it. An empty object has a null pointer and zero readings. A month can have up to 31 readings.

`heatingPrice` is non-negative. Temperature offsets do not change this price.

`objectCount` counts currently existing objects. Each successful constructor, including copy and move constructors, increments it. The destructor decrements it. Assignment does not create an object, so it does not change the count.

The shared counter is defined once outside the class:

```cpp
unsigned int MonthlyTemperatures::objectCount = 0;
```

Add a constructor that receives the readings, their count, a location, a brand, and a heating price:

```cpp
double readings[] = {18, 20, 19};
MonthlyTemperatures month(readings, 3, "Bucharest", Brand::OMRON, 2.5F);
```

The constructor must copy the values into its own allocation, not keep a pointer to `readings`.

The destructor releases the array using `delete[]`.

The headers are `<iostream>` for console streams, `<string>` for `std::string`, and `<utility>` for `std::move`. Invalid arguments use string-literal throws, such as `throw "Invalid day index.";`, caught with `catch (const char* message)`.

## 2. Copy Constructor and Assignment

```cpp
MonthlyTemperatures first(readings, 3);
MonthlyTemperatures second(first);    // Copy constructor
MonthlyTemperatures third = first;   // Copy constructor

third = first;                       // Copy assignment
```

Construction creates a new object. Assignment changes an existing object.

Copying only the pointer would make both objects share the same allocation. Both destructors would then try to delete it.

A deep copy creates a separate array:

```text
first.temperatures  ----> [18][20][19]
second.temperatures ----> [18][20][19]
```

The example uses `setReadings` to allocate and copy the values:

```cpp
MonthlyTemperatures(const MonthlyTemperatures& other)
    : location(other.location), brand(other.brand), heatingPrice(other.heatingPrice)
{
    setReadings(other.temperatures, other.dayCount);
    objectCount++;
}
```

> [!IMPORTANT]
> `const MonthlyTemperatures& other` refers to the existing source without copying it as a parameter. `const` prevents changing the source through this reference. The constructor still creates a separate array for the new object.

Assignment also needs to replace the destination's previous array:

```cpp
MonthlyTemperatures& operator=(const MonthlyTemperatures& other)
{
    if (this != &other)
    {
        std::string copiedLocation = other.location;
        setReadings(other.temperatures, other.dayCount);
        location = std::move(copiedLocation);
        brand = other.brand;
        heatingPrice = other.heatingPrice;
    }
    return *this;
}
```

The location is copied before replacing the readings. `setReadings` prepares the new array before deleting the old one. Allocation failure during these preparations leaves the destination unchanged.

`this != &other` checks for self-assignment. Here, `&other` takes an address; it does not declare a reference.

> [!IMPORTANT]
> The return type `MonthlyTemperatures&` refers to the existing destination, not a new copy. `return *this` returns that object and allows `a = b = c`. The parameter's `const &` protects the source, not the destination.

## 3. Move Constructor and Assignment

Moving transfers ownership of the existing array:

```cpp
MonthlyTemperatures(MonthlyTemperatures&& other) noexcept
    : location(std::move(other.location)), brand(other.brand),
      heatingPrice(other.heatingPrice),
      temperatures(other.temperatures), dayCount(other.dayCount)
{
    other.location.clear();
    other.brand = Brand::GENERIC;
    other.heatingPrice = 0.0F;
    other.temperatures = nullptr;
    other.dayCount = 0;
    objectCount++;
}
```

```cpp
MonthlyTemperatures destination(std::move(source)); // Move constructor
existing = std::move(destination);                  // Move assignment
```

> [!IMPORTANT]
> `MonthlyTemperatures&& other` is an rvalue-reference parameter. It refers to the source rather than copying it, and is not `const` because moving resets that source. Both move construction and move assignment use this parameter type.

`std::move`, from `<utility>`, allows a move operation to be selected; it does not transfer the array itself.

The move constructor takes the pointer and resets the source so that its destructor cannot delete the transferred array. `noexcept` states that this operation does not throw.

Move assignment does the same transfer, but first releases the destination's previous array.

In this class, moved-from objects have an empty location, a generic brand, zero heating price, and no readings. They remain valid and can be reused with `setReadings`.

The destructor, copy constructor, copy assignment, move constructor, and move assignment form the **Rule of Five** for classes directly managing resources.

## 4. Adding an Offset

```cpp
month += 2; // [18,20,19] becomes [20,22,21]
```

```cpp
MonthlyTemperatures& operator+=(double offset)
{
    for (int i = 0; i < dayCount; i++)
    {
        temperatures[i] += offset;
    }
    return *this;
}
```

`+=` changes the current object. `+` creates a changed copy:

```cpp
MonthlyTemperatures operator+(const MonthlyTemperatures& month, double offset)
{
    MonthlyTemperatures result(month);
    result += offset;
    return result;
}
```

```cpp
MonthlyTemperatures warmer = month + 5;
```

> [!IMPORTANT]
> `+=` returns `MonthlyTemperatures&` because it changes and returns the existing object. `+` receives its source by `const &` to avoid copying the parameter, then returns a new result by value. Returning a reference to its local `result` would leave a dangling reference.

`+=` is a member: its left operand is the current object. `+` is a non-member: both operands are explicit parameters. It uses public operations, so it does not need friendship.

## 5. Friend Functions and Other Operators

A friend function is a non-member allowed to access private data. Declare it inside the class and define it outside:

```cpp
friend bool operator==(const MonthlyTemperatures& left,
                       const MonthlyTemperatures& right);
```

Equality compares the location, brand, heating price, reading counts, and each stored value, not the array addresses. This example uses exact equality, suitable for checking unchanged copies. The shared object counter is not part of equality.

> [!IMPORTANT]
> Both equality operands use `const &`: comparing them should neither copy nor modify them. The result is a `bool` value, not a reference.

`friend` does not make the function a member. It has no `this` pointer.

The member `operator[]` reads one temperature:

```cpp
std::cout << month[0]; // First reading
```

The completed example checks that the index is valid and returns the temperature by value.

## 6. Prefix and Postfix Increment

Both forms add one degree to every reading.

Prefix returns the updated object by reference:

```cpp
MonthlyTemperatures& operator++()
{
    *this += 1;
    return *this;
}
```

Postfix returns a copy of the previous state:

```cpp
MonthlyTemperatures operator++(int)
{
    MonthlyTemperatures previous(*this);
    ++(*this);
    return previous;
}
```

The unused `int` distinguishes postfix from prefix. It is not an offset.

> [!IMPORTANT]
> Prefix returns a reference to the updated object. Postfix returns the saved previous state by value: its local `previous` object cannot safely be returned by reference.

Starting with `[18,20,19]`:

```cpp
MonthlyTemperatures before = month++; // before: [18,20,19]; month: [19,21,20]
MonthlyTemperatures after = ++month;  // after: [20,22,21]; month: [20,22,21]
```

## 7. Console Input and Output

Declare these non-member operators as friends:

```cpp
friend std::ostream& operator<<(std::ostream& out,
                               const MonthlyTemperatures& month);
friend std::istream& operator>>(std::istream& in,
                               MonthlyTemperatures& month);
```

> [!IMPORTANT]
> Output receives the month by `const &` to display it without copying or changing it. Input receives it by non-const `&` so it can replace the caller's object.
>
> Streams are also passed by non-const reference: reading or writing changes the existing stream, and stream objects cannot be copied. Returning that same stream by reference allows chaining.

```cpp
std::cout << first << '\n' << second << '\n';
std::cin >> first >> second;
```

Input is a one-word location, brand number, heating price, count, and temperatures. Brand numbers are `0` for GENERIC, `1` for OMRON, and `2` for BEURER:

```text
Bucharest 1 2.5 3 18 20 19
```

The input operator reads into a temporary object and replaces the destination only when all readings succeed.

Keep prompts and error messages in `main`:

```cpp
std::cout << "Enter location, brand number, heating price, day count, then temperatures:\n";
if (!(std::cin >> month))
{
    std::cerr << "Invalid input.\n";
    return 1;
}
std::cout << month << '\n';
```

On invalid input, the destination remains unchanged. Input already consumed is not restored.

## 8. Exercises

1. Copy a month, add an offset to the copy, and check that the original is unchanged.
2. Assign into an object that already owns an array of a different size.
3. Move a month and display both source and destination.
4. Compare the results of `month++` and `++month`.
5. Test empty objects, self-assignment, and invalid indices.
6. Read `Bucharest 1 2.5 0`, `Cluj 2 1.5 3 -2 0 4.5`, a negative price, an invalid brand or count, and non-numeric readings.
7. Add `operator!=` using `operator==`.
8. Check that construction increases the live-object counter, assignment leaves it unchanged, and destruction decreases it.
