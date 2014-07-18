#
# Copyright (c) Coding Technologies 2003
# All Rights Reserved
#
# Makefile for FloatFR_aacPlusenc
# $Id: Makefile,v 1.6.2.18 2005-09-29 14:46:10 snd Exp $
#

UNAME :=$(shell uname -s)

ifeq "$(DEBUG)" "1"
override UNAME := $(UNAME)_D
endif

ifeq "$(OPTIM)" "3"
override UNAME := $(UNAME)_O3
endif

ifeq "$(PROF)" "1"
override UNAME := $(UNAME)_P
endif

ifeq "$(INSURE)" "1"
override UNAME := $(UNAME)_I
endif

######## Compiler definition #############
ifeq "$(INSURE)" "1"
CC=insure
CXX=insure
else
CC=gcc
CXX=g++
endif

DATE      :=$(shell date +%Y-%m-%d)
MAKE      =gmake

ifeq "$(DEBUG)" "1"
CFLAGS    = -g
else
CFLAGS    = -O2
endif

ifeq "$(MONO)" "1"
override CFLAGS+=-DMONO_ONLY
TARGET =enhAacPlusEnc.mono
else
TARGET =enhAacPlusEnc
endif

override CFLAGS  +=-IFloatFRlib -IFloatFR_fastaacenc -IFloatFR_fastaacenc/src -IFloatFR_resamplib -IFloatFR_bitbuflib -IFloatFR_sbrenclib -Ilib -Ilib/linux

LDFLAGS   =-LFloatFR_fastaacenc/lib/$(UNAME) -laacenc
LDFLAGS   +=-LFloatFR_sbrenclib/lib/$(UNAME) -lsbrenc
LDFLAGS   +=-Llib/linux -laudio
LDFLAGS   +=-LFloatFR_resamplib/lib/$(UNAME) -lFloatFR_resamp
LDFLAGS   +=-LFloatFRlib/lib/$(UNAME) -lFloatFR 
LDFLAGS   +=-LFloatFR_bitbuflib/lib/$(UNAME) -lbitbuf
LDFLAGS   +=-Llib/linux -lisomediafile
LDFLAGS   +=-lm

ifeq "$(PROF)" "1"
override CFLAGS += -pg
override LDFLAGS += -pg
endif

FloatFRAACENCLIB = FloatFR_fastaacenc/lib/$(UNAME)/libaacenc.a
FloatFRLIB    = FloatFRlib/lib/$(UNAME)/libFloatFR.a
FloatFR_RESAMPLIB = FloatFR_resamplib/lib/$(UNAME)/libFloatFR_resamp.a
FloatFR_BITBUFLIB= FloatFR_bitbuflib/lib/$(UNAME)/libbitbuf.a
FloatFR_SBRENCLIB = FloatFR_sbrenclib/lib/$(UNAME)/libsbrenc.a

SRC       =./src
OBJ       =./src


sources=$(SRC)/main.c $(SRC)/mp4file.c $(SRC)/brswitchmain.c


ofiles = $(sources:.c=.o)
.SUFFIXES: .c .o

.c.o:
	$(CC) $(CFLAGS) -c -o $@ $<

all: .depend $(ofiles) FloatFR FloatFRAACENC FloatFR_RESAMP FloatFR_SBRENC FloatFR_BITBUF
	$(CXX) -o $(TARGET) $(SRC)/main.o $(SRC)/mp4file.o $(LDFLAGS)
	$(CXX) -o enhAacPlusEnc_brswitch $(SRC)/brswitchmain.o $(SRC)/mp4file.o $(LDFLAGS)

.depend:
	@echo "Creating dependencies..."
	@for i in $(sources); do \
	(echo -n "$(OBJ)/" >> tmp_depend; $(CC) -MM $(CFLAGS) $$i >> tmp_depend); done
	@mv tmp_depend .depend

FloatFR:
	cd FloatFRlib ; make

FloatFRAACENC:
	cd FloatFR_fastaacenc ; make DEBUG=$(DEBUG) MONO=$(MONO)

FloatFR_RESAMP:
	cd FloatFR_resamplib ; make DEBUG=$(DEBUG) MONO=$(MONO)	

FloatFR_BITBUF: 
	cd FloatFR_bitbuflib; make DEBUG=$(DEBUG)

FloatFR_SBRENC:
	cd FloatFR_sbrenclib; make DEBUG=$(DEBUG) MONO=$(MONO)

clean:
	rm -f .depend
	rm -f $(ofiles) $(TARGET)
	make -C FloatFRlib clean
	make -C FloatFR_fastaacenc DEBUG=$(DEBUG) OPTIM=$(OPTIM) PROF=$(PROF) clean
	make -C FloatFR_sbrenclib  DEBUG=$(DEBUG) OPTIM=$(OPTIM) PROF=$(PROF) clean
	make -C FloatFR_resamplib  DEBUG=$(DEBUG) OPTIM=$(OPTIM) PROF=$(PROF) clean
	make -C FloatFR_bitbuflib  DEBUG=$(DEBUG) OPTIM=$(OPTIM) PROF=$(PROF) clean

tags:
	etags *.c *.h


-include .depend

