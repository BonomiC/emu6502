# Directories
SRCDIR = src
OBJDIR = .obj
BINDIR = bin
INCDIR = include

# -fno-common to disable warning due to large ram array
CFLAGS = -fno-common

# Executable name
TARGET = m6502

# Find all C source files in the src directory
SOURCES = $(wildcard $(SRCDIR)/*.c)

# Create object file names in the obj directory
OBJECTS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SOURCES))

.PHONY: all clean

all: $(BINDIR)/$(TARGET)

# Rule to create the binary directory if it doesn't exist
$(BINDIR):
	mkdir -p $(BINDIR)

# Rule to link object files into the executable
$(BINDIR)/$(TARGET): $(OBJECTS) | $(BINDIR)
	$(CC) $(OBJECTS) -o $@

# Rule to compile C source files into object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@ -I $(INCDIR)

# Rule to create the object directory if it doesn't exist
$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR) $(BINDIR)
