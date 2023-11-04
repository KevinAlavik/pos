#include <libkrnl/arch/x86/io/portio.h>
#include "ktools/pic-controller.h"

#define PS2_COMMAND 0x64
#define PS2_DATA 0x60

char *se_layout_lower[0xFF + 1] = {
    [0x01] = "Escaped", // Escape
    [0x02] = "\x31",    // ASCII for '1'
    [0x03] = "\x32",    // ASCII for '2'
    [0x04] = "\x33",    // ASCII for '3'
    [0x05] = "\x34",    // ASCII for '4'
    [0x06] = "\x35",    // ASCII for '5'
    [0x07] = "\x36",    // ASCII for '6'
    [0x08] = "\x37",    // ASCII for '7'
    [0x09] = "\x38",    // ASCII for '8'
    [0x0A] = "\x39",    // ASCII for '9'
    [0x0B] = "\x30",    // ASCII for '0'
    [0x0C] = "+",
    [0x0D] = "´",
    [0x0E] = "",     // Backspace
    [0x0F] = "    ", // Tab
    [0x10] = "q",    // ASCII for 'q'
    [0x11] = "w",    // ASCII for 'w'
    [0x12] = "e",    // ASCII for 'e'
    [0x13] = "r",    // ASCII for 'r'
    [0x14] = "t",    // ASCII for 't'
    [0x15] = "y",    // ASCII for 'y'
    [0x16] = "u",    // ASCII for 'u'
    [0x17] = "i",    // ASCII for 'i'
    [0x18] = "o",    // ASCII for 'o'
    [0x19] = "p",    // ASCII for 'p'
    [0x1A] = "å",    // ASCII for 'å'
    [0x1B] = "¨",
    [0x1C] = "",  // Enter
    [0x1D] = "",  // Left controll
    [0x1E] = "a", // ASCII for 'a'
    [0x1F] = "s", // ASCII for 's'
    [0x20] = "d", // ASCII for 'd'
    [0x21] = "f", // ASCII for 'f'
    [0x22] = "g", // ASCII for 'g'
    [0x23] = "h", // ASCII for 'h'
    [0x24] = "j", // ASCII for 'j'
    [0x25] = "k", // ASCII for 'k'
    [0x26] = "l", // ASCII for 'l'
    [0x27] = "ö", // ASCII for 'ö'
    [0x28] = "ä", // ASCII for 'ä'
    [0x29] = "´",
    [0x2A] = "", // Left shift
    [0x2B] = "<",
    [0x2C] = "z", // ASCII for 'z'
    [0x2D] = "x", // ASCII for 'x'
    [0x2E] = "c", // ASCII for 'c'
    [0x2F] = "v", // ASCII for 'v'
    [0x30] = "b", // ASCII for 'b'
    [0x31] = "n", // ASCII for 'n'
    [0x32] = "m", // ASCII for 'm'
    [0x33] = ",", // ASCII for ','
    [0x34] = ".", // ASCII for '.'
    [0x35] = "-", // ASCII for '-'
    [0x36] = "",  // Right shift
    [0x37] = "*",
    [0x38] = "",  // Left alt
    [0x39] = " ", // ASCII for ' '
    [0x3A] = "",  // Caps lock
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
    [0x45] = "",  // Num lock
    [0x46] = "",  // Scroll lock
    [0x47] = "7", // k7
    [0x48] = "8", // k8
    [0x49] = "9", // k9
    [0x4A] = "-", // k-
    [0x4B] = "",  // Left Arrow
    [0x4C] = "5", // k5
    [0x4D] = "6", // k6
    [0x4E] = "+", // k+
    [0x4F] = "1", // k1
    [0x50] = "2", // k2
    [0x51] = "3", // k3
    [0x52] = "0", // k0
    [0x53] = ".", // k.
    [0x57] = "",  // F11
    [0x58] = "",  // F12
    [0x59 ... 0xFF] = {""}};

int letterWidth = 8;
int letterHeight = 16;
int letterSpacing = 0;
int ammountOfLettersOnScreen = 0;
int ammountOfLines = 0;
int letterStartX = 5;
int letterY = 5;

void erase_letter(int x, int y, int red, int green, int blue)
{
    draw_rect(x, y, x + 8, y + 16, red, green, blue, 1);
}

#define INPUT_BUFFER_SIZE 255

char currentUserInput[INPUT_BUFFER_SIZE + 1];

void initUserInputBuffer()
{
    kmemset(currentUserInput, '\0', INPUT_BUFFER_SIZE + 1);
}

void clearUserInputBuffer()
{
    for (int i = 0; i <= INPUT_BUFFER_SIZE; i++)
    {
        currentUserInput[i] = '\0';
    }
}

