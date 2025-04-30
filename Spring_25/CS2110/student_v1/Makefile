################################################################################
# These are variables for the GBA toolchain build
################################################################################

PROGNAME = App

GBA_CFILES := $(filter-out client.c $(wildcard suites/*.c),$(wildcard *.c **/*.c))
GBA_OFILES := $(patsubst %.c,%.o,$(GBA_CFILES))

# gnu99 is due to use of MAP_ANONYMOUS extension for dma_framework
# other flags are same as always
TEST_CFLAGS = -std=gnu99 -Wall -pedantic -Wextra -Werror -Wstrict-prototypes -Wold-style-definition -Werror=vla -g -O1

TEST_ARCH=$(shell uname -m)
# Suite and helper files:
TEST_FILES := $(filter-out $(wildcard suites/_*.c), $(wildcard suites/*.c))
TEST_FILES += $(wildcard suites/$(TEST_ARCH)/*.o)
# The student file
TEST_FILES += $(GBA_CFILES)

ifdef TEST
	TEST_SUITE = $(shell echo $(TEST) | awk -F'::' '{print $$1}')
	TEST_NAME = $(shell echo $(TEST) | awk -F'::' '{print $$2}')
	TEST_BP = -ex 'b $(TEST_SUITE)_$(TEST_NAME)_impl'
	TEST_FILTER = --filter $(TEST_SUITE)/$(TEST_NAME)
endif

################################################################################
# These are various settings used to make the GBA toolchain work
# DO NOT EDIT BELOW.
################################################################################

.PHONY: all
all: CFLAGS += $(CDEBUG) -I../shared
all: LDFLAGS += $(LDDEBUG)
all: $(PROGNAME).gba
	@echo "[FINISH] Created $(PROGNAME).gba"

include /opt/cs2110-tools/GBAVariables.mak

LDFLAGS += --specs=nosys.specs

# Adjust default compiler warnings and errors
CFLAGS += -Wstrict-prototypes -Wold-style-definition -Werror=vla

###############
# GBA TARGETS #
###############

.PHONY: debug
debug: CFLAGS += $(CDEBUG) -I../shared
debug: LDFLAGS += $(LDDEBUG)
debug: $(PROGNAME).gba
	@echo "[FINISH] Created $(PROGNAME).gba"

$(PROGNAME).gba: clean $(PROGNAME).elf client
	@echo "[LINK] Linking objects together to create $(PROGNAME).gba"
	@$(OBJCOPY) -O binary $(PROGNAME).elf $(PROGNAME).gba

$(PROGNAME).elf: crt0.o $(GCCLIB)/crtbegin.o $(GCCLIB)/crtend.o $(GCCLIB)/crti.o $(GCCLIB)/crtn.o $(GBA_OFILES) libc_sbrk.o
	$(CC) -o $(PROGNAME).elf $^ $(LDFLAGS)

.PHONY: client
client: client.c
	@gcc -o client client.c

.PHONY: mgba
mgba: debug
	@./client ${PROGNAME}.gba
	@echo "[EXECUTE] Running emulator mGBA"

.PHONY: gdb
gdb: debug
	@./client ${PROGNAME}.gba gdb
	@sleep 1
	@echo "[EXECUTE] Attempting to connect to GDB server."
	@echo "          Make sure mGBA is already running on the host machine!"
	@gdb-multiarch -ex 'file $(PROGNAME).elf' -ex 'target remote host.docker.internal:2345'

#######################
# AUTOGRADING TARGETS #
#######################

# Builds the test executable and runs it:
.PHONY: run-tests
run-tests: tests
	@ ./tests $(TEST_FILTER) --timeout 20 -j1

# Builds the test executable and runs GDB on it:
.PHONY: run-tests-gdb
run-tests-gdb: tests
	@ gdb -q -ex 'set follow-fork-mode child' $(TEST_BP) -ex 'python gdb.execute("handle SIGSTOP nostop noprint")' -ex 'python gdb.execute("handle SIGCONT nostop noprint")' --args ./tests $(TEST_FILTER)
	
# Builds the test executable:
.PHONY: tests
tests: $(TEST_FILES) check-violations clean
	@ gcc -DAG_RUNNING -fno-asm $(TEST_CFLAGS) $(TEST_FILES) -lcriterion -o tests

.PHONY: check-violations
check-violations:
	@ gcc -fno-asm -nostartfiles -Wl,-e0 $(TEST_CFLAGS) $(GBA_CFILES) -o /dev/null

#########################
# GENERAL BUILD TARGETS #
#########################

.PHONY: clean
clean:
	@echo "[CLEAN] Removing all compiled files"
	rm -f *.o *.elf *.gba *.log */*.o *.sav */*/*.sav client tests
