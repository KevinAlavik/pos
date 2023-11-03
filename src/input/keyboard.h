#include <libkrnl/arch/x86/io/portio.h>
#include "ktools/pic-controller.h"

#define PS2_COMMAND 0x64
#define PS2_DATA 0x60

char *us_layout[0xFF + 1] = {
    [0x01] = "", // Escape
    [0x02] = "1",
    [0x03] = "2",
    [0x04] = "3",
    [0x05] = "4",
    [0x06] = "5",
    [0x07] = "6",
    [0x08] = "7",
    [0x09] = "8",
    [0x0A] = "9",
    [0x0B] = "0",
    [0x0C] = "-",
    [0x0D] = "=",
    [0x0E] = "\b \b", // Backspace
    [0x0F] = "\t",    // Tab
    [0x10] = "Q",
    [0x11] = "W",
    [0x12] = "E",
    [0x13] = "R",
    [0x14] = "T",
    [0x15] = "Y",
    [0x16] = "U",
    [0x17] = "I",
    [0x18] = "O",
    [0x19] = "P",
    [0x1A] = "[",
    [0x1B] = "]",
    [0x1C] = "\n", // Enter
    [0x1D] = "",   // Left Control
    [0x1E] = "A",
    [0x1F] = "S",
    [0x20] = "D",
    [0x21] = "F",
    [0x22] = "G",
    [0x23] = "H",
    [0x24] = "J",
    [0x25] = "K",
    [0x26] = "L",
    [0x27] = ";",
    [0x28] = "'",
    [0x29] = "`",
    [0x2A] = "", // Left Shift
    [0x2B] = "\\",
    [0x2C] = "Z",
    [0x2D] = "X",
    [0x2E] = "C",
    [0x2F] = "V",
    [0x30] = "B",
    [0x31] = "N",
    [0x32] = "M",
    [0x33] = ",",
    [0x34] = ".",
    [0x35] = "/",
    [0x36] = "",  // Right Shift
    [0x37] = "*", // Keypad *
    [0x38] = "",  // Left Alt
    [0x39] = " ", // Space
    [0x3A] = "",  // Caps Lock
    [0x3B] = "",  // F1
    [0x3C] = "",  // F2
    [0x3D] = "",  // F3
    [0x3E] = "",  // F4
    [0x3F] = "",  // F5
    [0x40] = "",  // F6
    [0x41] = "",  // F7
    [0x42] = "",  // F8
    [0x43] = "",  // F9
    [0x44] = "",  // F10
    [0x45] = "",  // Num Lock
    [0x46] = "",  // Scroll Lock
    [0x47] = "7", // Keypad 7
    [0x48] = "8", // Keypad 8
    [0x49] = "9", // Keypad 9
    [0x4A] = "-", // Keypad -
    [0x4B] = "4", // Keypad 4
    [0x4C] = "5", // Keypad 5
    [0x4D] = "6", // Keypad 6
    [0x4E] = "+", // Keypad +
    [0x4F] = "1", // Keypad 1
    [0x50] = "2", // Keypad 2
    [0x51] = "3", // Keypad 3
    [0x52] = "0", // Keypad 0
    [0x53] = ".", // Keypad .
    [0x57] = "",  // F11
    [0x58] = "",  // F12
    [0x59 ... 0xFF] = {""}};

char *se_layout[0xFF + 1] = {
    [0x01] = "Escaped", // Escape
    [0x02] = "1",
    [0x03] = "2",
    [0x04] = "3",
    [0x05] = "4",
    [0x06] = "5",
    [0x07] = "6",
    [0x08] = "7",
    [0x09] = "8",
    [0x0A] = "9",
    [0x0B] = "0",
    [0x0C] = "+",
    [0x0D] = "´",
    [0x0E] = "\b \b", // Backspace
    [0x0F] = "\t",    // Tab
    [0x10] = "Q",
    [0x11] = "W",
    [0x12] = "E",
    [0x13] = "R",
    [0x14] = "T",
    [0x15] = "Y",
    [0x16] = "U",
    [0x17] = "I",
    [0x18] = "O",
    [0x19] = "P",
    [0x1A] = "Å",
    [0x1B] = "¨",
    [0x1C] = "\n", // Enter
    [0x1D] = "Left Control",
    [0x1E] = "A",
    [0x1F] = "S",
    [0x20] = "D",
    [0x21] = "F",
    [0x22] = "G",
    [0x23] = "H",
    [0x24] = "J",
    [0x25] = "K",
    [0x26] = "L",
    [0x27] = "Ö",
    [0x28] = "Ä",
    [0x29] = "´",
    [0x2A] = "Left Shift",
    [0x2B] = "<",
    [0x2C] = "Z",
    [0x2D] = "X",
    [0x2E] = "C",
    [0x2F] = "V",
    [0x30] = "B",
    [0x31] = "N",
    [0x32] = "M",
    [0x33] = ",",
    [0x34] = ".",
    [0x35] = "-",
    [0x36] = "Right Shift",
    [0x37] = "Keypad *",
    [0x38] = "Left Alt",
    [0x39] = " ",
    [0x3A] = "Caps Lock",
    [0x3B] = "F1",
    [0x3C] = "F2",
    [0x3D] = "F3",
    [0x3E] = "F4",
    [0x3F] = "F5",
    [0x40] = "F6",
    [0x41] = "F7",
    [0x42] = "F8",
    [0x43] = "F9",
    [0x44] = "F10",
    [0x45] = "Num Lock",
    [0x46] = "Scroll Lock",
    [0x47] = "Keypad 7",
    [0x48] = "Keypad 8",
    [0x49] = "Keypad 9",
    [0x4A] = "Keypad -",
    [0x4B] = "\b",
    [0x4C] = "Keypad 5",
    [0x4D] = "Keypad 6",
    [0x4E] = "Keypad +",
    [0x4F] = "Keypad 1",
    [0x50] = "Keypad 2",
    [0x51] = "Keypad 3",
    [0x52] = "Keypad 0",
    [0x53] = "Keypad .",
    [0x57] = "F11",
    [0x58] = "F12",
    [0x59 ... 0xFF] = {""}
};

