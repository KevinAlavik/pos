#include "libs/stdlib.h"

#define INPUT_BUFFER_SIZE 255

char currentUserInput[INPUT_BUFFER_SIZE + 1];


void updatePrompt() {
    if(drawShellIcon) {
        if(letterStartX != letterWidth + printfLetterSpacing) {
            letterStartX = letterWidth + printfLetterSpacing;
        }

        print("$");
        letterStartX += (letterWidth + letterSpacing) * 2;
    }
}

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

int echoText;
int textBuffer;

void handle_user_input_buffer()
{
    drawShellIcon = 0;
    letterStartX = (letterWidth + letterSpacing);

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

            drawShellIcon = 1;
            updatePrompt();
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

        drawShellIcon = 1;
        updatePrompt();
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

            drawShellIcon = 1;
            updatePrompt();
        }
        else
        {
            println_err("Invalid interrupt number. Must be between 0 and 255.");

            drawShellIcon = 1;
            updatePrompt();
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

        drawShellIcon = 1;
        updatePrompt();
    }
    else
    {

        println("- psh: Command not found.");

        drawShellIcon = 1;
        updatePrompt();
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