#include <iostream>

#include "../include/termstyle.hpp"

namespace ts = termstyle;

int main()
{
    ts::PresetConfig input_preset = {
        .prefix = {
            .text = "Whatever you type will have a green background",
        },
        .suffix = {
            .text = " >> ",
            .prestyles = {ts::Color(ts::Codes::FLASH)},
            .poststyles = {ts::Color(ts::Codes::FLASH_RESET), ts::Color(ts::Col256(ts::ColorMode::BACKGROUND, 28))}
        },
        .config = {
            .trailing_restore = false,
            .trailing_newline = false
        }
    };

    ts::addPreset("input", input_preset);
    std::string input;
    ts::print("input");
    std::getline(std::cin, input);
    ts::style("input");
    std::getline(std::cin, input);
    
    return 0;
}