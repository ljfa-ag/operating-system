#include "gdt.h"

namespace os {

gdt gdt_inst;

void gdt::init()
{
    set_entry(0, 0, 0, 0, 0);
    set_entry(1, 0, 0xfffff, segment | codeseg | present, gran_4k | _32_bit);
    set_entry(2, 0, 0xfffff, segment | dataseg | present, gran_4k | _32_bit);
    set_entry(3, 0, 0xfffff, segment | codeseg | present | ring3, gran_4k | _32_bit);
    set_entry(4, 0, 0xfffff, segment | dataseg | present | ring3, gran_4k | _32_bit);
    load();
}

void gdt::set_entry(int i, uint32_t base, uint32_t limit, uint8_t access, uint8_t gran)
{
    table[i].base_low = base & 0xffff;
    table[i].base_middle = (base >> 16) & 0xff;
    table[i].base_high = (base >> 24) & 0xff;
    
    table[i].limit_low = limit & 0xffff;
    table[i].granularity = (limit >> 16) & 0xf;
    
    table[i].granularity |= gran & 0xf0;
    table[i].access = access;
}

void gdt::load()
{
    struct
    {
        uint16_t limit;
        entry* base;
    } __attribute__((packed))
    gdtp = { 8*entries - 1, table };
    
    asm volatile(
        "lgdt %0;"
        "mov $0x10, %%ax;"
        "mov %%ax, %%ds;"
        "mov %%ax, %%es;"
        "mov %%ax, %%fs;"
        "mov %%ax, %%gs;"
        "mov %%ax, %%ss;"
        "ljmp $0x8, $.l;"
        ".l:"
        : : "m" (gdtp)
    );
}

} //namespace os
