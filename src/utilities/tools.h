int printfLetterWidth = 8;
int printfLetterSpacing = 5;
int printfStartFromMiddle = 0;

void printf(const int letters[], size_t length, int x, int y, int r, int g, int b)
{
    int wordWidth = (length - 1) * (printfLetterWidth + printfLetterSpacing);
    
    int startX = (printfStartFromMiddle) ? (x - (wordWidth / 2)) : x;

    int currentX = startX;
    for (size_t i = 0; i < length; i++)
    {
        int letterIndex = letters[i];
        if (letterIndex >= 0 && letterIndex <= 255)
        {
            draw_letter(letterIndex, currentX, y, r, g, b);
            currentX += printfLetterWidth + printfLetterSpacing;
        }
    }
}

