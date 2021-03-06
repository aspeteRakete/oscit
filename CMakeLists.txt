# ==============================================================================
#
#  This file is part of the OSCIT library (http://rubyk.org/liboscit)
#  Copyright (c) 2007-2010 by Gaspard Bucher - Buma (http://teti.ch).
#
# ------------------------------------------------------------------------------
#
#  Permission is hereby granted, free of charge, to any person obtaining a copy
#  of this software and associated documentation files (the "Software"), to deal
#  in the Software without restriction, including without limitation the rights
#  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
#  copies of the Software, and to permit persons to whom the Software is
#  furnished to do so, subject to the following conditions:
#
#  The above copyright notice and this permission notice shall be included in
#  all copies or substantial portions of the Software.
#
#  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
#  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
#  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
#  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
#  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
#  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
#  THE SOFTWARE.
#
# ==============================================================================

project (OSCIT)
cmake_minimum_required(VERSION 2.6.2)
# ==============================================================================
#
#  Options
#
# ==============================================================================
option (RELEASE                  "Universal build (release) ?"                OFF )
option (OSCIT_386_ONLY           "Compile for i386 target only (no 64bit) ?"  OFF )
option (OSCIT_MEMORY_CHECKING    "Enable checking against memory leaks ?"     OFF )
option (OSCIT_ENABLE_TESTING     "Build and run tests ?"                      ON  )
option (OSCIT_OPENCV_LINK        "Set this to NO to link opencv alloc later." ON  )


# handle memory checking option
if (OSCIT_MEMORY_CHECKING)

  if (APPLE)
    find_library (HAVE_MALLOC_DEBUG MallocDebug ${lib_locations})

    if (HAVE_MALLOC_DEBUG)
      message (STATUS "libMallocDebug = ${HAVE_MALLOC_DEBUG}")
    else (HAVE_MALLOC_DEBUG)
      set (HAVE_MALLOC_DEBUG NO)
    endif (HAVE_MALLOC_DEBUG)
  endif (APPLE)

endif (OSCIT_MEMORY_CHECKING)

# ragel executable
set(RAGEL "ragel")

# ==============================================================================
#
#  Platform guessing
#
# ==============================================================================
if(UNIX)
  set(OSCPACK_PLAT "posix")
  if(APPLE)
    set(PLAT "macosx")
    set(PLAT_LINK "")
    set(CMAKE_CXX_FLAGS "-mmacosx-version-min=10.4 -g -Wall")
    if (OSCIT_386_ONLY)
      set(CMAKE_OSX_ARCHITECTURES "i386" CACHE STRING "Build architectures for OSX" FORCE)
    else(OSCIT_386_ONLY)
      if (RELEASE)
        set(CMAKE_OSX_ARCHITECTURES "ppc;i386;x86_64" CACHE STRING "Build architectures for OSX" FORCE)
      endif(RELEASE)
    endif(OSCIT_386_ONLY)
  else(APPLE)
    set(PLAT "linux")
    set(PLAT_LINK avahi-client rt)
    set (CMAKE_CXX_FLAGS "-g -Wall")
    if(ENDIANESS)
    else(ENDIANESS)
      message(STATUS, "Guessing Linux host endianess to be little endian.")
      message(STATUS, "You can run cmake with -DENDIANESS=OSC_HOST_BIG_ENDIAN if you need to change this setting.")
    endif(ENDIANESS)
    set (ENDIANESS "OSC_HOST_LITTLE_ENDIAN")
    add_definitions(-D${ENDIANESS})
  endif(APPLE)
else(UNIX)
  if(WIN32)
    set(PLAT "win32")
    set(OSCPACK_PLAT "win32")
    set (CMAKE_CXX_FLAGS "-g -Wall")
  else(WIN32)
    set(PLAT "unsupported")
  endif(WIN32)
endif(UNIX)

add_definitions(-D__${PLAT}__)

