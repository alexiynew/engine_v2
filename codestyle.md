# C++ Coding Style Guide

This document outlines the rules for writing C++ code to ensure readability, maintainability, and safety.

---

## 1. Naming Conventions

### 1.1. Variables and Functions
Use `snake_case` for variables, constants and functions.

```cpp
int user_age;
void calculate_sum();
```

### 1.2. Classes and Structs
Use `PascalCase` for classes, structs, enums, unions and any type declarations.

```cpp
class UserProfile;
struct DataPoint;
enum Directions;
```

### 1.3. Class Members
Use the m_ prefix for class member variables.

```cpp
class User {
private:
    int m_id;
};
```

### 1.4. Global and Static Variables
Use the g_ prefix for global variables and s_ for static variables.

```cpp
int g_global_counter;
static int s_static_value;
```

### 1.5. Macros
Use UPPER_SNAKE_CASE exclusively for macros.

```cpp
#define MAX(a, b) ((a) > (b) ? (a) : (b))
```

## 2. Comments
### 2.1. Documentation
Use Doxygen-style comments for documenting functions and classes.

```cpp
/**
 * @brief Calculates the sum of two numbers.
 * @param a The first number.
 * @param b The second number.
 * @return The sum of a and b.
 */
int calculate_sum(int a, int b);
```
