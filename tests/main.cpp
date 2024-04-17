#include <iostream>

#include "../include/termstyle.hpp"

namespace ts = termstyle;

int main()
{
    ts::PresetConfig error_preset;
    error_preset.prefix.prestyles = {ts::Codes::BRIGHT, ts::Codes::FOREGROUND_RED};
    error_preset.prefix.text = "[ERROR] ";
    error_preset.prefix.poststyles = {ts::Codes::BRIGHT_RESET};

    ts::PresetConfig debug_preset = {
        .prefix = {
            .prestyles = {ts::Codes::DIM, ts::Codes::FOREGROUND_CYAN},
            .text = "[DEBUG] ",
            .poststyles = {ts::Codes::DIM_RESET}
        }
    };

    ts::PresetConfig input_preset = {
        .prefix = {
            .text = "Whatever you type will have a green background",
        },
        .suffix = {
            .prestyles = {ts::Codes::FLASH},
            .text = " >> ",
            .poststyles = {ts::Codes::FLASH_RESET, ts::Codes::BACKGROUND_GREEN}
        },
        .config = {
            .trailing_restore = false,
            .trailing_newline = false
        }
    };

    ts::addPreset("error", error_preset);
    ts::print("error", "This is an error message printed using termstyle::print.");
    ts::style("error") << "This is an error message printed using termstyle::style.";

    ts::addPreset("debug", debug_preset);
    ts::print("debug", "This is a debug message printed using termstyle::print.");
    ts::style("debug") << "This is a debug message printed using termstyle::style.";

    ts::addPreset("input", input_preset);
    char * input;
    ts::print("input");
    scanf("%s", input);
    ts::style("input");
    scanf("%s", input);
    
    return 0;
}