# C++ Coding Style Guide

This document outlines the rules for writing C++ code to ensure readability, maintainability, and safety.

---

## 1. Naming Conventions

### 1.1. Variables and Functions
Use `camelCase` for variables, constants and functions.

```cpp
int userAge;
void calculateSum();
```

### 1.2. Classes and Structs
Use `PascalCase` for classes, structs, enums, unions and any type declarations.

```cpp
class UserProfile;
struct DataPoint;
enum Directions;
```

### 1.3. Namespaces
Use `snake_case` for namespace names.

```cpp
namespace game_engine::core;
using namespace game_engine;
```

### 1.4. Class Members
Use the m_ prefix for class member variables.

```cpp
class User {
private:
    int m_id;
};
```

### 1.5. Static Variables
Use the s_ prefix for for static variables.

```cpp
static int s_staticValue;
```

### 1.6. Global Constants
Use CamelCase for global constants defined in .cpp files.

```cpp
const int MaxUsers = 100;
constexpr double Pi = 3.14159;
const std::string DefaultUsername = "guest";
```

### 1.7. Macros
Use UPPER_SNAKE_CASE exclusively for macros.

```cpp
#define MAX_MACRO(a, b) ((a) > (b) ? (a) : (b))
```

## 2. Comments
### 2.1. Documentation
Use Doxygen-style comments for documenting functions and classes.

```cpp
/// @brief Calculates the sum of two numbers.
/// @param a The first number.
/// @param b The second number.
/// @return The sum of a and b.
int calculateSum(int a, int b);

struct Data {
    int std::vector<int> data ///< Some actual data.
}
```
