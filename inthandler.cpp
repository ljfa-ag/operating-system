#include "inthandler.h"
#include "outb.h"

namespace os
{

extern "C" void handle_interrupt(cpu_state* cpu)
{
    if(cpu->intr <= 0x1f)
    {
        scr << "Exception " << cpu->intr << ", Kernel halted!\Error code 0x";
        scr.putu(cpu->error, 16, 8);
        scr << '\n';
        cpu->dump();
        asm volatile("cli; hlt");
    }
    else
    {
        if(cpu->intr >= 0x20 && cpu->intr <= 0x2f)
        {
            outb(0x20, 0x20);
            if(cpu->intr >= 0x28)
            {
                outb(0xa0, 0x20);
            }
        }
        if(cpu->intr == 48)
        {
            scr << "Syscall.\n";
            cpu->dump();
        }
        else
            scr << "Interrupt " << cpu->intr << ".\n";
    }
}

void cpu_state::dump()
{
    scr << "eax: 0x";
    scr.putu(eax, 16, 8);
    scr << "  ebx: 0x";
    scr.putu(ebx, 16, 8);
    scr << "\necx: 0x";
    scr.putu(ecx, 16, 8);
    scr << "  edx: 0x";
    scr.putu(edx, 16, 8);
    scr << "\nesi: 0x";
    scr.putu(esi, 16, 8);
    scr << "  edi: 0x";
    scr.putu(edi, 16, 8);
    scr << "\nebp: 0x";
    scr.putu(ebp, 16, 8);
    scr << "  eip: 0x";
    scr.putu(eip, 16, 8);
    scr << "\n cs: 0x";
    scr.putu(cs, 16, 8);
    scr << "   ss: 0x";
    scr.putu(ss, 16, 8);
    scr << "\nesp: 0x";
    scr.putu(esp, 16, 8);
    scr << "\neflags: 0x";
    scr.putu(eflags, 16, 8);
    scr << " (";
    scr.putu(eflags, 2, 32);
    scr << ")\n";
}

}
