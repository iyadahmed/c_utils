cmake_minimum_required(VERSION 3.16)
project(c_utils)

add_library(c_utils INTERFACE
    include/allocator.h
    include/bitwise.h
    include/clz.h
    include/common.h
    include/list.h
    include/map.h
    include/murmur_hash.h
    include/siphash.h
    include/siphash_half.h
    include/small_array.h
    include/utf8.h
)
target_include_directories(c_utils INTERFACE include)
set_target_properties(c_utils PROPERTIES
    LANGUAGE C
    CMAKE_C_STANDARD 99
    CMAKE_C_STANDARD_REQUIRED YES
    CMAKE_C_EXTENSIONS NO
)

add_executable(example example.c)
target_link_libraries(example c_utils)