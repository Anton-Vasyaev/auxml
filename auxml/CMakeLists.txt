cmake_minimum_required(VERSION 3.19)

project(auxml)
set(TARGET_NAME auxml)
set(TARGET_TYPE SHARED)

set(CMAKE_VERBOSE_MAKEFILE TRUE)
set(CMAKE_CXX_STANDARD 20)

if(MSVC)
    add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
endif()

if(TARGET_TYPE STREQUAL SHARED)
    set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
endif()

# conan settings **********************************************************************************
set(CONAN_DIR "")
if(DEFINED CREATE_PACKAGE)
    set(CONAN_DIR ${CMAKE_BINARY_DIR}/..)
    message($"CONAN CREATE CONAN DIR:${CONAN_DIR}")
else()
    set(CONAN_DIR ${CMAKE_CURRENT_LIST_DIR}/../.main_cache/conan/x64/${CMAKE_BUILD_TYPE})
endif()
set(CMAKE_PREFIX_PATH ${CONAN_DIR})
set(CMAKE_MODULE_PATH ${CONAN_DIR})


# configure project
file(GLOB_RECURSE SOURCE_LIST "src/*.c" "src/*.cpp")

include_directories(include)

add_library(${TARGET_NAME} ${TARGET_TYPE} ${SOURCE_LIST})

target_include_directories(${TARGET_NAME} PUBLIC include)
target_include_directories(${TARGET_NAME} PRIVATE src)

# dependencies
find_package(Callisto)
find_package(Boost)

target_link_libraries(${TARGET_NAME} PUBLIC Boost::headers)
target_link_libraries(${TARGET_NAME} PUBLIC Callisto::framework)
target_link_libraries(${TARGET_NAME} PUBLIC Callisto::math)