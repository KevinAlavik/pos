// Standard Libraries
#include <stdint.h>
#include <stddef.h>
#include "limine.h"

// Drivers
#include "drivers/serial.h"
#include "drivers/display.h"
#include "drivers/font.h"

// Utilities
#include <libkrnl/essentials.h>
#include <libkrnl/arch/x86/cpu.h>
#include <libkrnl/arch/x86/power.h>
#include <libkrnl/arch/x86/rtc.h>

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
    serial_println(SERIAL_PORT, "[ \e[0;32m OK \e[0m ] Initialized serial driver.");

    init_display(framebuffer_request);

    serial_println(SERIAL_PORT, "[ \e[0;32m OK \e[0m ] Initialized display driver.");
    set_background_color(0, 0, 0);
    int width = getWidth();
    int height = getHeight();

    for(int i = 0; i < height - i; i++) {  
        draw_circle(width / 2, height / 2, i, 255, 255, 255, 1);
    }

    draw_circle(width / 2, height / 2, 200, 0, 0, 0, 1);

    hcf();
}