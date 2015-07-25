#include "idt.h"
#include "outb.h"

namespace os {

idt idt_inst;

void idt::set_entry(uint8_t i, void (*func)(), uint16_t selector, uint8_t flags)
{
    auto base = reinterpret_cast<uint32_t>(func);

    table[i].base_low = base & 0xffff;
    table[i].base_high = (base >> 16) & 0xffff;
    
    table[i].selector = selector;
    table[i].flags = flags;
}

void idt::init()
{

//Set the IDT entries for all interrupts from 0 to 48 to their stubs
#define IDT_ENTRY(z, n, unused) set_entry(n, int_stub_##n, 0x8, idt::interrupt_gate | idt::ring0 | idt::present);
BOOST_PP_REPEAT(49, IDT_ENTRY, ~)

}

void idt::load()
{
    struct
    {
        uint16_t limit;
        entry* base;
    } __attribute__((packed))
    idtp = { 8*entries - 1, table };
    
    asm volatile("lidt %0" : : "m" (idtp));
}

void idt::init_pic()
{
    // Initialize master PIC
    outb(0x20, 0x11); // PIC Initialization instruction
    outb(0x21, 0x20); // Number for IRQ 0
    outb(0x21, 0x04); // Slave on IRQ 2
    outb(0x21, 0x01); // ICW 4
     
    // Initialize slave PIC
    outb(0xa0, 0x11); // PIC Initialization instruction
    outb(0xa1, 0x28); // Number for IRQ 0
    outb(0xa1, 0x02); // Slave on IRQ 2
    outb(0xa1, 0x01); // ICW 4
     
    // Activate (demask) all IRQs
    outb(0x20, 0x0);
    outb(0xa0, 0x0);
}

}
