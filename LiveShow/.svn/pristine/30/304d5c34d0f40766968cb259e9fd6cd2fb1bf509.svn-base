####################################################################
#
# Makefile for SBR encoder - library
#
####################################################################
include make.cfg


################## user section: insert objlist here #######
LIB = libsbrenc.a


OBJS =	$(OBJDIR)/code_env.o \
	$(OBJDIR)/env_est.o \
	$(OBJDIR)/env_bit.o \
	$(OBJDIR)/fram_gen.o \
	$(OBJDIR)/freq_sca.o \
	$(OBJDIR)/nf_est.o \
	$(OBJDIR)/qmf_enc.o \
	$(OBJDIR)/tran_det.o \
	$(OBJDIR)/mh_det.o \
	$(OBJDIR)/sbr_rom.o \
	$(OBJDIR)/sbr_ram.o \
	$(OBJDIR)/invf_est.o \
	$(OBJDIR)/sbr_misc.o \
	$(OBJDIR)/bit_sbr.o \
	$(OBJDIR)/sbr_main.o \
	$(OBJDIR)/ton_corr.o 

ifeq "$(MONO)" "1"
override CFLAGS+=-DMONO_ONLY
else
OBJS += $(OBJDIR)/ps_bitenc.o \
	$(OBJDIR)/ps_enc.o \
	$(OBJDIR)/hybrid.o
endif

include stdtargets.cfg


