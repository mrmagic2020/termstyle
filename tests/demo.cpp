#include <iostream>
#include "../include/termstyle.hpp"

namespace ts = termstyle;

void createAndAddPresets()
{
    // Preset for errors
    ts::PresetConfig error_preset;
    error_preset.prefix.prestyles = {ts::Color(ts::Codes::BRIGHT), ts::Color(ts::Codes::FOREGROUND_RED)};
    error_preset.prefix.text = "[ERROR] ";
    ts::addPreset("error", error_preset);

    // Preset for warnings
    ts::PresetConfig warning_preset;
    warning_preset.prefix.prestyles = {ts::Color(ts::Codes::BRIGHT), ts::Color(ts::Codes::FOREGROUND_YELLOW)};
    warning_preset.prefix.text = "[WARNING] ";
    ts::addPreset("warning", warning_preset);

    // Preset for info messages
    ts::PresetConfig info_preset;
    info_preset.prefix.prestyles = {ts::Color(ts::Codes::BRIGHT), ts::Color(ts::Codes::FOREGROUND_CYAN)};
    info_preset.prefix.text = "[INFO] ";
    ts::addPreset("info", info_preset);

    // Preset for success messages
    ts::PresetConfig success_preset;
    success_preset.prefix.prestyles = {ts::Color(ts::Codes::BRIGHT), ts::Color(ts::Codes::FOREGROUND_GREEN)};
    success_preset.prefix.text = "[SUCCESS] ";
    ts::addPreset("success", success_preset);
}

int main()
{
    createAndAddPresets();

    // Print styled messages
    ts::print("error", "An error has occurred while processing your request.");
    ts::print("warning", "This action will overwrite the existing file.");
    ts::print("info", "Updates are available for your system.");
    ts::print("success", "The operation completed successfully.");

    // Styled output using style() and stream operator
    ts::style("error") << "Styled stream: This action is not allowed.";
    ts::style("warning") << "Styled stream: Proceed with caution.";
    ts::style("info") << "Styled stream: Press 'Y' to continue.";
    ts::style("success") << "Styled stream: File saved successfully.";

    return 0;
}
