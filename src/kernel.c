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
#include "input/keyboard.h"

// Kernel Utilities
#include <libkrnl/essentials.h>
#include <libkrnl/arch/x86/cpu.h>
#include <libkrnl/arch/x86/power.h>
#include <libkrnl/arch/x86/rtc.h>
/* #include <libkrnl/arch/x86/io/portio.h> */

// Kernel Tools
/* #include "ktools/pic-controller.h" */

// Other Utilities
#include "utilities/math.h"
#include "utilities/tools.h"
#include "serial/logger.h"
#include "utilities/errors.h"

static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0};

__attribute__((interrupt)) void division_by_zero_error(void*) {
    logger_err("Division by zero!");
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
    logger_ok("Initialized Display.");

    idt_init();
    logger_ok("Initialized IDT..");

    i8259_Configure(PIC_REMAP_OFFSET, PIC_REMAP_OFFSET + 8, false);
    logger_ok("Initialized PIC Controller");

    keyboard_init();
    logger_ok("Initialized Keyboard..");

    int width = getWidth();
    int height = getHeight();

    // set_idt_gate(0, (uint64_t)&division_by_zero_error, 0x28, 0x8E);

    // logger_dbg("Registered \"division by zero\" interupt.");
    // logger_dbg("Triggering \"division by zero\" interupt.");    

    // division_by_zero();

    hcf();
}