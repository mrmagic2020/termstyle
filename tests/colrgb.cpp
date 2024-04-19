/**
 * colrgb.cpp -- test for RGB colors
*/

#include "../include/termstyle.hpp"

namespace ts = termstyle;

int main()
{
    int r, g, b;
    std::cout << "Enter RGB values (0-255): ";
    std::cin >> r >> g >> b;

    ts::PresetConfig rgb_preset = {
        .prefix = {
            .text = "R=" + std::to_string(r) + " G=" + std::to_string(g) + " B=" + std::to_string(b) + ": ",
            .prestyles = {ts::Color(ts::ColRGB(ts::ColorMode::FOREGROUND, r, g, b))}
        },
        .suffix = {
            .text = "                              ",
            .prestyles = {ts::Color(ts::ColRGB(ts::ColorMode::BACKGROUND, r, g, b))}
        }
    };

    ts::addPreset("rgb", rgb_preset);
    ts::print("rgb");
    
    return 0;
}