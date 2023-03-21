#include "graphics.h"

int start() {
    ClearScreen(125, 0, 125);
    DrawCharacter(getArialCharacter, font_arial_width, font_arial_height, 'P', 10, 10, 0, 255, 0);
    DrawCharacter(getArialCharacter, font_arial_width, font_arial_height, '#', 15, 10, 0, 255, 0);

    while(1);
}