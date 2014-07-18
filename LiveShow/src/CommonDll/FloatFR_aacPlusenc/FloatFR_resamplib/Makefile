####################################################################
#
# Makefile for resampler library
# $Id: Makefile,v 1.1.1.1.2.2 2004-02-15 13:23:49 ehr Exp $
#
####################################################################
include make.cfg


################## user section: insert objlist here #######
LIB = libFloatFR_resamp.a


OBJS =	$(OBJDIR)/resampler.o \
        $(OBJDIR)/iir32resample.o

ifeq "$(MONO)" "1"
override CFLAGS+=-DMONO_ONLY
endif

include stdtargets.cfg
