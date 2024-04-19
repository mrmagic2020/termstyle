/**
 * col256.cpp - tests 256-color terminal color presets
*/

#include "../include/termstyle.hpp"

namespace ts = termstyle;

int main()
{
    std::vector<ts::PresetConfig> configs(256);
    for (int i = 0; i < 256; i++)
    {
        ts::PresetConfig config = {
            .prefix = {
                .text = "[Color #" + std::to_string(i) + "]",
                .prestyles = {ts::Color(ts::Col256(ts::ColorMode::FOREGROUND, i))},
                .poststyles = {ts::Color(ts::Codes::RESTORE), ts::Color(ts::Col256(ts::ColorMode::BACKGROUND, i))}
            }
        };
        ts::addPreset("color" + std::to_string(i), config);
    }

    for (int i = 0; i < 256; i++)
    {
        ts::print("color" + std::to_string(i), "                    ");
    }
    return 0;
}