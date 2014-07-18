#
# Makefile for $(TARGET)
# $Id: Makefile,v 1.2.6.6 2005-02-15 16:34:13 schug Exp $
#
####################################################################
#
# Makefile for FloatFR_aacdec - library
#
####################################################################

include make.cfg

LIB = libaacdec.a

OBJS =  $(OBJDIR)/aacdecoder.o \
        $(OBJDIR)/block.o \
        $(OBJDIR)/channel.o \
        $(OBJDIR)/channelinfo.o \
        $(OBJDIR)/dse.o \
        $(OBJDIR)/imdct.o \
        $(OBJDIR)/longblock.o \
        $(OBJDIR)/pulsedata.o \
        $(OBJDIR)/shortblock.o \
        $(OBJDIR)/stereo.o \
        $(OBJDIR)/aac_ram.o \
        $(OBJDIR)/aac_rom.o \
        $(OBJDIR)/tns.o \
        $(OBJDIR)/bitstream.o \
        $(OBJDIR)/streaminfo.o \
	$(OBJDIR)/conceal.o \
	$(OBJDIR)/pns.o

ifeq "$(MONO)" "1"
override CFLAGS += -DMONO_ONLY -DLP_SBR_ONLY
endif

include stdtargets.cfg

DATE   :=$(shell date +%Y-%m-%d)
TARGET  = FloatFR_aacdeclib
