#include <exception>
#include <stdexcept>
#include <string>
#include "display.h"

const int displayCount() {
    int i{0};
    while (true) {
        DEVMODE dm{};
        DISPLAY_DEVICE dd;
        dd.cb = { sizeof(DISPLAY_DEVICE) };
        if (!EnumDisplayDevices(NULL, i++, &dd, 0) 
                || !EnumDisplaySettings(dd.DeviceName, ENUM_CURRENT_SETTINGS, &dm)) {
            return i - 1;
        }
    }
}

Display::Display(int device) {
    d.cb = { sizeof(DISPLAY_DEVICE) };
    if (!EnumDisplayDevices(NULL, device, &d, 0)) {
        throw std::runtime_error("could not enum display device " + std::to_string(device));
    }
    if (!EnumDisplaySettings(d.DeviceName, ENUM_CURRENT_SETTINGS, &devmode)) {
        throw std::runtime_error("could not enum display settings for " + std::string{d.DeviceName});
    }
}

const std::string Display::getName()
{
    return std::string{d.DeviceName};
}

const long Display::getPosX() {
    return devmode.dmPosition.x;
}

const long Display::getPosY() {
    return devmode.dmPosition.y;
}

const DisplayRotation Display::getRotation() {
    return static_cast<DisplayRotation>(devmode.dmDisplayOrientation);
}

bool Display::rotate(const DisplayRotation rotation) {
    int oldRot{devmode.dmDisplayOrientation};
    int newRot{ (oldRot + rotation) % 4 };
    if ((newRot % 2) != (oldRot % 2)) {
        int temp = devmode.dmPelsHeight;
        devmode.dmPelsHeight = devmode.dmPelsWidth;
        devmode.dmPelsWidth = temp;
    }
    devmode.dmDisplayOrientation = newRot;
    return ChangeDisplaySettingsEx(d.DeviceName, &devmode, NULL, CDS_UPDATEREGISTRY, NULL) != 0;
}

bool Display::resetRotation() {
    if ((devmode.dmDisplayOrientation % 2) != 0) {
        int temp{devmode.dmPelsHeight};
        devmode.dmPelsHeight = devmode.dmPelsWidth;
        devmode.dmPelsWidth = temp;
    }
    devmode.dmDisplayOrientation = { 0 };
    return ChangeDisplaySettingsEx(d.DeviceName, &devmode, NULL, CDS_UPDATEREGISTRY, NULL) != 0;
}

bool Display::setPos(const long x, const long y) {
    devmode.dmPosition.x = { x };
    devmode.dmPosition.y = { y };
    return ChangeDisplaySettingsEx(d.DeviceName, &devmode, NULL, CDS_UPDATEREGISTRY, NULL) != 0;
}
