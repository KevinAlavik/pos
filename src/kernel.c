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
#include "utilities/tools.h"

static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0};

__attribute__((interrupt)) void division_by_zero_error(void*) {
    serial_println(SERIAL_PORT, "division by zero.");
    set_background_color(255, 0, 0);
    hcf();
}

void _start(void)
{
    sys_init_fpu();

    if (framebuffer_request.response == NULL || framebuffer_request.response->framebuffer_count < 1)
    {
        hcf();
    }

    serial_printw(SERIAL_PORT, "\e[1;1H\e[2J");
    init_display(framebuffer_request);
    serial_println(SERIAL_PORT, "[ \e[0;32m OK \e[0m ] Initialized display driver.");
    idt_init();
    serial_println(SERIAL_PORT, "[ \e[0;32m OK \e[0m ] Initialized IDT.");

    int width = getWidth();
    int height = getHeight();

    set_idt_gate(0, (uint64_t)&division_by_zero_error, 0x28, 0x8E);

    serial_println(SERIAL_PORT, "[ \e[0;32m OK \e[0m ] Registered interupts.");
    serial_println(SERIAL_PORT, "[ \e[0;32m OK \e[0m ] Triggering interupts.");

    division_by_zero();
    hcf();
}