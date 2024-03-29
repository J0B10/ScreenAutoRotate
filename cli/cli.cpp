#include <iostream>
#include <string>
#include <exception>
#include <stdexcept>
#include "display.h"

void help() {
    std::cout << "Rotate and rearrange displays\n";
    std::cout << "\n";
    std::cout << "Arguments:\n";
    std::cout << "\n";
    std::cout << "  No args     Show this help, same as /?.\n";
    std::cout << "  /?          Show this help page.\n";
    std::cout << "  [rotate | /r] [angle] [display]\n";
    std::cout << "              Rotate the display with the given number clockwise by the given angle.\n";
    std::cout << "              The angle a must be a multiple of 90 (or 0 to reset any rotation).\n";
    std::cout << "  [position | /p] [x] [y] [display]\n";
    std::cout << "              Rearrange the display with the given number to the given coordinates.\n";
    std::cout << "  [info | /i] [display]\n";
    std::cout << "              Display information such as name, orientation and position of displays.\n";
    std::cout << "              If display argument is not speicfied it shows the info for all displays.\n";
    std::cout << "\n";
    std::cout << "Display numbers for all arguments start with 1. \n";
    std::cout << "Unless otherwise specified display argument may be left out with default value of 1.\n";
    std::cout << "\n";
}

int rotate(int argc, char** argv) {
    if (argc < 3) {
        std::cerr << "missing angle, use argument /? for further help\n";
        return 1;
    }

    int disp{0};
    if (argc >= 4) {
        try {
            disp = { std::stoi(argv[3]) - 1 };
        } catch (const std::invalid_argument &ex) {
            std::cerr << "invalid display number, use argument /? for further help\n";
            return 1;
        }
    }
    
    try {
        Display d{ disp };
        int rotDeg{ std::stoi(argv[2]) };
        if ((rotDeg % 90 != 0) || rotDeg < 0) {
            std::cerr << "invalid rotation, use argument /? for further help\n";
            return 1;
        }
        DisplayRotation rot = static_cast<DisplayRotation>( (rotDeg / 90) % 4);
        if (rot == DisplayRotation::cw_0) {
            return !d.resetRotation();
        } else {
            return !d.rotate(rot);
        }
    } catch (const std::invalid_argument & ex) {
        std::cerr << "invalid rotation, use argument /? for further help\n";
        return 1;
    } catch (const std::runtime_error & ex) {
        std::cerr << ex.what() << ", use argument /? for further help and /i to see current displays\n";
        return 1;
    }
}

int setPosition(int argc, char** argv) {
    if (argc < 4) {
        std::cerr << "missing coords, use argument /? for further help\n";
        return 1;
    }

    int disp{0};
    if (argc >= 5) {
        try {
            disp = { std::stoi(argv[4]) - 1 };
        }
        catch (const std::invalid_argument &ex) {
            std::cerr << "invalid display number, use argument /? for further help\n";
            return 1;
        }
    }

    try {
        Display d{ disp };
        int x{ std::stoi(argv[2]) };
        int y{ std::stoi(argv[3]) };
        return !d.setPos(x, y);
    }
    catch (const std::invalid_argument &ex) {
        std::cerr << "invalid coordinates, use argument /? for further help\n";
        return 1;
    }
    catch (const std::runtime_error &ex) {
        std::cerr << ex.what() << ", use argument /? for further help and /i to see current displays\n";
        return 1;
    }
}

void printInfo(int disp) {
    Display d{ disp };
    std::cout << "[" << (disp + 1) << "] " << d.getName() << ":\n";
    int rot{ static_cast<int>(d.getRotation()) * 90 };
    std::cout << "  Rotation: " << rot << " deg (clockwise)\n";
    std::cout << "  Position: (" << d.getPosX() << ", " << d.getPosY() <<")\n";
}

int info(int argc, char** argv) {
    if (argc < 3) {
        int count{ displayCount() };
        std::cout << count << " displays found:\n";
        for (int i{ 0 }; i < count; i++) {
            std::cout << "\n";
            printInfo(i);
        }
        return 0;
    } else {
        try {
            int disp { std::stoi(argv[2]) - 1 };
            printInfo(disp);
            return 0;
        }
        catch (const std::invalid_argument & ex) {
            std::cerr << "invalid display number, use argument /? for further help\n";
            return 1;
        }
    }
}

int main(int argc, char** argv) {
    if (argc < 2) {
        help();
        return 0;
    }
    std::string mode{argv[1]};
    if (mode.compare("/?") == 0) {
        help();
        return 0;
    } else if (mode.compare("rotate") == 0 || mode.compare("/r") == 0) {
        return rotate(argc, argv);
    } else if (mode.compare("position") == 0 || mode.compare("/p") == 0) {
        return setPosition(argc, argv);
    } else if (mode.compare("info") == 0 || mode.compare("/i") == 0) {
        return info(argc, argv);
    } else {
        std::cerr << "unknown argument, use /? for further help\n";
        return 1;
    }
}
