cmake_minimum_required(VERSION 2.8.11)
project(Zopfli)

# Check if Zopfli is the top-level project
set(zopfli_standalone FALSE)
get_directory_property(zopfli_parent_directory PARENT_DIRECTORY)
if(zopfli_parent_directory STREQUAL "")
  set(zopfli_standalone TRUE)
endif()
unset(zopfli_parent_directory)

# Options
set(zopfli_shared_default OFF)
if(DEFINED BUILD_SHARED_LIBS)
  set(zopfli_shared_default ${BUILD_SHARED_LIBS})
endif()
option(ZOPFLI_BUILD_SHARED "Build Zopfli with shared libraries" ${zopfli_shared_default})
unset(zopfli_shared_default)

if(zopfli_standalone OR ZOPFLI_BUILD_SHARED)
  option(ZOPFLI_BUILD_INSTALL "Add Zopfli install target" ON)
else()
  option(ZOPFLI_BUILD_INSTALL "Add Zopfli install target" OFF)
endif()

option(ZOPFLI_DEFAULT_RELEASE "If CMAKE_BUILD_TYPE is empty, default to Release" ON)
if(zopfli_standalone AND ZOPFLI_DEFAULT_RELEASE)
  if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS "CMAKE_BUILD_TYPE empty, defaulting to Release")
    set(CMAKE_BUILD_TYPE Release)
  endif()
endif()

set(ZOPFLI_VERSION_MAJOR 1)
set(ZOPFLI_VERSION_MINOR 0)
set(ZOPFLI_VERSION_PATCH 3)
set(ZOPFLI_VERSION ${ZOPFLI_VERSION_MAJOR}.${ZOPFLI_VERSION_MINOR}.${ZOPFLI_VERSION_PATCH})

# --- แก้ไขตรงนี้: สร้าง libzopfli เป็น STATIC library ---
if(ZOPFLI_BUILD_SHARED)
  set(zopfli_library_type SHARED)
else()
  set(zopfli_library_type STATIC)
endif()

# libzopfli
add_library(libzopfli ${zopfli_library_type}
  src/zopfli/blocksplitter.c
  src/zopfli/cache.c
  src/zopfli/deflate.c
  src/zopfli/gzip_container.c
  src/zopfli/hash.c
  src/zopfli/katajainen.c
  src/zopfli/lz77.c
  src/zopfli/squeeze.c
  src/zopfli/tree.c
  src/zopfli/util.c
  src/zopfli/zlib_container.c
  src/zopfli/zopfli_lib.c
)
target_include_directories(libzopfli
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/zopfli
)
set_target_properties(libzopfli PROPERTIES
  OUTPUT_NAME zopfli
  VERSION ${ZOPFLI_VERSION}
  SOVERSION ${ZOPFLI_VERSION_MAJOR}
)
if(UNIX AND NOT (BEOS OR HAIKU))
  target_link_libraries(libzopfli m)
endif()

# libzopflipng
add_library(libzopflipng ${zopfli_library_type}
  src/zopflipng/zopflipng_lib.cc
  src/zopflipng/lodepng/lodepng.cpp
  src/zopflipng/lodepng/lodepng_util.cpp
)
target_link_libraries(libzopflipng PRIVATE libzopfli)
target_include_directories(libzopflipng
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/zopflipng
)
set_target_properties(libzopflipng PROPERTIES
  OUTPUT_NAME zopflipng
  VERSION ${ZOPFLI_VERSION}
  SOVERSION ${ZOPFLI_VERSION_MAJOR}
)

# --- ลบ target EXECUTABLE zopfli และ zopflipng สำหรับ subproject ---
# add_executable(zopfli src/zopfli/zopfli_bin.c)
# target_link_libraries(zopfli libzopfli)
# add_executable(zopflipng src/zopflipng/zopflipng_bin.cc)
# target_link_libraries(zopflipng libzopflipng)

# Create aliases (เฉพาะ library)
if(NOT CMAKE_VERSION VERSION_LESS 3.0)
  add_library(Zopfli::libzopfli ALIAS libzopfli)
  add_library(Zopfli::libzopflipng ALIAS libzopflipng)
endif()
