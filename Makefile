# 
# Makefile for a workspace of EV3 Platform.  
# 

HRP2DIR = /home/vagrant/hrp2
SDKDIR = $(HRP2DIR)/sdk
WSPDIR = $(basename $(PWD))

# Configuration
SRCLANG := c++
KERNEL := hrp2

#
# Functions
# $2 から $1 への相対パスを生成する
#
get_relpath = $(shell perl -MFile::Spec -e "print File::Spec->abs2rel(q($1),q($2))")

#
# Include configurations of EV3RT SDK
#
include $(SDKDIR)/Makefile.sdk.conf

#
# Paths
#
KERNELDIR    := $(SDKDIR)/$(EV3RT_BASE_DIR)
OBJDIR       := $(SDKDIR)/$(EV3RT_PRJ_OBJ_DIR)
LIBKERNELDIR := $(SDKDIR)/$(EV3RT_LIBKERNEL_DIR)
TARGETDIR    := $(KERNELDIR)/target/ev3_gcc

# Object files
OBJFILENAME := $(KERNEL)
OBJBINARY   := $(OBJDIR)/$(KERNEL).bin

#
# アプリケーションビルド用のMakefile
#
APPLDIR       := $(PWD)
MKFILENAME    := Makefile.app
MKFILE_DIR    := $(APPLDIR)/conf

#
# Target for an application module (dynamic)
#
include $(APPLDIR)/conf/Makefile.inc

temp_mkfilename := .ev3rt_temp_Makefile
configure_copts := -DBUILD_MODULE
# ここでやっていること
#   Makefile.app を元に $(OBJDIR) にアプリケーションビルド用の Makefile を生成する
#   その後 make を行い，生成されたバイナリ(app)をコピーしてくる
app: $(APPLDIR)
	@cp $(MKFILE_DIR)/$(MKFILENAME) $(APPLDIR)/$(temp_mkfilename)
	@mkdir -p $(OBJDIR)
	cd $(OBJDIR) \
    && $(KERNELDIR)/configure \
        -T ev3_gcc \
        -A app \
        -a $(call get_relpath,$(APPLDIR),$(OBJDIR)) \
        -t $(call get_relpath,$(APPLDIR),$(OBJDIR)) \
        -D $(call get_relpath,$(KERNELDIR),$(OBJDIR)) \
        -L $(call get_relpath,$(LIBKERNELDIR),$(OBJDIR)) \
        -l $(SRCLANG) \
        -m $(temp_mkfilename) \
        -o "$(configure_copts)" \
        -U "$(APPLOBJS)" \
    && rm $(APPLDIR)/$(temp_mkfilename) \
    && mv $(temp_mkfilename) Makefile \
    && make clean \
    && cd $(OBJDIR) \
    && make module_cfg.h \
    && make -j8 \
    && cp $(OBJDIR)app /vagrant/selab

clean:
	rm -rf $(OBJDIR)

realclean: clean
	rm -rf $(notdir $(OBJFILENAME)) uImage app $(LIBKERNELDIR)/libkernel.a

.PHONY: clean realclean app 

