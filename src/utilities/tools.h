int printfLetterSpacing = 0;
int printfLetterWidth = 8;
int printfLetterHeight = 16;
int printfStartFromMiddle = 0;

size_t kstrlen(const char* str) {
    size_t length = 0;
    while (str[length] != '\0') {
        length++;
    }
    return length;
}

void println(char* string) {
    int length = kstrlen(string);
    int wordWidth = (length - 1) * (printfLetterWidth + printfLetterSpacing);
    
    int startX = (printfStartFromMiddle) ? (letterStartX - (wordWidth / 2)) : letterStartX;

    int currentX = startX;
    for (int i = 0; i < length; i++)
    {
        int letterIndex = (int)string[i];
        if (letterIndex >= 0 && letterIndex <= 255)
        {
            draw_letter(letterIndex, currentX, letterY, 255, 255, 255);
            currentX += printfLetterWidth + printfLetterSpacing;
        }
    }

    letterY += printfLetterHeight; // Move to the next line
}

void println_ok(char* string) {
    char* okString = "[ OK ]";
    int okLength = kstrlen(okString);
    int wordWidth = (okLength - 1) * (printfLetterWidth + printfLetterSpacing);
    
    int startX = (printfStartFromMiddle) ? (letterStartX - (wordWidth / 2)) : letterStartX;

    int currentX = startX;
    for (int i = 0; i < okLength; i++)
    {
        int letterIndex = (int)okString[i];
        if (letterIndex >= 0 && letterIndex <= 255)
        {
            if (i > 1 && i < 4) {
                draw_letter(letterIndex, currentX, letterY, 0, 255, 0);
            } else {
                draw_letter(letterIndex, currentX, letterY, 255, 255, 255);
            }
            currentX += printfLetterWidth + printfLetterSpacing;
        }
    }

    currentX += printfLetterWidth + printfLetterSpacing;

    int providedLength = kstrlen(string);
    for (int i = 0; i < providedLength; i++)
    {
        int letterIndex = (int)string[i];
        if (letterIndex >= 0 && letterIndex <= 255)
        {
            draw_letter(letterIndex, currentX, letterY, 255, 255, 255);
            currentX += printfLetterWidth + printfLetterSpacing;
        }
    }

    letterY += printfLetterHeight;
}

void println_err(char* msg) {
    char* errString = "[ ERR ]";
    int errLength = kstrlen(errString);

    int startX = letterStartX;

    int currentX = startX;
    for (int i = 0; i < errLength; i++)
    {
        int letterIndex = (int)errString[i];
        if (letterIndex >= 0 && letterIndex <= 255)
        {
            if (i > 1 && i < 5) {
                draw_letter(letterIndex, currentX, letterY, 255, 0, 0); // Set color for "ERR" to red
            } else {
                draw_letter(letterIndex, currentX, letterY, 255, 255, 255);
            }
            currentX += printfLetterWidth + printfLetterSpacing;
        }
    }

    currentX += printfLetterWidth + printfLetterSpacing;

    int msgLength = kstrlen(msg);
    for (int i = 0; i < msgLength; i++)
    {
        int letterIndex = (int)msg[i];
        if (letterIndex >= 0 && letterIndex <= 255)
        {
            draw_letter(letterIndex, currentX, letterY, 255, 255, 255);
            currentX += printfLetterWidth + printfLetterSpacing;
        }
    }

    letterY += printfLetterHeight;
}

void println_warn(char* msg) {
    char* warnString = "[ WARN ]";
    int warnLength = kstrlen(warnString);

    int startX = letterStartX;

    int currentX = startX;
    for (int i = 0; i < warnLength; i++)
    {
        int letterIndex = (int)warnString[i];
        if (letterIndex >= 0 && letterIndex <= 255)
        {
            if (i > 1 && i < 6) {
                draw_letter(letterIndex, currentX, letterY, 255, 165, 0); // Set color for "WARN" to orange
            } else {
                draw_letter(letterIndex, currentX, letterY, 255, 255, 255);
            }
            currentX += printfLetterWidth + printfLetterSpacing;
        }
    }

    currentX += printfLetterWidth + printfLetterSpacing;

    int msgLength = kstrlen(msg);
    for (int i = 0; i < msgLength; i++)
    {
        int letterIndex = (int)msg[i];
        if (letterIndex >= 0 && letterIndex <= 255)
        {
            draw_letter(letterIndex, currentX, letterY, 255, 255, 255);
            currentX += printfLetterWidth + printfLetterSpacing;
        }
    }

    letterY += printfLetterHeight;
}

void println_dbg(char* msg) {
    char* dbgString = "[ DBG ]";
    int dbgLength = kstrlen(dbgString);

    int startX = letterStartX;

    int currentX = startX;
    for (int i = 0; i < dbgLength; i++)
    {
        int letterIndex = (int)dbgString[i];
        if (letterIndex >= 0 && letterIndex <= 255)
        {
            if (i > 1 && i < 5) {
                draw_letter(letterIndex, currentX, letterY, 192, 192, 192); // Set color for "DBG" to gray
            } else {
                draw_letter(letterIndex, currentX, letterY, 255, 255, 255);
            }
            currentX += printfLetterWidth + printfLetterSpacing;
        }
    }

    currentX += printfLetterWidth + printfLetterSpacing;

    int msgLength = kstrlen(msg);
    for (int i = 0; i < msgLength; i++)
    {
        int letterIndex = (int)msg[i];
        if (letterIndex >= 0 && letterIndex <= 255)
        {
            draw_letter(letterIndex, currentX, letterY, 255, 255, 255);
            currentX += printfLetterWidth + printfLetterSpacing;
        }
    }

    letterY += printfLetterHeight;
}

void print(char* string) {
    int length = kstrlen(string);
    int wordWidth = (length - 1) * (printfLetterWidth + printfLetterSpacing);
    
    int startX = (printfStartFromMiddle) ? (letterStartX - (wordWidth / 2)) : letterStartX;

    int currentX = startX;
    for (int i = 0; i < length; i++)
    {
        int letterIndex = (int)string[i];
        if (letterIndex >= 0 && letterIndex <= 255)
        {
            draw_letter(letterIndex, currentX, letterY, 255, 255, 255);
            currentX += printfLetterWidth + printfLetterSpacing;
        }
    }
}


void clear_screen() {
    letterY = 5;
    letterStartX = 5;

    set_background_color(display_red, display_green, display_blue);
}
