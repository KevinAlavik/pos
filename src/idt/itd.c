#include "idt.h"

idt_t idt[256];

void interrupt_wipe() {
    for (int i = 0; i < 256; ++i) {
        idt[i].offset_low = 0;
        idt[i].selector = 0;
        idt[i].stack_table = 0;
        idt[i].flags = 0;
        idt[i].offset_middle = 0;
        idt[i].offset_high = 0;
        idt[i].reserved = 0;
    }
}

void interrupt_flush() {
    descriptor_t descriptor;
    descriptor.limit = sizeof(idt) - 1;
    descriptor.base = (uint64_t)&idt;

    asm volatile ("lidt %0" :: "m"(descriptor) : "memory");
}

void interrupt_register(uint8_t vector, void *interrupt_handler, uint8_t flags) {
    uint64_t handler_interrupt = (uint64_t)interrupt_handler;

    idt[vector].offset_low = (uint16_t)handler_interrupt;
    idt[vector].offset_middle = (uint16_t)(handler_interrupt >> 16);
    idt[vector].offset_high = (uint32_t)(handler_interrupt >> 32);
    idt[vector].selector = 0x28;
    idt[vector].stack_table = 0;
    idt[vector].flags = flags;
    idt[vector].reserved = 0;
}