#
# Copyright (c) Coding Technologies 2004
# All Rights Reserved
#
# Makefile for FloatFR_aacPlusdec
# $Id: Makefile,v 1.13.4.18 2005-03-11 16:22:58 ehr Exp $
#

UNAME :=$(shell uname -s)

ifeq "$(DEBUG)" "1"
override UNAME := $(UNAME)_D
endif

ifeq "$(OPTIM)" "3"
override UNAME := $(UNAME)_O3
endif


######## Compiler definition #############
CC=gcc
CXX=g++

CFLAGS       = -Wall -W -Wstrict-prototypes -Wmissing-prototypes -g
override CFLAGS += -IFloatFRlib -IFloatFR_bitbuflib -IFloatFR_sbrdeclib -IFloatFR_aacdec -IFloatFR_aacdec/src -Ilib -Ilib/linux -Iaudiolib -Ilibisomediafile/src -Ilibisomediafile/linux

LDFLAGS      = -LFloatFR_aacdec/lib/$(UNAME) -laacdec
LDFLAGS     += -LFloatFR_sbrdeclib/lib/$(UNAME) -lsbrdec
LDFLAGS     += -LFloatFR_bitbuflib/lib/$(UNAME) -lbitbuf
LDFLAGS     += -LFloatFRlib/lib/$(UNAME) -lFloatFR -Llib/linux -Laudiolib/lib/$(UNAME) -laudio -Llib/linux -Llibisomediafile/lib/$(UNAME) -lisomediafile
SBRDECLIB    = FloatFR_sbrdeclib/lib/$(UNAME)/libsbrdec.a
FloatFRAACDECLIB = FloatFR_aacdec/lib/$(UNAME)/libaacdec.a
FloatFRLIB       = FloatFRlib/lib/$(UNAME)/libFloatFR.a
BITBUFLIB    = FloatFR_bitbuflib/lib/$(UNAME)/libbitbuf.a


ifeq "$(MONO)" "1"
override CFLAGS += -DMONO_ONLY -DLP_SBR_ONLY
TARGET = enhAacPlusDec.mono
else
TARGET = enhAacPlusDec
endif # MONO


SRC          = ./src
OBJ          = ./src


sources=$(SRC)/main.c \
	$(SRC)/spline_resampler.c \
	$(SRC)/fileifc.c 

# Generate list of object files from list of source files:
# First, replace .cpp by .o, then .c by .o
temp = $(sources:.cpp=.o)
ofiles = $(temp:.c=.o)
.SUFFIXES: .cpp .o .c


.cpp.o:
	$(CXX) $(CFLAGS) -c -o $@ $<

all: .depend $(ofiles) FloatFR_LIB BITBUFLIB SBRDECLIB FloatFRAACDECLIB
	$(CXX) -o $(TARGET) $(ofiles) $(LDFLAGS)

.depend:
	@echo "Creating dependencies..."
	for i in $(sources); do \
	($(CXX) -MM $(CFLAGS) $$i >> tmp_depend); done
	mv tmp_depend .depend

FloatFR_LIB:
	cd FloatFRlib ; make

BITBUFLIB:
	cd FloatFR_bitbuflib ; make

FloatFRAACDECLIB:
	cd FloatFR_aacdec ; make MONO=$(MONO)

SBRDECLIB:
	cd FloatFR_sbrdeclib ; make MONO=$(MONO)


.PHONY: clean
clean:
	rm -f .depend
	rm -f $(ofiles) $(TARGET)
	make -C FloatFRlib clean
	make -C FloatFR_bitbuflib DEBUG=$(DEBUG) clean  
	make -C FloatFR_aacdec DEBUG=$(DEBUG) OPTIM=$(OPTIM) clean
	make -C FloatFR_sbrdeclib DEBUG=$(DEBUG) OPTIM=$(OPTIM) clean

.PHONY: tags
tags:
	etags *.c *.h

-include .depend

