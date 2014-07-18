####################################################################
#
# Makefile for SBR decoder - library
#
####################################################################

include make.cfg


################## user section: insert objlist here #######
LIB = libsbrdec.a

PROCHECK=FFR_aacPLUScheck.o

OBJS =	$(OBJDIR)/env_calc.o \
	$(OBJDIR)/env_dec.o \
	$(OBJDIR)/env_extr.o \
	$(OBJDIR)/freq_sca.o \
	$(OBJDIR)/lpp_tran.o \
	$(OBJDIR)/qmf_dec.o \
	$(OBJDIR)/sbr_ram.o \
	$(OBJDIR)/sbr_rom.o \
	$(OBJDIR)/sbr_bitb.o \
	$(OBJDIR)/sbr_crc.o \
	$(OBJDIR)/sbr_dec.o \
	$(OBJDIR)/sbr_fft.o \
	$(OBJDIR)/sbrdecoder.o \
        $(OBJDIR)/huff_dec.o \
	$(OBJDIR)/$(PROCHECK)

ifeq "$(MONO)" "1"
override CFLAGS += -DMONO_ONLY -DLP_SBR_ONLY
else
OBJS += $(OBJDIR)/ps_bitdec.o \
	$(OBJDIR)/ps_dec.o \
	$(OBJDIR)/hybrid.o
endif

include stdtargets.cfg

DATE   :=$(shell date +%Y-%m-%d)
TARGET  = FloatFR_sbrdeclib


