#include <stdint.h>
#include "display/font.h"

int allow_overlapping_char = 0;

void draw_letter(int letterIndex, int x, int y, int r, int g, int b)
{
    if (allow_overlapping_char == 0)
    {
        clear_area(x, y, 8, 16);
    }

    for (size_t yi = 0; yi < 16; yi++)
    {
        for (size_t xi = 0; xi < 8; xi++)
        {
            uint8_t bit = font[letterIndex][yi] & (0x80 >> xi);
            if (bit != 0)
            {
                draw_pixel(x + xi, y + yi, r, b, g);
            }
        }
    }
}
