include(FetchContent)

FetchContent_Declare(
    GLFW
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG        3.4
)

FetchContent_MakeAvailable(GLFW)

set_target_properties(glfw uninstall update_mappings PROPERTIES FOLDER "libs/glfw3")

# Define lids to link
add_library(Libs::glfw INTERFACE IMPORTED)
target_link_libraries(Libs::glfw INTERFACE glfw)
target_include_directories(Libs::glfw INTERFACE ${GLFW_SOURCE_DIR}/include)
