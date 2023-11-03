struct limine_framebuffer *framebuffer;
volatile uint32_t *fb_ptr;

void init_display(volatile struct limine_framebuffer_request framebuffer_request) {
    framebuffer = framebuffer_request.response->framebuffers[0];
    fb_ptr = framebuffer->address;
}

unsigned int rgbth(uint8_t red, uint8_t green, uint8_t blue) {
    return (0xFF << 24) | (red << 16) | (green << 8) | blue;
}

void draw_pixel(uint64_t x, uint64_t y, uint8_t r, uint8_t b, uint8_t g) {
    fb_ptr[y * framebuffer->width + x] = rgbth(r, g, b);
}

void set_background_color(uint16_t red, uint16_t green, uint16_t blue) {
    for (size_t x = 0; x < framebuffer->width; x++) {
        for(size_t y = 0; y < framebuffer->height; y++) {
            draw_pixel(x, y, red, green, blue);
        }
    }
}

void display_write_data(uint32_t address, uint8_t red, uint8_t green, uint8_t blue) {
    fb_ptr[address] = rgbth(red, green, blue);
}

int getHeight() {
    return framebuffer->height;
}

int getWidth() {
    return framebuffer->width;
}

void draw_line(uint64_t x1, uint64_t y1, uint64_t x2, uint64_t y2, uint8_t r, uint8_t g, uint8_t b) {
    int64_t u_llabs(int64_t num) {
        return (num < 0) ? -num : num;
    }
    
    int64_t dx = x2 - x1;
    int64_t dy = y2 - y1;
    int64_t dxabs = u_llabs(dx);
    int64_t dyabs = u_llabs(dy);
    int64_t sdx = (x2 > x1) ? 1 : -1;
    int64_t sdy = (y2 > y1) ? 1 : -1;
    int64_t x = dyabs / 2;
    int64_t y = dxabs / 2;
    int64_t px = x1;
    int64_t py = y1;

    if (dxabs >= dyabs) {
        for (int64_t i = 0; i < dxabs; i++) {
            y += dyabs;
            if (y >= dxabs) {
                y -= dxabs;
                py += sdy;
            }
            px += sdx;
            draw_pixel(px, py, r, g, b);
        }
    } else {
        for (int64_t i = 0; i < dyabs; i++) {
            x += dxabs;
            if (x >= dyabs) {
                x -= dyabs;
                px += sdx;
            }
            py += sdy;
            draw_pixel(px, py, r, g, b);
        }
    }
}

void draw_rect(uint64_t x, uint64_t y, uint64_t width, uint64_t height, uint8_t r, uint8_t g, uint8_t b, int filled) {
    if (filled) {
        for (size_t i = x; i < x + width; i++) {
            for (size_t j = y; j < y + height; j++) {
                draw_pixel(i, j, r, g, b);
            }
        }
    } else {
        for (size_t i = x; i < x + width; i++) {
            draw_pixel(i, y, r, g, b);
            draw_pixel(i, y + height - 1, r, g, b);
        }
        for (size_t j = y; j < y + height; j++) {
            draw_pixel(x, j, r, g, b);
            draw_pixel(x + width - 1, j, r, g, b);
        }
    }
}

void draw_circle(uint64_t centerX, uint64_t centerY, uint64_t radius, uint8_t r, uint8_t g, uint8_t b, int filled) {
    int64_t x = radius - 1;
    int64_t y = 0;
    int64_t dx = 1;
    int64_t dy = 1;
    int64_t err = dx - (radius << 1);

    while (x >= y) {
        if (filled) {
            for (int64_t i = centerX - x; i <= centerX + x; i++) {
                draw_pixel(i, centerY + y, r, g, b);
                draw_pixel(i, centerY - y, r, g, b);
            }
            for (int64_t i = centerX - y; i <= centerX + y; i++) {
                draw_pixel(i, centerY + x, r, g, b);
                draw_pixel(i, centerY - x, r, g, b);
            }
        } else {
            draw_pixel(centerX + x, centerY + y, r, g, b);
            draw_pixel(centerX - x, centerY + y, r, g, b);
            draw_pixel(centerX + x, centerY - y, r, g, b);
            draw_pixel(centerX - x, centerY - y, r, g, b);
            draw_pixel(centerX + y, centerY + x, r, g, b);
            draw_pixel(centerX - y, centerY + x, r, g, b);
            draw_pixel(centerX + y, centerY - x, r, g, b);
            draw_pixel(centerX - y, centerY - x, r, g, b);
        }

        if (err <= 0) {
            y++;
            err += dy;
            dy += 2;
        }
        if (err > 0) {
            x--;
            dx += 2;
            err += dx - (radius << 1);
        }
    }
}

void draw_triangle(uint64_t x1, uint64_t y1, uint64_t x2, uint64_t y2, uint64_t x3, uint64_t y3, uint8_t r, uint8_t g, uint8_t b, int filled) {
    if (filled) {
        // Define the bounding box for the triangle
        int minX = x1 < x2 ? (x1 < x3 ? x1 : x3) : (x2 < x3 ? x2 : x3);
        int maxX = x1 > x2 ? (x1 > x3 ? x1 : x3) : (x2 > x3 ? x2 : x3);
        int minY = y1 < y2 ? (y1 < y3 ? y1 : y3) : (y2 < y3 ? y2 : y3);
        int maxY = y1 > y2 ? (y1 > y3 ? y1 : y3) : (y2 > y3 ? y2 : y3);

        for (int y = minY; y <= maxY; y++) {
            for (int x = minX; x <= maxX; x++) {
                int w0 = (x - x2) * (y3 - y2) - (y - y2) * (x3 - x2);
                int w1 = (x - x3) * (y1 - y3) - (y - y3) * (x1 - x3);
                int w2 = (x - x1) * (y2 - y1) - (y - y1) * (x2 - x1);

                if ((w0 >= 0 && w1 >= 0 && w2 >= 0) || (w0 <= 0 && w1 <= 0 && w2 <= 0)) {
                    draw_pixel(x, y, r, g, b);
                }
            }
        }
    } else {
        draw_line(x1, y1, x2, y2, r, g, b);
        draw_line(x2, y2, x3, y3, r, g, b);
        draw_line(x3, y3, x1, y1, r, g, b);
    }
}
