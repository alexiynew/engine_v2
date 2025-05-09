# C++ Coding Style Guide

This document outlines the rules for writing C++ code to ensure readability, maintainability, and safety.

---

## 1. Naming Conventions

### 1.1. Variables and Non-Static Methods
Use `camelCase` for:
 - Non-static class member variables
 - Non-static class methods
 - Local variables
 - Function parameters
 - Constants (except global constants, see 1.6)

```cpp
// Non-static member variables and methods
class UserManager
{
public:
    void addUser(const std::string& userName); // Method
private:
    int m_userCount; // Member variable
};

// Local variables and parameters
void ProcessRequest(int requestId)
{
    bool isValid = CheckValidity(requestId);
}

// Constants in local/class scope
const int maxRetries = 3;
```

### 1.2. Static Methods and Free Functions
Use `PascalCase` for:
 - Static methods of classes
 - Free functions (non-member functions)
   
This distinguishes them from regular camelCase member methods and emphasizes their broader scope.
```cpp
// Static method  
class Logger {  
public:  
    static void WriteToFile(const std::string& message);  
};  

// Free function  
std::string GenerateUniqueId();  
void InitializeGlobalCache();  

// Template free function
template <typename T>  
T Clamp(T value, T min, T max);  

// Avoid mixing styles:  
void processData();  // ❌ camelCase (incorrect for free functions)  
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
namespace game_engine::graphics;
using namespace game_engine;
```

### 1.4. Class Members
Use the `m_` prefix for class member variables.

```cpp
class User {
private:
    int m_id;
};
```

### 1.5. Static Variables
Use the `s_` prefix for for static variables.

```cpp
static int s_staticValue;
```

### 1.6. Global Constants
Use `CamelCase` for global constants defined in .cpp files.

```cpp
const int MaxUsers = 100;
constexpr double Pi = 3.14159;
const std::string DefaultUsername = "guest";
```

### 1.7. Macros
Use `UPPER_SNAKE_CASE` exclusively for macros.

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

    DataProcessor(int initialSize);  // Constructor  
    ~DataProcessor() override;       // Destructor  

    DataProcessor& operator=(const DataProcessor& other);  // Copy assignment  
    DataProcessor& operator=(DataProcessor&& other) noexcept;  // Move assignment  

    static ResultType DefaultProcessing();  // Static method  

    // Inherited from BaseAlgorithm  
    void initialize() override;  
    void execute() override;  

    // Inherited from Serializable  
    std::string serialize() const override;  

    // Class-specific methods  
    void addDataPoint(double value);  
    void clear();  

private:  
    // Internal types  
    enum class State { Idle, Processing };  

    // Static method  
    static void LogError(const std::string& message);  

    // Data members  
    std::vector<double> m_dataPoints;  
    State m_state = State::Idle;  
    int m_id;  
};
```

### 3.2. Method Definition Order in .cpp Files
Method definitions in the .cpp file must follow the exact order of their declarations in the header.\
Use `#pragma region/#pragma endregion` to visually group related method blocks.\
This is optional but recommended for large classes.

```cpp
#pragma region BaseAlgorithm

void DataProcessor::initialize()
{
    /* ... */
}

void DataProcessor::execute()
{
    /* ... */
}

#pragma endregion

#pragma region Serializable

std::string DataProcessor::serialize() const
{
    /* ... */
}

#pragma endregion

#pragma region Class-specific methods

void DataProcessor::addDataPoint(double value)
{
    /* ... */
}

void DataProcessor::clear()
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
 - Place regions after core components (ctors, operators, static methods)
 - Add empty line between regions for visual separation

