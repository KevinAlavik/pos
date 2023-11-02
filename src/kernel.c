// Standard Libraries
#include <stdint.h>
#include <stddef.h>

// Kernel Dependecies
#include "utilities/limine.h"

// Kernel Imports
#include "display/display.h"
#include "display/font.h"
#include "serial/serial.h"
#include "mm/mm.h"
#include "idt/idt.h"

// Kernel Utilities
#include <libkrnl/essentials.h>
#include <libkrnl/arch/x86/cpu.h>
#include <libkrnl/arch/x86/power.h>
#include <libkrnl/arch/x86/rtc.h>

// Other Utilities
#include "utilities/math.h"

static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0};

void interrupt0(void *frame) {
    (void)frame;
    serial_println(SERIAL_PORT, "interupt called");
}

void _start(void)
{
    sys_init_fpu();

    if (framebuffer_request.response == NULL || framebuffer_request.response->framebuffer_count < 1)
    {
        hcf();
    }

    init_display(framebuffer_request);
    serial_printw(SERIAL_PORT, "\e[1;1H\e[2J");
    serial_println(SERIAL_PORT, "[ \e[0;32m OK \e[0m ] Display driver.");

    int width = getWidth();
    int height = getHeight();

    interrupt_wipe();   

    interrupt_register(0, &interrupt0, 0x8E);
    interrupt_flush();

    asm volatile ("int $0");
    hcf();
}