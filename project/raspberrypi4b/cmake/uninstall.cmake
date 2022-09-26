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

# check the install_manifest.txt
if(NOT EXISTS "${CMAKE_CURRENT_BINARY_DIR}/install_manifest.txt")
    # output the error
    message(FATAL_ERROR "cannot find install manifest: ${CMAKE_CURRENT_BINARY_DIR}/install_manifest.txt")
endif()

# read install_manifest.txt to uninstall_list
file(READ "${CMAKE_CURRENT_BINARY_DIR}/install_manifest.txt" ${CMAKE_PROJECT_NAME}_uninstall_list)

# replace '\n' to ';'
string(REGEX REPLACE "\n" ";" ${CMAKE_PROJECT_NAME}_uninstall_list "${${CMAKE_PROJECT_NAME}_uninstall_list}")

# uninstall the list files
foreach(${CMAKE_PROJECT_NAME}_uninstall_list ${${CMAKE_PROJECT_NAME}_uninstall_list})
    # if a link or a file
    if(IS_SYMLINK "$ENV{DESTDIR}${${CMAKE_PROJECT_NAME}_uninstall_list}" OR EXISTS "$ENV{DESTDIR}${${CMAKE_PROJECT_NAME}_uninstall_list}")
        # delete the file
        execute_process(COMMAND ${CMAKE_COMMAND} -E remove ${${CMAKE_PROJECT_NAME}_uninstall_list}
                        RESULT_VARIABLE rm_retval
                       )
        
        # check the retval
        if(NOT "${rm_retval}" STREQUAL 0)
            # output the error
            message(FATAL_ERROR "failed to remove file: '${${CMAKE_PROJECT_NAME}_uninstall_list}'.")
        else()
            # uninstalling files
            message(STATUS "uninstalling: $ENV{DESTDIR}${${CMAKE_PROJECT_NAME}_uninstall_list}")
        endif()
    else()
        # output the error
        message(STATUS "file: $ENV{DESTDIR}${${CMAKE_PROJECT_NAME}_uninstall_list} does not exist.")
    endif()
endforeach()
