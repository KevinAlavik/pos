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
    int width = getWidth();
    int height = getHeight();

    for (uint64_t i = 0; i < width * height; i++) {
        uint8_t red = rand() % 256;
        uint8_t green = rand() % 256;
        uint8_t blue = rand() % 256;
        draw_pixel(i % width, i / width, red, green, blue);
    }

    hcf();
}