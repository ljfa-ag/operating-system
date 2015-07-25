#include "video.h"
#include "gdt.h"
#include "idt.h"

using namespace os;

void static_construct();
void static_destruct();

extern "C" {

void handler()
{
    scr << "Interrupt test\n";
    video::letter ls[] = {
        {'l',fbcolor(0)},
        {'j',fbcolor(1)},
        {'f',fbcolor(2)},
        {'a',fbcolor(3)},
        {'-',fbcolor(4)},
        {'a',fbcolor(5)},
        {'g',fbcolor(6)},
    };
    scr.copy(ls, sizeof(ls)/sizeof(video::letter));
    scr.newline();
    scr.update_cursor();
}

void kernel_main()
{
    //Initialize
    static_construct();

    gdt_inst.init();
    
    idt_inst.init();
    idt_inst.load();
    idt::init_pic();
    //asm volatile("sti");
    
    const void* _esp;
    asm("mov %%esp, %0" : "=r" (_esp));
    scr << "Hello World!\n" << _esp << "\n\n";

    //Set IDT entry for interrupt 47 and call it
    idt_inst.set_entry(47, handler, 0x8, idt::interrupt_gate | idt::ring0 | idt::present);
    asm volatile("int $47");
    scr.newline();
    
    //Divide 105 by 19 and examine the result (quotient in eax, remainder in edx)
    asm volatile("mov $105, %%eax; cdq; mov $19, %%ebx; div %%ebx; int $48" :);
    
    scr << "\nI'm back!\n";
    
    //Clean up and halt
    static_destruct();
    while(true)
        asm volatile("hlt");
}

}
