#include <iostream>

#include "../include/termstyle.hpp"

namespace ts = termstyle;

int main()
{
    ts::PresetConfig error_preset;
    error_preset.prefix.prestyles = {ts::Codes::BRIGHT, ts::Codes::FOREGROUND_RED};
    error_preset.prefix.text = "[ERROR] ";
    error_preset.prefix.poststyles = {ts::Codes::BRIGHT_RESET};
    error_preset.suffix.text = "\n";

    ts::PresetConfig debug_preset = {
        .prefix = {
            .text = "[DEBUG] ",
            .prestyles = {ts::Codes::DIM, ts::Codes::FOREGROUND_CYAN},
            .poststyles = {ts::Codes::DIM_RESET}
        },
        .suffix = {
            .text = "\n"
        }
    };

    ts::PresetConfig input_preset = {
        .prefix = {
            .text = "Whatever you type will have a green background",
        },
        .suffix = {
            .text = " >> ",
            .prestyles = {ts::Codes::FLASH},
            .poststyles = {ts::Codes::FLASH_RESET, ts::Codes::BACKGROUND_GREEN}
        }
    };

    ts::addPreset("error", error_preset);
    ts::print("error", "This is an error message printed using termstyle::print.");
    ts::style("error") << "This is an error message printed using termstyle::style.";

    ts::addPreset("debug", debug_preset);
    ts::print("debug", "This is a debug message printed using termstyle::print.");
    ts::style("debug") << "This is a debug message printed using termstyle::style.";

    ts::config::trailing_restore = false;
    ts::addPreset("input", input_preset);
    char * input;
    ts::print("input");
    scanf("%s", input);
    ts::style("input");
    scanf("%s", input);
    
    return 0;
}