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
#include "ktools/irqs.h"
#include "ktools/ktools.h"

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
    println_ok("Initialized Display.");

    idt_init();
    println_ok("Initialized IDT..");

    i8259_Configure(PIC_REMAP_OFFSET, PIC_REMAP_OFFSET + 8, false);
    println_ok("Initialized PIC Controller");

    pit_init();
    println_ok("Initialized PIT Controller"); 

    init_periodic_event();
    println_ok("Initialized Periodic Event"); 

    keyboard_init();
    println_ok("Initialized Keyboard.");

    init_os_interupts();
    println_ok("Registered OS Interupts");

    int width = getWidth();
    println_ok("Window Width Saved");

    int height = getHeight();   
    println_ok("Window Height Saved");
    
    enableTextBuffer();

    println_ok("Enabled Text Buffer");

    echoText = 0;

    println_ok("Disabled Text Output");

    println("");
    println("        /      _/./");
    println("   ,-'    `-:.,-'/");
    println("  > O )<)    _  (");
    println("   `-._  _.:' `-.\\");
    println("       `` \\;");
    println("");
    println_color("Piece Of Shit - version 0.1", 66, 135, 245);
    hlt();
}
