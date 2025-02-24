include(FetchContent)

FetchContent_Declare(
	GLM
	GIT_REPOSITORY https://github.com/g-truc/glm.git
	GIT_TAG        1.0.1
	PATCH_COMMAND ${CMAKE_COMMAND} -P ${CMAKE_SOURCE_DIR}/libs/glm_patch.cmake ${CMAKE_SOURCE_DIR}/build/_deps/glm-src
)

set(GLM_ENABLE_CXX_17 ON CACHE BOOL "" FORCE)
set(GLM_ENABLE_CXX_20 ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(GLM)

target_compile_options(glm PRIVATE "-Wno-unsafe-buffer-usage")
target_compile_options(glm PRIVATE "-Wno-sign-conversion")

set_target_properties(glm PROPERTIES FOLDER "libs/glm")

# Define lids to link
add_library(Libs::glm INTERFACE IMPORTED)
target_link_libraries(Libs::glm INTERFACE glm)
target_include_directories(Libs::glm INTERFACE ${GLM_SOURCE_DIR})

