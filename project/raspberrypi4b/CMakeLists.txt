#
# Copyright (c) 2015 - present LibDriver All rights reserved
#
# The MIT License (MIT)
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#

# set the cmake minimum version
cmake_minimum_required(VERSION 3.0)

# set the project name and language
project(w25qxx C)

# read the version from files
file(READ ${CMAKE_CURRENT_SOURCE_DIR}/cmake/VERSION ${CMAKE_PROJECT_NAME}_VERSION)

# set the project version
set(PROJECT_VERSION ${${CMAKE_PROJECT_NAME}_VERSION})

# set c standard c99
set(CMAKE_C_STANDARD 99)

# enable c standard required
set(CMAKE_C_STANDARD_REQUIRED True)

# set release level
set(CMAKE_BUILD_TYPE Release)

# set the release flags of c
set(CMAKE_C_FLAGS_RELEASE "-O3 -DNDEBUG")

# include cmake package config helpers
include(CMakePackageConfigHelpers)

# find the pkgconfig and use this tool to find the third party packages
find_package(PkgConfig REQUIRED)

# find the third party packages with pkgconfig
pkg_search_module(GPIOD REQUIRED libgpiod)

# include all library header directories
set(LIB_INC_DIRS
    ${GPIOD_INCLUDE_DIRS}
   )

# include all linked libraries
set(LIBS
    ${GPIOD_LIBRARIES}
   )

# include all header directories
set(INC_DIRS
    ${LIB_INC_DIRS}
    ${CMAKE_CURRENT_SOURCE_DIR}/../../src
    ${CMAKE_CURRENT_SOURCE_DIR}/../../interface
    ${CMAKE_CURRENT_SOURCE_DIR}/../../example
    ${CMAKE_CURRENT_SOURCE_DIR}/../../test
    ${CMAKE_CURRENT_SOURCE_DIR}/interface/inc
   )

# include all installed headers
file(GLOB INSTL_INCS
     ${CMAKE_CURRENT_SOURCE_DIR}/../../src/*.h
    )

# include all sources files
file(GLOB SRCS
     ${CMAKE_CURRENT_SOURCE_DIR}/../../src/*.c
    )

# include executable source
file(GLOB MAIN
     ${SRCS}
     ${CMAKE_CURRENT_SOURCE_DIR}/../../example/*.c
     ${CMAKE_CURRENT_SOURCE_DIR}/../../test/*.c
     ${CMAKE_CURRENT_SOURCE_DIR}/interface/src/*.c
     ${CMAKE_CURRENT_SOURCE_DIR}/driver/src/*.c
     ${CMAKE_CURRENT_SOURCE_DIR}/src/main.c
    )

# enable output as a static library
add_library(${CMAKE_PROJECT_NAME}_static STATIC ${SRCS})

# set the static library include directories
target_include_directories(${CMAKE_PROJECT_NAME}_static PRIVATE ${INC_DIRS})

# set the static library link libraries
target_link_libraries(${CMAKE_PROJECT_NAME}_static
                      m
                     )

# rename as ${CMAKE_PROJECT_NAME}
set_target_properties(${CMAKE_PROJECT_NAME}_static PROPERTIES OUTPUT_NAME ${CMAKE_PROJECT_NAME})

# don't delete ${CMAKE_PROJECT_NAME} libs
set_target_properties(${CMAKE_PROJECT_NAME}_static PROPERTIES CLEAN_DIRECT_OUTPUT 1)

# set the static library version
set_target_properties(${CMAKE_PROJECT_NAME}_static PROPERTIES VERSION ${${CMAKE_PROJECT_NAME}_VERSION})

# enable output as a dynamic library
add_library(${CMAKE_PROJECT_NAME} SHARED ${SRCS})

# set the executable program include directories
target_include_directories(${CMAKE_PROJECT_NAME}
                           PUBLIC $<INSTALL_INTERFACE:include/${CMAKE_PROJECT_NAME}>
                           PRIVATE ${INC_DIRS}
                          )

# set the dynamic library link libraries
target_link_libraries(${CMAKE_PROJECT_NAME}
                      m
                     )

# rename as ${CMAKE_PROJECT_NAME}
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES OUTPUT_NAME ${CMAKE_PROJECT_NAME})

# don't delete ${CMAKE_PROJECT_NAME} libs
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES CLEAN_DIRECT_OUTPUT 1)

# include the public header
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES PUBLIC_HEADER "${INSTL_INCS}")

# set the dynamic library version
set_target_properties(${CMAKE_PROJECT_NAME} PROPERTIES VERSION ${${CMAKE_PROJECT_NAME}_VERSION})

# enable the executable program
add_executable(${CMAKE_PROJECT_NAME}_exe ${MAIN})

# set the executable program include directories
target_include_directories(${CMAKE_PROJECT_NAME}_exe PRIVATE ${INC_DIRS})

# set the executable program link libraries
target_link_libraries(${CMAKE_PROJECT_NAME}_exe
                      ${LIBS}
                      m
                      pthread
                     )

# rename as ${CMAKE_PROJECT_NAME}
set_target_properties(${CMAKE_PROJECT_NAME}_exe PROPERTIES OUTPUT_NAME ${CMAKE_PROJECT_NAME})

# don't delete ${CMAKE_PROJECT_NAME} exe
set_target_properties(${CMAKE_PROJECT_NAME}_exe PROPERTIES CLEAN_DIRECT_OUTPUT 1)

# install the binary
install(TARGETS ${CMAKE_PROJECT_NAME}_exe
        RUNTIME DESTINATION bin
       )

# install the static library
install(TARGETS ${CMAKE_PROJECT_NAME}_static
        ARCHIVE DESTINATION lib
       )

# install the dynamic library
install(TARGETS ${CMAKE_PROJECT_NAME}
        EXPORT ${CMAKE_PROJECT_NAME}-targets
        LIBRARY DESTINATION lib
        PUBLIC_HEADER DESTINATION include/${CMAKE_PROJECT_NAME}
       )

# make the cmake config file
configure_package_config_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/config.cmake.in
                              ${CMAKE_CURRENT_BINARY_DIR}/cmake/${CMAKE_PROJECT_NAME}-config.cmake
                              INSTALL_DESTINATION cmake
                             )

# write the cmake config version
write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/cmake/${CMAKE_PROJECT_NAME}-config-version.cmake
                                 VERSION ${PACKAGE_VERSION}
                                 COMPATIBILITY AnyNewerVersion
                                )

# install the cmake files
install(FILES "${CMAKE_CURRENT_BINARY_DIR}/cmake/${CMAKE_PROJECT_NAME}-config.cmake"
              "${CMAKE_CURRENT_BINARY_DIR}/cmake/${CMAKE_PROJECT_NAME}-config-version.cmake"
        DESTINATION cmake
       )

# set the export items
install(EXPORT ${CMAKE_PROJECT_NAME}-targets 
        DESTINATION cmake
       )

# add uninstall command
add_custom_target(uninstall
                  COMMAND ${CMAKE_COMMAND} -P ${CMAKE_CURRENT_SOURCE_DIR}/cmake/uninstall.cmake
                 )

#include ctest module
include(CTest)

# creat a test
add_test(NAME ${CMAKE_PROJECT_NAME}_test COMMAND ${CMAKE_PROJECT_NAME}_exe -p)
