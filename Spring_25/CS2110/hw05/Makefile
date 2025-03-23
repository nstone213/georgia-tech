# Homework 5 - CS 2110 Spring 2025
# DO NOT MODIFY FILE
# GCC flags from the syllabus (each flag described for the curious minds!)
# Flag info credit: https://gcc.gnu.org/onlinedocs/gcc/Warning-Options.html

CFLAGS = -std=c99                          # Using the C99 standard
CFLAGS += -Wall                            # This enables all the warnings about constructions that some users consider questionable, and that are easy to avoid (or modify to prevent the warning), even in conjunction with macros
CFLAGS += -pedantic                        # Issue all the warnings demanded by strict ISO C and ISO C++; reject all programs that use forbidden extensions, and some other programs that do not follow ISO C and ISO C++
CFLAGS += -Wextra                          # This enables some extra warning flags that are not enabled by -Wall
CFLAGS += -Werror                          # Make all warnings into errors
CFLAGS += -Wstrict-prototypes              # Warn if a function is declared or defined without specifying the argument types
CFLAGS += -Wold-style-definition           # Warn if an old-style function definition is used. A warning is given even if there is a previous prototype
CFLAGS += -Werror=vla                      # Generate an error if variable-length arrays (bad practice in C!) are used

# Flags not defined in the syllabus, but useful regardless:
EXTRA_CFLAGS = -g                          # Generate debugging information
# EXTRA_CFLAGS = -fsanitize=address          # Enable ASan

CFILES = my_string.c minecraft.c
# Name of the driver executable
DRIVER_EXEC = driver

ARCH=$(shell uname -m)
TEST_FILES := $(wildcard suites/*.c)
# Helpers:
TEST_FILES += $(wildcard suites/$(ARCH)/*.o)

ifdef TEST
	SUITE = $(shell echo $(TEST) | awk -F'::' '{print $$1}')
	TEST_NAME = $(shell echo $(TEST) | awk -F'::' '{print $$2}')
	BREAKPOINT = -ex 'b $(SUITE)_$(TEST_NAME)_impl'
	FILTER = --filter $(SUITE)/$(TEST_NAME)
endif


#######################
# AUTOGRADING TARGETS #
#######################

# Builds the test executable and runs it:
.PHONY: run-tests
run-tests: tests
	@ ./tests $(FILTER) --timeout 3

# Builds the test executable and runs GDB on it:
.PHONY: run-gdb
run-gdb: tests
	@ gdb -q -ex 'set follow-fork-mode child' $(BREAKPOINT) -ex 'python gdb.execute("handle SIGSTOP nostop noprint")' -ex 'python gdb.execute("handle SIGCONT nostop noprint")' --args ./tests $(FILTER)
	
# Builds the test executable:
.PHONY: tests
tests: $(CFILES) $(TEST_FILES) clean check-violations
	@ gcc -fno-asm $(CFLAGS) $(EXTRA_CFLAGS) $(CFILES) $(TEST_FILES) -lcriterion -o tests

#########################
# GENERAL BUILD TARGETS #
#########################

# Checks to make sure no invariants are violated prior to building. The invariants are as follows:
# 1. No usage of array notation in my_string.c
#     GCC is used to strip out comments from the code
#     grep is used to detect instances of "[" (array notation)
#     If not found, grep returns 1, so invert the output
# 2. No usage of standard library functions and no usage of AG helpers
#     The poison.h file is provided to ban all usages of stdlib functions and AG helpers
#     The student files have to be compiled **and linked** on their own 
#     without the AG helpers to ensure AG helpers cannot be used.
#         -nostartfiles -Wl,-e0 initiates linking without requiring main
#         -o /dev/null prevents an output file from being created
.PHONY: check-violations
check-violations: $(AUTOGRADER_C_FILES)
	@ echo "Checking for array notation in my_string.c..."
	@ ! gcc -fpreprocessed -dD -E my_string.c | grep "\["
	@ echo "No array notation found!"
	@ gcc -fno-asm -nostartfiles -Wl,-e0 $(CFLAGS) $(EXTRA_CFLAGS) $(CFILES) -include suites/poison.h -o /dev/null

# Compile all source files with the given flags into the specified executable object!
driver: $(CFILES) main.c check-violations
	@ gcc -fno-asm $(CFLAGS) $(CFILES) main.c -o $(DRIVER_EXEC)

# Remove any AG files
.PHONY: clean
clean:
	@ rm -f *.o tests
