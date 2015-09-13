#ifndef INTHANDLER_H
#define INTHANDLER_H

#include "types.h"
#include "video.h"

namespace os
{

struct cpu_state
{
    // Manually saved registered
    uint32_t eax;
    uint32_t ebx;
    uint32_t ecx;
    uint32_t edx;
    uint32_t esi;
    uint32_t edi;
    uint32_t ebp;

    uint32_t intr;
    uint32_t error;

    // Registers saved by the CPU
    uint32_t eip;
    uint32_t cs;
    uint32_t eflags;
    uint32_t esp;
    uint32_t ss;
    
    // Writes the contents of the state on the screen
    void dump();
};

extern "C" void handle_interrupt(cpu_state* cpu);

}

#endif
