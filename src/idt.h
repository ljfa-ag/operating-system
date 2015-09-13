#ifndef IDT_H
#define IDT_H

#include "types.h"
#include "outb.h"

#include <boost/preprocessor/repetition.hpp>

extern "C" {

//Declare interrupt handler stubs for all interrupts from 0 to 48.
//The handler stubs are defined in assembly.
#define INT_STUB_DECL(z, n, unused) void int_stub_##n ();
BOOST_PP_REPEAT(49, INT_STUB_DECL, ~)

}

namespace os {

//The interrupt descriptor table
class idt
{
public:
    struct entry
    {
        uint16_t base_low = 0;
        uint16_t selector = 0;
        uint8_t zero = 0;
        uint8_t flags = 0;
        uint16_t base_high = 0;
    } __attribute__((packed));
    
    enum flags : uint8_t
    {
        interrupt_gate = 0x0e,
        present = 0x80,
        ring0 = 0x00,
        ring3 = 0x60
    };
    
    static const int entries = 256;
    entry table[entries];
    
    //Sets an entry
    void set_entry(uint8_t i, void (*func)(), uint16_t selector, uint8_t flags);
    //Initializes the IDT entries with
    void init();
    //Loads the IDT into the processor
    void load();
    
    static void init_pic();
};

//The instance of the IDT
extern idt idt_inst;

}

#endif
