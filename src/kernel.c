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

    int r, g, b;

    for (int x = 0; x < width; x++)
    {
        r = (int)(255 * (float)x / width);
        g = (int)(255 * (1 - (float)x / width));
        b = 255;

        for (int y = 0; y < height; y++)
        {
            display_write_data(y * width + x, r, g, b);
        }
    }

    draw_rect(10, 10, width - 20, height - 20, 0, 0, 0, 1);

    for (int y = 11; y < height - 11; y++)
    {
        for (int x = 11; x < width - 11; x++)
        {
            if (x >= 10 && y >= 10 && x < width - 10 && y < height - 10)
            {
                int i = y * width + x;
                display_write_data(i, rand() % 256, rand() % 256, rand() % 256);
            }
        }
    }

    hcf();
}