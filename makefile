# Makefile.

SDIR=src
SOURCES_C = $(shell find $(SDIR) -type f -name "*.c";)
SOURCES_CXX = $(shell find $(SDIR) -type f -name "*.cpp";)
SOURCES_ASM = $(shell find $(SDIR) -type f -name "*.asm";)
SOURCES_S = $(shell find $(SDIR) -type f -name "*.s";)
AUX_FILES = makefile
OUTPUT=$(BDIR)/kernel

CC=i686-elf-gcc
CXX=i686-elf-g++
NASM=nasm
AS=i686-elf-as
LD=$(CC)
WARNINGS=-Wall -Wextra
CFLAGS=-std=gnu99 -ffreestanding -O2 $(WARNINGS)
CXXFLAGS=-std=c++11 -ffreestanding -O2 $(WARNINGS) -I/usr/local/include #For Boost, TODO: Remove
LDFLAGS=-Tkernel.ld -ffreestanding -O2 -nostdlib $(WARNINGS) -lgcc
NASMFLAGS=-felf -Wall
ASFLAGS=-warn

BDIR=bin
ODIR=$(BDIR)/obj
_OBJ = $(patsubst %.c,%.o,$(SOURCES_C))
_OBJ += $(patsubst %.cpp,%.o,$(SOURCES_CXX))
_OBJ += $(patsubst %.asm,%.o,$(SOURCES_ASM))
_OBJ += $(patsubst %.asm,%.o,$(SOURCES_S))
OBJ = $(patsubst $(SDIR)/%,$(ODIR)/%,$(_OBJ))

ALLFILES = $(SOURCES_C) $(SOURCES_CXX) $(SOURCES_ASM) $(SOURCES_S) $(AUX_FILES)

.PHONY: all clean todolist run

all: $(OUTPUT)

clean:
	-rm -f $(OBJ) $(OUTPUT)

$(OUTPUT): $(OBJ)
	$(LD) $(LDFLAGS) -o $@ $(OBJ)

todolist:
	-@for file in $(ALLFILES:Makefile=); do fgrep -H -e TODO -e FIXME $$file; done; true

run: $(OUTPUT)
	./emu.sh

#handle .asm files with nasm
$(ODIR)/%.o: $(SDIR)/%.asm
	$(NASM) $(NASMFLAGS) $< -o $@

$(ODIR)/%.o: $(SDIR)/%.s
	$(AS) $(ASFLAGS) $< -o $@

$(ODIR)/%.o: $(SDIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

$(ODIR)/%.o: $(SDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

