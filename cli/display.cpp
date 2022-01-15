#include <exception>
#include <stdexcept>
#include <string>
#include "display.h"

int displayCount() {
    int i{0};
    while (true) {
        DEVMODE dm{};
        DISPLAY_DEVICE dd{};
        dd.cb = { sizeof(DISPLAY_DEVICE) };
        if (!EnumDisplayDevices(nullptr, i++, &dd, 0) 
                || !EnumDisplaySettings(dd.DeviceName, ENUM_CURRENT_SETTINGS, &dm)) {
            return i - 1;
        }
    }
}

Display::Display(int device) {
    display.cb = { sizeof(DISPLAY_DEVICE) };
    if (!EnumDisplayDevices(nullptr, device, &display, 0)) {
        throw std::runtime_error("could not enum display device " + std::to_string(device));
    }
    if (!EnumDisplaySettings(display.DeviceName, ENUM_CURRENT_SETTINGS, &devmode)) {
        throw std::runtime_error("could not enum display settings for " + std::string{ display.DeviceName });
    }
}

const std::string Display::getName() const {
    return std::string{ display.DeviceName };
}

long Display::getPosX() const {
    return devmode.dmPosition.x;
}

long Display::getPosY() const {
    return devmode.dmPosition.y;
}

const DisplayRotation Display::getRotation() const {
    return static_cast<DisplayRotation>(devmode.dmDisplayOrientation);
}

bool Display::rotate(const DisplayRotation rotation) {
    int oldRot{ static_cast<int>(devmode.dmDisplayOrientation) };
    int newRot{ (oldRot + static_cast<int>(rotation)) % 4 };
    if ((newRot % 2) != (oldRot % 2)) {
        int temp = devmode.dmPelsHeight;
        devmode.dmPelsHeight = devmode.dmPelsWidth;
        devmode.dmPelsWidth = temp;
    }
    devmode.dmDisplayOrientation = newRot;
    return ChangeDisplaySettingsEx(display.DeviceName, &devmode, nullptr, CDS_UPDATEREGISTRY, nullptr) != 0;
}

bool Display::resetRotation() {
    if ((devmode.dmDisplayOrientation % 2) != 0) {
        int temp{ static_cast<int>(devmode.dmPelsHeight) };
        devmode.dmPelsHeight = devmode.dmPelsWidth;
        devmode.dmPelsWidth = temp;
    }
    devmode.dmDisplayOrientation = { 0 };
    return ChangeDisplaySettingsEx(display.DeviceName, &devmode, nullptr, CDS_UPDATEREGISTRY, nullptr) != 0;
}

bool Display::setPos(const long x, const long y) {
    devmode.dmPosition.x = { x };
    devmode.dmPosition.y = { y };
    return ChangeDisplaySettingsEx(display.DeviceName, &devmode, nullptr, CDS_UPDATEREGISTRY, nullptr) != 0;
}
