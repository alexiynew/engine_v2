include(FetchContent)

FetchContent_Declare(
	glm
	GIT_REPOSITORY https://github.com/g-truc/glm.git
	GIT_TAG        1.0.1
)

set(GLM_ENABLE_CXX_17 ON CACHE BOOL "" FORCE)
set(GLM_ENABLE_CXX_20 ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(glm)

set(GLM_GCC_CUSTOM_COMPILE_OPTIONS -Wno-unsafe-buffer-usage)
set(GLM_MSVC_CUSTOM_COMPILE_OPTIONS )

target_compile_options(glm 
	PRIVATE 
        $<${GCC_LIKE_COMPILER}:${GLM_GCC_CUSTOM_COMPILE_OPTIONS}>
        $<${MSVC_LIKE_COMPILER}:${GLM_MSVC_CUSTOM_COMPILE_OPTIONS}>
)

set_target_properties(glm PROPERTIES FOLDER "engine/third_party/glm")

# Define lids to link
add_library(third_party::glm INTERFACE IMPORTED)
target_link_libraries(third_party::glm INTERFACE glm)
target_include_directories(third_party::glm INTERFACE ${glm_SOURCE_DIR})
