## Suitable choices for your temperature class

| Operator | Suggested requirement | Example call | Difficulty |
| --- | --- | --- | --- |
| `-=` | Subtract an offset from every reading; return the current object by reference | `month -= 2;` | Easy |
| `-` | Return a copy with an offset subtracted; leave the original unchanged | `MonthlyTemperatures colder = month - 2;` | Easy |
| `!=` | Return the opposite of `==` | `bool different = first != second;` | Very easy |
| Prefix `--` | Lower every reading by one; return the updated object by reference | `MonthlyTemperatures updated = --month;` | Easy |
| Postfix `--` | Lower every reading by one; return the previous state by value | `MonthlyTemperatures previous = month--;` | Medium |
| `!` | Return whether the object has no readings | `bool empty = !month;` | Easy |
| `>` | Compare average temperatures; reject comparison if either object is empty | `bool warmer = first > second;` | Medium |
| `<` | Compare average temperatures using the same rules | `bool colder = first < second;` | Medium |
| `()` | Return the average temperature; reject an empty object | `double average = month();` | Medium |