void appendDataToUserInput(int data)
{
    if (data >= 0 && data <= 0xFF)
    {
        if (strlen(se_layout_lower[data]) > 0)
        {
            size_t inputLength = strlen(currentUserInput);
            size_t newLength = inputLength + strlen(se_layout_lower[data]);

            if (newLength < INPUT_BUFFER_SIZE)
            {
                kmemcpy(currentUserInput + inputLength, se_layout_lower[data], strlen(se_layout_lower[data]));
                currentUserInput[newLength] = '\0'; // Null-terminate the string
            }
        }
    }
}

int outputText;

void handle_user_input_buffer()
{
    if (kmemcmp(currentUserInput, "echo", 4) == 0)
    {
        char command[5];
        char message[1000];
        int input_length = strlen(currentUserInput);

        if (input_length > 5 && currentUserInput[4] == ' ' && !(currentUserInput[5] == ' ' || currentUserInput[5] == '\t'))
        {
            int message_start = 5;
            int i = 0;
            while (message_start + i < input_length && !(currentUserInput[message_start + 1] == ' ' || currentUserInput[message_start + 1] == '\t'))
            {
                message[i] = currentUserInput[message_start + i];
                i++;
            }
            message[i] = '\0';
            println(message);
        }
    }

    else if (kmemcmp(currentUserInput, "clear", 5) == 0)
    {
        clear_screen();
    }
    else if (kmemcmp(currentUserInput, "reboot", 6) == 0)
    {
        sys_reboot();
    }
    else if (kmemcmp(currentUserInput, "shutdown", 8) == 0)
    {
        sys_shutdown_vm_only();
    }
    else if (kmemcmp(currentUserInput, "exit", 4) == 0)
    {
        outputText = 0;
        set_background_color(display_red, display_green, display_blue);
        logger_dbg("Exited shell, reboot to relaunch the shell");
    }
    else if (kmemcmp(currentUserInput, "int", 3) == 0)
    {
        const char *number_str = currentUserInput + 3;
        int number = 0;
        int i = 0;
        while (number_str[i] >= '0' && number_str[i] <= '9')
        {
            number = number * 10 + (number_str[i] - '0');
            i++;
        }


        if (number >= 0 && number <= 255)
        {
            asm volatile("int $0" ::"r"((int)number));
        }
        else
        {
            println_err("Invalid interrupt number. Must be between 0 and 255.");
        }
    }

    else if (kmemcmp(currentUserInput, "help", 4) == 0)
    {
        println("psh command list:");
        println(" - echo <message...>       : Prints the message to the screen");
        println(" - int <interupt number>   : Prints the message to the screen");
        println(" - reboot                  : Reboot the system");
        println(" - shutdown                : Shutdown the system (VM ONLY)");
        println(" - exit                    : Exits the shell");
        println(" - clear                   : Clears the screen");
        println(" - help                    : Shows this text");
        println(" ");
    }
    else
    {
        println("- psh: Command not found.");
    }
}

void removeLastNonEmptyCharacter()
{
    for (int i = INPUT_BUFFER_SIZE; i >= 0; i--)
    {
        if (currentUserInput[i] != '\0')
        {
            currentUserInput[i] = '\0';
            break; // Exit the loop after clearing the last non-empty character
        }
    }
}

__attribute__((interrupt)) void keyboard_handler(void *)
{
    uint8_t data = inb8(PS2_DATA);
    outputText = 1;

    if (data == 0)
    {
        invalid_keyboard_data();
    }

    if (se_layout_lower[data] != "" || (data == 28 || data == 14 || data == 156) && outputText)
    {

        char *letterString = se_layout_lower[data];
        int letterAscii = (int)letterString[0];

        if ((letterStartX + (letterWidth + letterSpacing) * ammountOfLettersOnScreen) + letterWidth > getWidth() || data == 0x1C)
        {
            ammountOfLines++;
            letterY += letterHeight + letterSpacing;
            ammountOfLettersOnScreen = 0;
        }
        else if (data == 28) // Enter
        {
            ammountOfLines++;
            letterY += letterHeight + letterSpacing;
            ammountOfLettersOnScreen = 0;
        }

        else if (data == 156)
        {
            handle_user_input_buffer();
            clearUserInputBuffer();
        }

        else if (data == 14) // Backspace
        {
            if (ammountOfLettersOnScreen > 0)
            {
                ammountOfLettersOnScreen -= 1;
                erase_letter(letterStartX + (letterWidth + letterSpacing) * ammountOfLettersOnScreen, letterY, display_red, display_green, display_blue);
                removeLastNonEmptyCharacter();
            }
        }
        else
        {
            appendDataToUserInput(data);
            draw_letter(letterAscii, letterStartX + (letterWidth + letterSpacing) * ammountOfLettersOnScreen, letterY, 255, 255, 255);
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
