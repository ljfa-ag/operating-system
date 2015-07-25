#ifndef GDT_H
#define GDT_H

#include "types.h"

namespace os {

//The global descriptor table
class gdt
{
public:
    struct entry
    {
        uint16_t limit_low;
        uint16_t base_low;
        uint8_t base_middle;
        uint8_t access;
        uint8_t granularity;
        uint8_t base_high;
    } __attribute__((packed));
    
    enum flags : uint8_t
    {
        dataseg = 0x02,
        codeseg = 0x0a,
        tss = 0x09,
        
        segment = 0x10,
        ring0 = 0x00,
        ring3 = 0x60,
        present = 0x80
    };
    
    enum gran : uint8_t
    {
        gran_4k = 0x80,
        _32_bit = 0x40
    };
    
    static const int entries = 5;
    entry table[entries];

    //Sets an entry
    void set_entry(int i, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran);
    //Loads the GDT into the processor
    void load();
    //Initializes the GDT entries
    void init();
};

//The instance of the GDT
extern gdt gdt_inst;

} //namespace os

#endif
