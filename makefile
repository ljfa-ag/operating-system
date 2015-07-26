#SRCS = $(shell find -name '*.c' -or -name '*.cpp')
OBJS = start.o main.o constructors.o video.o idt.o gdt.o inthandler.o memcpy.o \
       intfwd.o

#CC   = gcc
LD   = ld
NASM = nasm
CXX  = g++

#CFLAGS    = -m32 -Wall -g -fno-stack-protector -std=c99
LDFLAGS   = -melf_i386 -Tkernel.ld
NASMFLAGS = -felf
CXXFLAGS_NOOPT = -m32 -Wall -g -fno-stack-protector -fno-use-cxa-atexit -nostdlib -fno-builtin -fno-rtti \
                 -fno-exceptions -ffreestanding -std=c++11 -I/usr/include/c++/4.8/i686-linux-gnu
CXXFLAGS  = $(CXXFLAGS_NOOPT) -O2

all: kernel

kernel: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $^

#%.o: %.c
#	$(CC) $(CFLAGS) -c -o $@ $<

%.o: %.asm
	$(NASM) $(NASMFLAGS) -o $@ $<

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

#Optimization with -O2 does not work properly for gdt.cpp due to inline asm
gdt.o:
	$(CXX) $(CXXFLAGS_NOOPT) -O1 -c -o $@ $<

clean:
	rm -f $(OBJS) kernel

dep:
	g++ -std=c++11 -MM *.cpp > depend

test: kernel
	./emu.sh

-include depend

.PHONY: clean dep test
