#ifndef IDT_H
#define IDT_H

#include <stdint.h>

typedef struct
{
    uint16_t offset_low;
    uint16_t selector;
    uint8_t stack_table;
    uint8_t flags;
    uint16_t offset_middle;
    uint32_t offset_high;
    uint32_t reserved;
} idt_t;

typedef struct
{
    uint16_t limit;
    uint64_t base;
} descriptor_t;

extern idt_t idt[256];

void interrupt_wipe();
void interrupt_flush();
void interrupt_register(uint8_t vector, void *interrupt_handler, uint8_t flags);

#endif // IDT_H
