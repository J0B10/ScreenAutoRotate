#ifndef DISPLAY_H
#define DISPLAY_H
#include <string>
#include <Windows.h>

enum DisplayRotation { cw_0, cw_90, cw_180, cw_270 };

const int displayCount();

class Display {
private:
    DEVMODE devmode;
    DISPLAY_DEVICE d;

public:
    Display(const int device);

    const std::string getName();

    const long getPosX();

    const long getPosY();

    const DisplayRotation getRotation();
    
    bool rotate(const DisplayRotation rotation);

    bool resetRotation();

    bool setPos(const long x, const long y);
};
#endif