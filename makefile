# ----------------------------
# Makefile Options
# ----------------------------

NAME ?= ENDMAZE
ICON ?= icon.png
DESCRIPTION ?= "A maze generator for the CE"
COMPRESSED ?= NO
ARCHIVED ?= NO

CFLAGS ?= -Wall -Wextra -Oz
CXXFLAGS ?= -Wall -Wextra -Oz

# ----------------------------

ifndef CEDEV
$(error CEDEV environment path variable is not set)
endif

include $(CEDEV)/meta/makefile.mk
