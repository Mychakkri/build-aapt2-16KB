cmake_minimum_required(VERSION 3.14.2)
project(aapt2)

# Architecture
if(ANDROID_ABI STREQUAL "arm64-v8a" OR ANDROID_ABI STREQUAL "armeabi-v7a")
    enable_language(ASM)
else()
    message(FATAL_ERROR "Unsupported architecture: ${ANDROID_ABI}")
endif()

# Global flags
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fcolor-diagnostics -fPIC -std=c11")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fcolor-diagnostics -fPIC -std=c++17")

# Paths
set(SRC_PATH ${PROJECT_SOURCE_DIR}/src)
set(TOOLS_VERSION "33.0.3")
set(PROTOC_COMPILER "tools/protoc")

# Dependencies

# --- Zopfli as STATIC library ---
add_subdirectory(src/zopfli EXCLUDE_FROM_ALL)
# src/zopfli/CMakeLists.txt ต้องแก้:
# add_executable(zopfli ...) -> add_library(zopfli STATIC ${ZOPFLI_SOURCES})
# target_include_directories(zopfli PUBLIC ${SRC_PATH}/zopfli)

add_subdirectory(src/fmtlib EXCLUDE_FROM_ALL)
add_subdirectory(src/googletest EXCLUDE_FROM_ALL)
add_subdirectory(cmake)

# Source files
file(GLOB_RECURSE AAPT2_SOURCES
    ${SRC_PATH}/androidfw/*.cpp
    ${SRC_PATH}/base/*.cpp
    ${SRC_PATH}/cmd/aapt2/*.cpp
    ${SRC_PATH}/config/*.cpp
    ${SRC_PATH}/idmap2/*.cpp
    ${SRC_PATH}/libziparchive/*.cpp
    ${SRC_PATH}/process/*.cpp
    ${SRC_PATH}/util/*.cpp
)

# Build shared library
add_library(aapt2 SHARED ${AAPT2_SOURCES})

# Include dirs
target_include_directories(aapt2 PRIVATE
    ${ANDROID_NDK}/toolchains/llvm/prebuilt/linux-x86_64/sysroot/usr/include
    ${NDK_SYSROOT}/usr/include
    ${SRC_PATH}
    ${SRC_PATH}/androidfw
    ${SRC_PATH}/aapt2
    ${SRC_PATH}/libbase/include
    ${SRC_PATH}/libsystem/include
    ${SRC_PATH}/libutils/include
    ${SRC_PATH}/liblog/include
    ${SRC_PATH}/androidfw/include
    ${SRC_PATH}/libidmap2_policies/include
    ${SRC_PATH}/libziparchive/include
    ${SRC_PATH}/incfs/util/include
    ${SRC_PATH}/googletest/googletest/include
    ${SRC_PATH}/protobuf/src
    ${SRC_PATH}/expat/lib
    ${SRC_PATH}/fmtlib/include
    ${SRC_PATH}/libpng
    ${SRC_PATH}/zlib
    ${SRC_PATH}/cutils
    ${SRC_PATH}/binder
    fake_headers
)

# Android define
target_compile_definitions(aapt2 PRIVATE ANDROID)

# Disable warnings as errors
target_compile_options(aapt2 PRIVATE
    -Wno-reorder-init-list
    -Wno-c99-designator
)

# Link dependencies with keyword signature
target_link_libraries(aapt2
    PRIVATE zopfli
    PRIVATE fmt
)

# 16KB page size
target_link_options(aapt2 PRIVATE "-Wl,-z,max-page-size=16384")
