#include <libkrnl/arch/x86/io/portio.h>
#include "ktools/pic-controller.h"

#include "layouts.h"    
#include "shell.h"

#define PS2_COMMAND 0x64
#define PS2_DATA 0x60

void disableTextOutput() {
    outputText = 0;
}

void enableTextOutput() {
    outputText = 1;
}

__attribute__((interrupt)) void keyboard_handler(void *)
{
    uint8_t data = inb8(PS2_DATA);

    if (data == 0)
    {
        invalid_keyboard_data();
    }

    if (se_layout_lower[data] != "" || (data == 28 || data == 14 || data == 156))
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

        else if (data == 156) // Enter - released
        {
            //handle_user_input_buffer();

            logger_ok(currentUserInput);

            clearUserInputBuffer();        
        }

        else if (data == 14) // Backspace
        {
            if (ammountOfLettersOnScreen > 0 && outputText)
            {
                ammountOfLettersOnScreen -= 1;
                erase_letter(letterStartX + (letterWidth + letterSpacing) * ammountOfLettersOnScreen, letterY, display_red, display_green, display_blue);
                removeLastNonEmptyCharacter();
            }
        }
        else
        {   
            if(outputText) {
                appendDataToUserInput(data);
                draw_letter(letterAscii, letterStartX + (letterWidth + letterSpacing) * ammountOfLettersOnScreen, letterY, 255, 255, 255);
                ammountOfLettersOnScreen += 1;
            }   
        }
    }


    i8259_SendEndOfInterrupt(1);
}

void keyboard_init()
{
    letterSpacing = printfLetterSpacing;

    set_idt_gate(0x21, (uint64_t)&keyboard_handler, 0x28, 0x8E);

    if (inb8(PS2_COMMAND) & 0x1)
        inb8(PS2_DATA);
    outb8(PS2_COMMAND, 0x60);
    outb8(PS2_DATA, 0b01100111);

    outb8(PS2_COMMAND, 0xAE);
    outb8(PS2_DATA, 0xf4);
    i8259_Unmask(1);
}
