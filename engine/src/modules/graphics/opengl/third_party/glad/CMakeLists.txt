
# Set the project name
project(glad VERSION 1.0.0 LANGUAGES C)

# Add the library
add_library(glad STATIC src/glad.c)

set_target_properties(glad PROPERTIES C_STANDARD 99)

set_target_properties(glad PROPERTIES FOLDER "engine/third_party")
target_include_directories(glad PUBLIC include)

add_library(third_party::glad ALIAS glad)
