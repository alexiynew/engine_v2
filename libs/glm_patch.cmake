math(EXPR LAST_ARG_NUM "${CMAKE_ARGC}-1") 
set(GML_PATCH_SOURCE_DIR ${CMAKE_ARGV${LAST_ARG_NUM}})

file(READ ${GML_PATCH_SOURCE_DIR}/CMakeLists.txt GLM_PATH_CMAKE)

string(REPLACE 
    "cmake_minimum_required(VERSION 3.6 FATAL_ERROR)" 
    "cmake_minimum_required(VERSION 3.6...3.14 FATAL_ERROR)" 
    GLM_PATH_CMAKE 
    "${GLM_PATH_CMAKE}"
)

string(REPLACE 
    "cmake_policy(VERSION 3.6)" 
    "cmake_policy(VERSION 3.6...3.14)" 
    GLM_PATH_CMAKE 
    "${GLM_PATH_CMAKE}"
)

file(WRITE ${GML_PATCH_SOURCE_DIR}/CMakeLists.txt "${GLM_PATH_CMAKE}")
