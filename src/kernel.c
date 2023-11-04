// Standard Libraries
#include <stdint.h>
#include <stddef.h>

// Kernel Dependecies
#include "utilities/limine.h"

// Kernel Imports
#include "display/display.h"
#include "display/bitmap.h"
#include "serial/serial.h"
#include "mm/mm.h"
#include "idt/idt.h"
#include "input/keyboard.h"
#include "sound/sound.h"

// Kernel Utilities
#include <libkrnl/essentials.h>
#include <libkrnl/arch/x86/cpu.h>
#include <libkrnl/arch/x86/power.h>
#include <libkrnl/klibc/memory.h>
#include <libkrnl/arch/x86/rtc.h>
/* #include <libkrnl/arch/x86/io/portio.h> */

// Kernel Tools
#include "ktools/interupts.h"
#include "ktools/ktools.h"
/* #include "ktools/pic-controller.h" */

// Other Utilities
#include "utilities/math.h"
#include "utilities/tools.h"
#include "serial/logger.h"
#include "utilities/errors.h"

static volatile struct limine_framebuffer_request framebuffer_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0};

void _start(void)
{
    sys_init_fpu();

    if (framebuffer_request.response == NULL || framebuffer_request.response->framebuffer_count < 1)
    {
        hcf();
    }

    // serial_printw(SERIAL_PORT, "\e[1;1H\e[2J");

    init_display(framebuffer_request);
    println_ok("Initialized Display.");

    idt_init();
    println_ok("Initialized IDT..");

    i8259_Configure(PIC_REMAP_OFFSET, PIC_REMAP_OFFSET + 8, false);
    println_ok("Initialized PIC Controller");

    keyboard_init();
    println_ok("Initialized Keyboard.");

    letterSpacing = printfLetterSpacing;

    init_os_interupts();
    println_dbg("Registered OS Interupts");

    int width = getWidth();
    int height = getHeight();

    println(" ");
    println("Welcome! (type help for command list)");
    
    hlt();
}