#include <libkrnl/arch/x86/io/portio.h>
#include "ktools/pic-controller.h"

#define PS2_COMMAND     0x64
#define PS2_DATA        0x60

__attribute__((interrupt)) void keyboard_handler(void*) {
    uint8_t data = inb8(PS2_DATA);

    if(data == 0) {
        invalid_keyboard_data();
    }
        
    outb8(SERIAL_PORT, data);

    i8259_SendEndOfInterrupt(1);
}

void keyboard_init() {
    set_idt_gate(0x21, (uint64_t)&keyboard_handler, 0x28, 0x8E);

    if (inb8(PS2_COMMAND) & 0x1) // initialize the ps2 controller
        inb8(PS2_DATA);
    outb8(PS2_COMMAND, 0x60);
    outb8(PS2_DATA, 0b01100111);

    outb8(PS2_COMMAND, 0xAE);
    outb8(PS2_DATA, 0xf4);
    i8259_Unmask(1);
}
