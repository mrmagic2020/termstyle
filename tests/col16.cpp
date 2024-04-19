/**
 * col16.cpp -- tests 16-color terminal color presets
*/

#include "../include/termstyle.hpp"

namespace ts = termstyle;

int main()
{
    ts::PresetConfig debug_preset = {
        .prefix = {
            .text = "[DEBUG] ",
            .prestyles = {ts::Color(ts::Codes::DIM), ts::Color(ts::Codes::FOREGROUND_CYAN)},
            .poststyles = {ts::Color(ts::Codes::DIM_RESET)}
        }
    };

    ts::addPreset("debug", debug_preset);
    ts::print("debug", "This is a debug message printed using termstyle::print.");
    ts::style("debug") << "This is a debug message printed using termstyle::style.";
    
    return 0;
}