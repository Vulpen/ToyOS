#ifndef GRAPHICS_ELEMENTS_H
#define GRAPHICS_ELEMENTS_H

int DrawCircleButton(int x, int y, int radius, int r, int g, int b);
int DrawButton(int x, int y, int width, int height, int r, int g, int b, char* text, int r1, int g1, int b1);
int DrawWindow(int* x, int* y, int* width, int* height, int r, int g, int b, int* mouse_held);

#endif