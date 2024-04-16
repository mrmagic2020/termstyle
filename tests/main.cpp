#include <iostream>

#include "../include/termstyle.hpp"

termstyle::PresetConfig preset_config = {
    {"[ERROR] ", {termstyle::codes::BRIGHT, termstyle::codes::FOREGROUND_RED}, {termstyle::codes::RESTORE}},
    {"\n", {}, {termstyle::codes::RESTORE}}};

int main()
{
    termstyle::addPreset("error", preset_config);
    // termstyle::addPreset("error", preset_config); // throws PresetNameUsed
    termstyle::print("error", "This is an error message printed using termstyle::print.");
    termstyle::style("error") << "This is an error message printed using termstyle::style.";
    return 0;
}