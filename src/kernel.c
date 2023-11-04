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

    serial_printw(SERIAL_PORT, "\e[1;1H\e[2J");

    init_display(framebuffer_request);
    logger_ok("Initialized Display.");

    idt_init();
    logger_ok("Initialized IDT..");

    i8259_Configure(PIC_REMAP_OFFSET, PIC_REMAP_OFFSET + 8, false);
    logger_ok("Initialized PIC Controller");

    keyboard_init();
    logger_ok("Initialized Keyboard.");

    letterSpacing = printfLetterSpacing;

    init_os_interupts();
    logger_dbg("Registered OS Interupts");

    int width = getWidth();
    int height = getHeight();

    // int hello[] = {P, O, S, EMPTY_CHAR, SYM_LP, V, NUM0, DOT, NUM0, DOT, NUM1, SYM_RP};

    // printf(hello, sizeof(hello) / sizeof(hello[0]), printfLetterSpacing, printfLetterSpacing, 255, 255, 255);

    // int seperator[] = { SYM_US, SYM_US, SYM_US, SYM_US, SYM_US, SYM_US, SYM_US, SYM_US, SYM_US, SYM_US, SYM_US, SYM_US };

    // printf(seperator, sizeof(seperator) / sizeof(seperator[0]), printfLetterSpacing, (printfLetterSpacing + 8) * 2 , 190, 190, 190);

    hlt();
}