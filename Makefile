ifeq ($(OS),Windows_NT)
CC := clang
BINEXT := .exe
SHELL := pwsh.exe
.SHELLFLAGS := -NoProfile -Command
endif

PROGNAME := main
PROGNAME := $(addsuffix $(BINEXT), $(PROGNAME))

# Recursive wildcard function. NOTE: Spaces in file names will break this.
rwildcard=$(wildcard $1$2) $(foreach d, $(wildcard $1*),$(call rwildcard,$d/,$2))

# Recursively find all C source and header files in the current directory.
SRC := $(call rwildcard,,*.c)
HEADERS := $(call rwildcard,,*.h)

CFLAGS += -I./include

OBJ := $(SRC:.c=.o)
DEP := $(SRC:.c=.d)

$(PROGNAME): $(OBJ)
	$(CC) $(LDFLAGS) $(OBJ) -o $(PROGNAME)

-include $(DEP)

# Create dependency information for source files when creating objects.
%.o: %.c
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

run: $(PROGNAME)
	./$(PROGNAME)

clean:
ifeq ($(OS), Windows_NT)
	Get-ChildItem -Recurse -Include *.o,*.d | Remove-Item
else
	$(RM) $(OBJ) $(DEP)
endif
Clean: clean
ifeq ($(OS), Windows_NT)
	Get-ChildItem -Name -Include $(PROGNAME) | Remove-Item
else
	$(RM) $(PROGNAME)
endif

.PHONY: clean Clean run
