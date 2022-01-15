#ifndef DISPLAY_H
#define DISPLAY_H
#include <string>
#include <Windows.h>

enum class DisplayRotation : std::uint8_t { cw_0, cw_90, cw_180, cw_270 };

const int displayCount();

class Display {
private:
    DEVMODE devmode;
    DISPLAY_DEVICE display{};

public:
    Display(const int device);

    const std::string getName() const;

    const long getPosX() const;

    const long getPosY() const;

    const DisplayRotation getRotation() const;
    
    bool rotate(const DisplayRotation rotation);

    bool resetRotation();

    bool setPos(const long x, const long y);
};
#endif