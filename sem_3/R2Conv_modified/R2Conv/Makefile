e <X11/Xos.h>

#include <stdio.h>
#include <stdlib.h>

#include "XLib.h"

int DMVN_XLib_RGBMode = RGB_565;

DWORD RGB(int R, int G, int B)
{
    switch (DMVN_XLib_RGBMode)
    {
	case RGB_555: return ((R >> 3) << 10) + ((G >> 3) << 5) + (B >> 3);
	case RGB_565: return ((R >> 3) << 11) + ((G >> 2) << 5) + (B >> 3);
	case RGB_888: return (R << 16) + (G << 8) + B;
	default: return 0;
    }
}

TXWin::TXWin(char * Title, int X, int Y, int Width, int Height, int RGBMode)
{
    Disp = XOpenDisplay(NULL);
    if (!Disp) throw Ex("Cannot open display");
    Scr = DefaultScreen(Disp);
    Win = XCreateSimpleWindow(Disp, DefaultRootWindow(Disp), X, Y, Width, Height, 0, BlackPixel(Disp, Scr), WhitePixel(Disp, 