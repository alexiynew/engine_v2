include(FetchContent)

FetchContent_Declare(
	GLM
	GIT_REPOSITORY https://github.com/g-truc/glm.git
	GIT_TAG        1.0.1
)

set(GLM_ENABLE_CXX_17 ON CACHE BOOL "" FORCE)
set(GLM_ENABLE_CXX_20 ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(GLM)

FetchContent_GetProperties(GLM)
if(GLM_POPULATED)
    execute_process(
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_SOURCE_DIR}/libs/glm_patch.cmake ${GLM_SOURCE_DIR}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
endif()

set(GLM_GCC_CUSTOM_COMPILE_OPTIONS -Wno-unsafe-buffer-usage)
set(GLM_MSVC_CUSTOM_COMPILE_OPTIONS )

target_compile_options(glm 
	PRIVATE 
        $<${GCC_LIKE_COMPILER}:${GLM_GCC_CUSTOM_COMPILE_OPTIONS}>
        $<${MSVC_LIKE_COMPILER}:${GLM_MSVC_CUSTOM_COMPILE_OPTIONS}>
)

set_target_properties(glm PROPERTIES FOLDER "libs/glm")

# Define lids to link
add_library(Libs::glm INTERFACE IMPORTED)
target_link_libraries(Libs::glm INTERFACE glm)
target_include_directories(Libs::glm INTERFACE ${GLM_SOURCE_DIR})
