.SUFFIXES:

ifeq ($(strip $(DEVKITARM)),)
$(error DEVKITARM is not set)
endif

include $(DEVKITARM)/3ds_rules

TARGET := ClubHidro
BUILD := build
SOURCES := source

ARCH := -march=armv6k -mtune=mpcore -mfloat-abi=hard -mtp=soft

CFLAGS := -g -Wall -O2 -mword-relocations $(ARCH)
CFLAGS += $(INCLUDE) -D__3DS__

LDFLAGS = -specs=3dsx.specs -g $(ARCH) -Wl,-Map,$(notdir $*.map)

LIBS := -lcitro2d -lcitro3d -lctru -lm
LIBDIRS := $(CTRULIB)

ifneq ($(BUILD),$(notdir $(CURDIR)))

export OUTPUT := $(CURDIR)/$(TARGET)
export VPATH := $(foreach dir,$(SOURCES),$(CURDIR)/$(dir))
export DEPSDIR := $(CURDIR)/$(BUILD)

CFILES := $(foreach dir,$(SOURCES),$(notdir $(wildcard $(dir)/*.c)))

export OFILES := $(CFILES:.c=.o)
export INCLUDE := $(foreach dir,$(LIBDIRS),-I$(dir)/include)
export LIBPATHS := $(foreach dir,$(LIBDIRS),-L$(dir)/lib)

.PHONY: all clean

all:
	@mkdir -p $(BUILD)
	@$(MAKE) --no-print-directory -C $(BUILD) -f $(CURDIR)/Makefile

clean:
	@rm -rf $(BUILD) $(TARGET).3dsx $(TARGET).elf

else

$(OUTPUT).3dsx: $(OUTPUT).elf
$(OUTPUT).elf: $(OFILES)

-include $(DEPSDIR)/*.d

endif