set(EXT_SOURCES)
# ==============================================================================
#
#  oscit library build
#
# ==============================================================================
file (GLOB OSCIT_SOURCES src/*.cpp src/${PLAT}/*.cpp)

if(OSCIT_OPENCV_LINK)
  # include cxalloc.cpp and cxsystem.cpp
  file (GLOB OPENCV_SOURCES vendor/opencv/*.cpp)
  list(APPEND EXT_SOURCES ${OPENCV_SOURCES})
endif(OSCIT_OPENCV_LINK)

include_directories (${OSCIT_SOURCE_DIR}/oscpack ${OSCIT_SOURCE_DIR}/include ${OSCIT_SOURCE_DIR}/vendor)
include_directories (AFTER ${OSCIT_SOURCE_DIR}/test)

file (GLOB RAGEL_SOURCES src/*.rl)
foreach (RAGEL_SRC ${RAGEL_SOURCES})
  get_filename_component (RAGEL_SRC_NAME ${RAGEL_SRC} NAME_WE)
  add_custom_command ( PRE_BUILD
    OUTPUT  ${OSCIT_SOURCE_DIR}/src/${RAGEL_SRC_NAME}.cpp
    COMMAND ${RAGEL} ${RAGEL_SRC} -o ${OSCIT_SOURCE_DIR}/src/${RAGEL_SRC_NAME}.cpp
    DEPENDS ${RAGEL_SRC}
  )
  # make sure output is included in OSCIT_SOURCES in case 'make clean' removes cached cpp file
  # TODO: how to make sure 'clean' does not remove these generated files ?
  set (OSCIT_SOURCES ${OSCIT_SOURCES} ${OSCIT_SOURCE_DIR}/src/${RAGEL_SRC_NAME}.cpp)
endforeach (RAGEL_SRC)

# oscpack
file (GLOB OSCPACK_SOURCES oscpack/ip/*.cpp oscpack/ip/${OSCPACK_PLAT}/*.cpp oscpack/osc/*.cpp)
list (APPEND EXT_SOURCES ${OSCPACK_SOURCES})

add_library (oscit STATIC ${OSCIT_SOURCES} ${OSCPACK_SOURCES} ${EXT_SOURCES})

# ==============================================================================
#
#  examples build
#
# ==============================================================================

# Exclude from default 'all' target
add_custom_target(examples)
file (GLOB OSCIT_EXAMPLES examples/*.cpp)

foreach (OSCIT_EXAMPLE ${OSCIT_EXAMPLES})
  get_filename_component (OSCIT_EXAMPLE_NAME ${OSCIT_EXAMPLE} NAME_WE)

  add_executable(${OSCIT_EXAMPLE_NAME} EXCLUDE_FROM_ALL ${OSCIT_EXAMPLE})
  target_link_libraries(${OSCIT_EXAMPLE_NAME} oscit ${PLAT_LINK})
  add_dependencies(examples ${OSCIT_EXAMPLE_NAME})
endforeach (OSCIT_EXAMPLE)

file(GLOB OSCIT_EXAMPLE_ASSETS examples/*.json)
foreach (OSCIT_EXAMPLE_ASSET ${OSCIT_EXAMPLE_ASSETS})
  get_filename_component (OSCIT_EXAMPLE_ASSET_NAME ${OSCIT_EXAMPLE_ASSET} NAME)
  configure_file(${OSCIT_EXAMPLE_ASSET} ${OSCIT_EXAMPLE_ASSET_NAME} COPYONLY)
endforeach(OSCIT_EXAMPLE_ASSET)

# ==============================================================================
#
#  test_runner build
#
# ==============================================================================
# (one test for all CxxTests)


# handle testing option
if (OSCIT_ENABLE_TESTING)
  enable_testing()
  if(OSCIT_MEMORY_CHECKING)
    file (GLOB OSCIT_TEST_SOURCES test/*_test.h test/*_test_slow.h test/zpause_for_memory_analysis.h)
  else(OSCIT_MEMORY_CHECKING)
    file (GLOB OSCIT_TEST_SOURCES test/*_test.h test/*_test_slow.h)
  endif(OSCIT_MEMORY_CHECKING)

  file (GLOB OSCIT_TEST_MOCKS test/mock/*.h)
  add_custom_command ( PRE_BUILD
    OUTPUT  test_runner.cpp
    COMMAND ${OSCIT_SOURCE_DIR}/test/cxxtest/cxxtestgen.pl --error-printer -o test_runner.cpp ${OSCIT_TEST_SOURCES}
    DEPENDS ${OSCIT_TEST_SOURCES} ${OSCIT_TEST_MOCKS}
  )

  add_executable (test_runner test_runner.cpp)
  target_link_libraries (test_runner oscit ${PLAT_LINK})
  add_test (oscit_test test_runner)

  # this is to enable verbose output during testing
  add_custom_target (test_all ALL ${CMAKE_CTEST_COMMAND} -V)
  add_dependencies (test_all test_runner)

  # avahi testing...
  if(PLAT STREQUAL "linux")
    add_executable(publish ${OSCIT_SOURCE_DIR}/test/mock/publish.c)
    target_link_libraries(publish ${PLAT_LINK})

    add_executable(registration ${OSCIT_SOURCE_DIR}/test/mock/registration.cpp)
    target_link_libraries(registration ${PLAT_LINK})
  endif(PLAT STREQUAL "linux")


  # valgrind command ?
  # valgrind --leak-check=full --track-origins=yes ./test_runner
endif (OSCIT_ENABLE_TESTING)

# ==============================================================================
#
#  configuration feedback
#
# ==============================================================================

message (STATUS "===========================================================================")
message (STATUS "   OSCIT build configuration settings                                      ")
message (STATUS "===========================================================================")
message (STATUS "   Platform                                                       = ${PLAT}")
message (STATUS "   Release                                                        = ${RELEASE}")
message (STATUS "   OSCIT_386_ONLY          (no 64bit)                             = ${OSCIT_386_ONLY}")
message (STATUS "   OSCIT_ENABLE_TESTING    (Build and run tests)                  = ${OSCIT_ENABLE_TESTING}")
message (STATUS "   OSCIT_MEMORY_CHECKING   (Enable checking against memory leaks) = ${OSCIT_MEMORY_CHECKING}")
message (STATUS "   OSCIT_OPENCV_LINK       (Include opencv alloc now)             = ${OSCIT_OPENCV_LINK}")
if(OSCIT_MEMORY_CHECKING)
message (STATUS "       you should run test_runner with")
message (STATUS "       > export MallocStackLogging=1;./test_runner")
endif(OSCIT_MEMORY_CHECKING)
message (STATUS "")
message (STATUS "   Type: 'make examples' to build examples")
message (STATUS "   Type: 'ccmake <path to oscit>' to change settings")
message (STATUS "===========================================================================")
