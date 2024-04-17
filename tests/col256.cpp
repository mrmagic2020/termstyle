#include "../include/termstyle.hpp"

namespace ts = termstyle;

int main()
{
    std::vector<ts::PresetConfig> configs(256);
    for (int i = 0; i < 256; i++)
    {
        ts::PresetConfig config = {
            .prefix = {
                .prestlye256 = {ts::Col256(ts::ColorMode::FOREGROUND, i)},
                .text = "[Color #" + std::to_string(i) + "]",
                .poststyles = {ts::Codes::RESTORE},
                .poststyle256 = {ts::Col256(ts::ColorMode::BACKGROUND, i)}
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