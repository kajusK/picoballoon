PROJECT = probe
BUILD_DIR = bin

SHARED_DIR = src
LMIC_DIR = external/arduino-lmic/src
CFILES = $(wildcard $(SHARED_DIR)/*.c) \
	 $(wildcard $(SHARED_DIR)/*/*.c) \
	 $(wildcard $(LMIC_DIR)/lmic/*.c) \
	 $(wildcard $(LMIC_DIR)/aes/*.c)

DEVICE=stm32f042f6p6

# You shouldn't have to edit anything below here.
VPATH += $(SHARED_DIR)
INCLUDES += $(patsubst %,-I%, . $(SHARED_DIR)) \
	    $(patsubst %,-I%, . $(LMIC_DIR))
OPENCM3_DIR=external/libopencm3

include $(OPENCM3_DIR)/mk/genlink-config.mk
include rules.mk
include $(OPENCM3_DIR)/mk/genlink-rules.mk
