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

# set the project version
VERSION := 1.0.0

# set the application name
APP_NAME := w25qxx

# set the shared libraries name
SHARED_LIB_NAME := libw25qxx.so

# set the static libraries name
STATIC_LIB_NAME := libw25qxx.a

# set the install directories
INSTL_DIRS := /usr/local

# set the include directories
INC_INSTL_DIRS := $(INSTL_DIRS)/include/$(APP_NAME)

# set the library directories
LIB_INSTL_DIRS := $(INSTL_DIRS)/lib

# set the bin directories
BIN_INSTL_DIRS := $(INSTL_DIRS)/bin

# set the compiler
CC := gcc

# set the ar tool
AR := ar

# set the packages name
PKGS := libgpiod

# set the pck-config header directories
LIB_INC_DIRS := $(shell pkg-config --cflags $(PKGS))

# set the linked libraries
LIBS := -lm \
		-lpthread

# add the linked libraries
LIBS += $(shell pkg-config --libs $(PKGS))

# set all header directories
INC_DIRS := -I ../../src/ \
			-I ../../interface/ \
			-I ../../example/ \
			-I ../../test/ \
			-I ./interface/inc/

# add the linked libraries header directories
INC_DIRS += $(LIB_INC_DIRS)

# set the installing headers
INSTL_INCS := $(wildcard ../../src/*.h)

# set all sources files
SRCS := $(wildcard ../../src/*.c)

# set the main source
MAIN := $(SRCS) \
		$(wildcard ../../example/*.c) \
		$(wildcard ../../test/*.c) \
		$(wildcard ./interface/src/*.c) \
		$(wildcard ./driver/src/*.c) \
		$(wildcard ./src/main.c)

# set flags of the compiler
CFLAGS := -O3 \
		-DNDEBUG

# set all .PHONY
.PHONY: all

# set the output list
all: $(APP_NAME) $(SHARED_LIB_NAME).$(VERSION) $(STATIC_LIB_NAME) 

# set the main app
$(APP_NAME) : $(MAIN)
			$(CC) $(CFLAGS) $^ $(INC_DIRS) $(LIBS) -o $@

# set the shared lib
$(SHARED_LIB_NAME).$(VERSION) : $(SRCS)
								$(CC) $(CFLAGS) -shared -fPIC $^ $(INC_DIRS) -lm -o $@

# set the *.o for the static libraries
OBJS := $(patsubst %.c, %.o, $(SRCS))

# set the static lib
$(STATIC_LIB_NAME) : $(OBJS)
					$(AR) -r $@ $^

# .*o used by the static lib
$(OBJS) : $(SRCS)
		$(CC) $(CFLAGS) -c $^ $(INC_DIRS) -o $@

# set install .PHONY
.PHONY: install

# install files
install :
		$(shell if [ ! -d $(INC_INSTL_DIRS) ]; then mkdir $(INC_INSTL_DIRS); fi;)
		cp -rv $(INSTL_INCS) $(INC_INSTL_DIRS)
		cp -rv $(SHARED_LIB_NAME).$(VERSION) $(LIB_INSTL_DIRS)
		ln -sf $(LIB_INSTL_DIRS)/$(SHARED_LIB_NAME).$(VERSION) $(LIB_INSTL_DIRS)/$(SHARED_LIB_NAME)
		cp -rv $(STATIC_LIB_NAME) $(LIB_INSTL_DIRS)
		cp -rv $(APP_NAME) $(BIN_INSTL_DIRS)

# set install .PHONY
.PHONY: uninstall

# uninstall files
uninstall :
		rm -rf $(INC_INSTL_DIRS)
		rm -rf $(LIB_INSTL_DIRS)/$(SHARED_LIB_NAME).$(VERSION)
		rm -rf $(LIB_INSTL_DIRS)/$(SHARED_LIB_NAME)
		rm -rf $(LIB_INSTL_DIRS)/$(STATIC_LIB_NAME) 
		rm -rf $(BIN_INSTL_DIRS)/$(APP_NAME)

# set clean .PHONY
.PHONY: clean

# clean the project
clean :
		rm -rf $(APP_NAME) $(SHARED_LIB_NAME).$(VERSION) $(STATIC_LIB_NAME)