char *se_layout_print[0xFF + 1] = {
    [0x01] = NULL_CHAR, // Escape
    [0x02] = NUM1,
    [0x03] = NUM2,
    [0x04] = NUM3,
    [0x05] = NUM4,
    [0x06] = NUM5,
    [0x07] = NUM6,
    [0x08] = NUM7,
    [0x09] = NUM8,
    [0x0A] = NUM9,
    [0x0B] = NUM0,
    [0x0C] = SYM_PLUS,
    [0x0D] = NULL_CHAR,
    [0x0E] = EMPTY_CHAR, // Backspace
    [0x0F] = NULL_CHAR,  // Tab
    [0x10] = Q,
    [0x11] = W,
    [0x12] = E,
    [0x13] = R,
    [0x14] = T,
    [0x15] = Y,
    [0x16] = U,
    [0x17] = I,
    [0x18] = O,
    [0x19] = P,
    [0x1A] = NULL_CHAR,
    [0x1B] = NULL_CHAR,
    [0x1C] = NULL_CHAR, // Enter
    [0x1D] = NULL_CHAR,
    [0x1E] = A,
    [0x1F] = S,
    [0x20] = D,
    [0x21] = F,
    [0x22] = G,
    [0x23] = H,
    [0x24] = J,
    [0x25] = K,
    [0x26] = L,
    [0x27] = NULL_CHAR,
    [0x28] = NULL_CHAR,
    [0x29] = NULL_CHAR,
    [0x2A] = NULL_CHAR,
    [0x2B] = SYM_LT,
    [0x2C] = Z,
    [0x2D] = X,
    [0x2E] = C,
    [0x2F] = V,
    [0x30] = B,
    [0x31] = N,
    [0x32] = M,
    [0x33] = NULL_CHAR,
    [0x34] = DOT,
    [0x35] = SYM_DAH,
    [0x36] = NULL_CHAR,
    [0x37] = NULL_CHAR,
    [0x38] = NULL_CHAR,
    [0x39] = EMPTY_CHAR,
    [0x3A] = NULL_CHAR,
    [0x3B] = NULL_CHAR,
    [0x3C] = NULL_CHAR,
    [0x3D] = NULL_CHAR,
    [0x3E] = NULL_CHAR,
    [0x3F] = NULL_CHAR,
    [0x40] = NULL_CHAR,
    [0x41] = NULL_CHAR,
    [0x42] = NULL_CHAR,
    [0x43] = NULL_CHAR,
    [0x44] = NULL_CHAR,
    [0x45] = NULL_CHAR,
    [0x46] = NULL_CHAR,
    [0x47] = NULL_CHAR,
    [0x48] = NULL_CHAR,
    [0x49] = NULL_CHAR,
    [0x4A] = SYM_DAH,
    [0x4B] = NULL_CHAR,
    [0x4C] = NULL_CHAR,
    [0x4D] = NULL_CHAR,
    [0x4E] = NULL_CHAR,
    [0x4F] = NULL_CHAR,
    [0x50] = NULL_CHAR,
    [0x51] = NULL_CHAR,
    [0x52] = NULL_CHAR,
    [0x53] = NULL_CHAR,
    [0x57] = NULL_CHAR,
    [0x58] = NULL_CHAR,
    [0x59 ... 0xFF] = {""}};

int letterWidth = 8;
int letterHeight = 16;

int letterSpacing = 5;
int ammountOfLettersOnScreen = 0;
int ammountOfLines = 0;

int letterStartX = 5;
int letterY = 5;

__attribute__((interrupt)) void keyboard_handler(void *)
{
    uint8_t data = inb8(PS2_DATA);

    if (data == 0)
    {
        invalid_keyboard_data();
    }

    if (se_layout_print[data] != "")
    {
        if ((letterStartX + (letterWidth + letterSpacing) * ammountOfLettersOnScreen) + letterWidth > getWidth())
        {
            ammountOfLines++;
            letterY = 16 * ammountOfLines;
        }

        if (data == 14 || data == 142)
        { 
            ammountOfLettersOnScreen-=1;
            draw_letter(se_layout_print[14], letterStartX + (letterWidth + letterSpacing) * ammountOfLettersOnScreen, letterY, 255, 255, 255);
        }
        else
        {
            draw_letter(se_layout_print[data], letterStartX + (letterWidth + letterSpacing) * ammountOfLettersOnScreen, letterY, 255, 255, 255);
            ammountOfLettersOnScreen += 1;
        }
    }

    i8259_SendEndOfInterrupt(1);
}

void keyboard_init()
{
    set_idt_gate(0x21, (uint64_t)&keyboard_handler, 0x28, 0x8E);

    if (inb8(PS2_COMMAND) & 0x1)
        inb8(PS2_DATA);
    outb8(PS2_COMMAND, 0x60);
    outb8(PS2_DATA, 0b01100111);

    outb8(PS2_COMMAND, 0xAE);
    outb8(PS2_DATA, 0xf4);
    i8259_Unmask(1);
}
