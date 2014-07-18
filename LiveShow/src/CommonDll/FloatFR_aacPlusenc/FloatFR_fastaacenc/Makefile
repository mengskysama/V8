####################################################################
#
# Makefile for aac encoder - library
#
####################################################################
include make.cfg


################## user section: insert objlist here #######
LIB = libaacenc.a


OBJS =  $(OBJDIR)/adj_thr.o \
	$(OBJDIR)/band_nrg.o \
	$(OBJDIR)/block_switch.o \
	$(OBJDIR)/grp_data.o \
	$(OBJDIR)/interface.o \
	$(OBJDIR)/ms_stereo.o \
	$(OBJDIR)/pre_echo_control.o \
	$(OBJDIR)/psy_configuration.o \
	$(OBJDIR)/psy_main.o \
	$(OBJDIR)/spreading.o \
	$(OBJDIR)/tns.o \
	$(OBJDIR)/tns_param.o \
	$(OBJDIR)/transform.o \
	$(OBJDIR)/adj_thr.o \
	$(OBJDIR)/dyn_bits.o \
	$(OBJDIR)/line_pe.o \
	$(OBJDIR)/qc_main.o \
	$(OBJDIR)/quantize.o \
	$(OBJDIR)/sf_estim.o \
	$(OBJDIR)/stat_bits.o \
	$(OBJDIR)/bit_cnt.o \
	$(OBJDIR)/bitenc.o \
	$(OBJDIR)/aacenc.o \
	$(OBJDIR)/stprepro.o \
	$(OBJDIR)/channel_map.o \
	$(OBJDIR)/aac_ram.o \
	$(OBJDIR)/aac_rom.o


ifeq "$(MONO)" "1"
override CFLAGS+=-DMONO_ONLY
endif

include stdtargets.cfg



