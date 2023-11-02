#include <stdint.h>

#define SERIAL_PORT 0x3f8 

uint8_t inb(uint16_t port) {
    uint8_t out;
    asm("inb %1, %0": "=a"(out) : "d"(port));
    
    return out;
}

void outb(uint16_t port, uint8_t a) {
    asm("outb %0, %1" :: "a"(a), "d"(port));
}

void fserial_println(uint16_t port, char* string) {

    int i = 0;
    while (string[i] != '\0') {
        outb(port, string[i]);
        i++;
    }
    outb(port, '\r');
    outb(port, '\n');
}

int serial_recived(uint16_t port) {
    return inb(port + 5) & 1;
}

int is_transmit_empty(uint16_t port) {
    return inb(port + 5) & 0x20;
}

char read_serial(uint16_t port) {
    while(serial_recived(port) == 0);
    
    return inb(port);
}

void serial_print(uint16_t port, char a) {
    while(is_transmit_empty(port) == 0);
    
    outb(port, a);
}

void serial_println(uint16_t port, char* string) {
    int i = 0;
    while (string[i] != '\0') {
        while(is_transmit_empty(port) == 0);
        outb(port, string[i]);
        i++;
    }
    outb(port, '\r');
    outb(port, '\n');
}

void serial_printw(uint16_t port, char* string) {
    while(is_transmit_empty(port) == 0);

    int i = 0;
    while (string[i] != '\0') {
        outb(port, string[i]);
        i++;
    }
}