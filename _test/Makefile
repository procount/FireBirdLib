APP1 = LinkerTest
BASE = $(shell cd /tapapi/TMS; pwd)
CUR_DIR = $(shell pwd)

# include default settings
include ${BASE}/include/tool.mk

# output object directory
BIN_DIR = ${CUR_DIR}/bin
SRC_DIR = ${CUR_DIR}

TAP_FLAGS += -fPIC

ifeq (${DEBUG},y)
TAP_FLAGS += -g -DDEBUG
endif

#compile options
CFLAGS += -DLINUX -MD -W -Wall -O2 -mips32 -std=c99
CPPFLAGS += -DLINUX -MD -W -Wall  -O2 -mips32

INCLUDE_DIRS = ${BASE}/include /opt/crosstool/include/
TAP_INCLUDE_DIRS = $(addprefix -I, $(INCLUDE_DIRS))
TAP_FLAGS += $(TAP_INCLUDE_DIRS)
TAP_LIBS = ${BASE}/tapinit.o ${BASE}/libtap.so -lFireBird -ldl -lc

all: $(BIN_DIR) $(APP1).tap clean

$(BIN_DIR):
	@echo "[Making directory... $@]"
	@$(MKDIR) "$@"

$(APP1).tap: $(APP1).o
	@echo "[Linking... $@]"
	@$(LD) -shared --no-undefined --allow-shlib-undefined  -o $(BIN_DIR)/$@ $< $(TAP_LIBS)
	@$(JB) $(BIN_DIR)/$@
	@-${RM} $(subst .tap,_TMS.tap,$@)
	@$(MV) $(BIN_DIR)/$@ $(subst .tap,_TMS.tap,$@)

$(APP1).o: $(APP1).c ${BASE}/include/libFireBird.h
	@echo "[Compiling... $@]"
	@$(CC) $(CFLAGS) $(TAP_FLAGS) -c $< -o $@

clean:
	@echo "[Cleaning up...]"
	@-${RM} *.d *.o
	@-$(RMDIR) $(BIN_DIR)
