# C++ Coding Style Guide

This document outlines the rules for writing C++ code to ensure readability, maintainability, and safety.

---

## 1. Naming Conventions

### 1.1. Namespaces

Use `snake_case` for namespace names.\
Use `using namespace ...` only in cpp files.

```cpp
namespace game_engine::graphics_interface 
{}

// in cpp file
using namespace game_engine;
```

### 1.2. Variables, arguments and parameters

Use `snake_case` for:

- Local variables
- Function parameters
- Local constants

```cpp
// Non-static member variables and methods
class UserManager
{
public:
    void AddUser(const std::string& user_name); // Parameter
private:
    int m_user_count; // Member variable
};

// Local variables and parameters
void ProcessRequest(int request_id)
{
    bool is_valid = CheckValidity(request_id);
}

// Constants in local/class scope
void foo() {
    const int max_retries = 3;
}
```

### 1.3. Class Members

Use `snake_case` and the `m_` prefix for class member variables.\
Use `snake_case` for structure member variables. (no `m_` prefix)

```cpp
class User {
private:
    int m_id;
};

struct Data {
    int value;
    int another_value;
};
```

### 1.4. Static Variables

Use the `s_` prefix for non-const local static variables.

```cpp
class Foo;
Foo& Foo::Instance() 
{
    static Foo s_instance;
    return s_instance;
}
```

### 1.5. Classes, Structs and Template Parameters

Use `PascalCase` for classes, structs, enums, unions and any type declarations.\
Use the `T` prefix for template parameters.

```cpp
class UserProfile;
struct DataPoint;
enum Directions { Up, Down, Left, Right };

template <typename TValue>
class Foo;
```

### 1.6. Methods, Static methods and Free functions

Use `PascalCase` for:

- Methods of classes
- Static methods of classes
- Free functions (non-member functions)

```cpp
// Static method  
class Logger {  
public:  
    static void WriteToFile(const std::string& message);  
    
    void Init(const std::string& filepath);  
    void Shutdown();  
};  

// Free function  
std::string GenerateUniqueId();  
void InitializeGlobalCache();  

// Template free function
template <typename T>  
T Clamp(T value, T min, T max);  

// Avoid mixing styles:  
void processData();  // ❌ camelCase incorrect  
```

### 1.7. Global Constants

Use `PascalCase` for:

- Global constants
- In class constants

```cpp
const int MaxUsers = 100;
constexpr double Pi = 3.14159;
const std::string DefaultUsername = "guest";

class Foo
{
public:
    static const int Value;
};

const int Foo::Value = 42;
```

### 1.8. Macros

⚠️ Use `UPPER_SNAKE_CASE` exclusively for macros.

```cpp
#define MAX_MACRO(a, b) ((a) > (b) ? (a) : (b))
```

## 2. Comments

### 2.1. Documentation

Use Doxygen-style comments for documenting functions and classes.\
Use `///` for documentation comment.\
Use `@` for documentation parameter.\
Use `///<` for inplace documentation.

```cpp
/// @brief Calculates the sum of two numbers.
/// @param a The first number.
/// @param b The second number.
/// @return The sum of a and b.
int calculateSum(int a, int b);

struct Data {
    std::vector<int> data; ///< Some actual data.
}
```

## 3. Class Declaration Order

### 3.1. Class Section Order

Declare class sections in the following order:

- `public`
- `protected`
- `private`

Within each section, use the following order:

- `Internal types` (e.g., using aliases, nested classes/enums)
- `Constructors and destructor`
- `Copy/move operators` (e.g., operator=)
- `Static methods`
- `Overridden methods` (grouped by base class, with a comment indicating the base class name)
- `Class-specific methods`
- `Static data members` (if declared in this section)
- `Data members` (if declared in this section)

```cpp
class DataProcessor
    : public BaseAlgorithm
    , public Serializable
{  
public:  
    using ResultType = std::vector<double>;  // Internal type  

    DataProcessor(int initial_size);  // Constructor  
    ~DataProcessor() override;        // Destructor  

    DataProcessor& operator=(const DataProcessor& other);      // Copy assignment  
    DataProcessor& operator=(DataProcessor&& other) noexcept;  // Move assignment  

    static ResultType DefaultProcessing();  // Static method  

    // Inherited from BaseAlgorithm  
    void Initialize() override;  
    void Execute() override;  

    // Inherited from Serializable  
    std::string Serialize() const override;  

    // Class-specific methods  
    void AddDataPoint(double value);  
    void Clear();  

private:  
    // Internal types  
    enum class State { Idle, Processing };  

    // Static method  
    static void LogError(const std::string& message);  

    // Data members  
    std::vector<double> m_data_points;
    State m_state = State::Idle;  
    int m_id;  
};
```

### 3.2. Method Definition Order in .cpp Files

Method definitions in the .cpp file must follow the exact order of their declarations in the header.\
Use `#pragma region/#pragma endregion` to visually group related method blocks.\
This is optional but recommended for large classes. Allowed to use it, despite compiler support.

```cpp
#pragma region BaseAlgorithm

void DataProcessor::Initialize()
{
    /* ... */
}

void DataProcessor::Execute()
{
    /* ... */
}

#pragma endregion

#pragma region Serializable

std::string DataProcessor::Serialize() const
{
    /* ... */
}

#pragma endregion

#pragma region Class-specific methods

void DataProcessor::AddDataPoint(double value)
{
    /* ... */
}

void DataProcessor::Clear()
{
    /* ... */
}

#pragma endregion

#pragma region Private helpers

void DataProcessor::LogError(const std::string& message)
{
    /* ... */
}

#pragma endregion
```

Important notes:

- Regions must never alter the header declaration order
- Use clear region names matching base class/functionality
- Place regions after core components (constructors, operators, static methods)
- Add empty line between regions for visual separation

