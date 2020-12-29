ifeq ($(OS),Windows_NT)
CC := clang
BINEXT := .exe
SHELL := pwsh.exe
.SHELLFLAGS := -NoProfile -Command
endif

ifeq ($(shell uname -s),Darwin)
LIBEXT := .dylib
else ifeq ($(OS),Windows_NT)
LIBEXT := .dll
else
LIBEXT := .so
endif

PROGNAME := main
PROGNAME := $(addsuffix $(BINEXT), $(PROGNAME))

# Recursive wildcard function. NOTE: Spaces in file names will break this.
rwildcard=$(wildcard $1$2) $(foreach d, $(wildcard $1*),$(call rwildcard,$d/,$2))

# Recursively find all C source and header files in the current directory.
# SRC := $(call rwildcard,,*.c)
HEADERS := $(call rwildcard,,*.h)

SRC := main.c

LIBSRC := $(call rwildcard,src,*.c)

LDFLAGS += -Llib -lrgl

OBJ := $(SRC:.c=.o)
DEP := $(SRC:.c=.d)
DEP += $(LIBSRC:.c=.d)

LIBOBJ := $(LIBSRC:.c=.o)

LIBRGL := lib/librgl$(LIBEXT)

CFLAGS += -Iinclude

$(PROGNAME): $(OBJ) $(LIBRGL)
	$(CC) $(LDFLAGS) $(OBJ) -o $(PROGNAME)

$(LIBRGL): $(LIBOBJ)
	$(CC) $(CFLAGS) -shared $^ -o $@

-include $(DEP)

src/%.o: src/%.c
	$(CC) $(CFLAGS) -MMD -MP -c -fpic $< -o $@

# Create dependency information for source files when creating objects.
%.o: %.c
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

run: $(PROGNAME)
	./$(PROGNAME)

clean:
ifeq ($(OS), Windows_NT)
	Get-ChildItem -Recurse -Include *.o,*.d | Remove-Item
else
	$(RM) $(OBJ) $(DEP) $(LIBOBJ)
endif
Clean: clean
ifeq ($(OS), Windows_NT)
	Get-ChildItem -Name -Include $(PROGNAME) | Remove-Item
else
	$(RM) $(PROGNAME) $(LIBRGL)
endif

.PHONY: clean Clean run
