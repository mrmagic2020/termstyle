# termstyle

<div align="center">

<img alt="banner" src="https://github.com/mrmagic2020/termstyle/blob/main/assets/banner.png?raw=true" width="500">

</div>

An open-source C++ header file that enables developers to easily define and apply customizable style presets for terminal outputs such as warnings, infos, and errors.

## Features

- Header-only.

- Customized style presets.

- Supports ANSI Colors and Styles.

## Usage

Documentation [here](https://mrmagic2020.github.io/termstyle/index.html).

### Adding presets

1. `termstyle::PresetConfig preset_config;` Define preset.

2. `termstyle::addPreset(std::string name, PresetConfig config);` Register preset.

3. `termstyle::print(std::string preset, std::string msg);` or `termstyle::style(std::string preset) << std::string msg;` to print using presets.

For `PresetConfig`:

```hpp
struct StyleString
{
    std::string text = "";
    std::vector<Codes> prestyles = {};
    std::vector<Codes> poststyles = {};
};
struct PresetConfig
{
    StyleString prefix;
    StyleString suffix;
};
```

A list of `Codes` can be found [here](https://mrmagic2020.github.io/termstyle/namespacetermstyle.html#a452219ccabb47a06a4046ae22252ed6c).

### Examples

#### Basic usage

```cpp
#include <iostream>
#include "../include/termstyle.hpp"
namespace ts = termstyle;

int main()
{
    ts::PresetConfig debug_preset = {
        .prefix = {
            .prestyles = {ts::Codes::DIM, ts::Codes::FOREGROUND_CYAN},
            .text = "[DEBUG] ",
            .poststyles = {ts::Codes::DIM_RESET}
        },
        .suffix = {
            .text = "\n"
        }
    }

    ts::addPreset("debug", debug_preset);
    ts::print("debug", "This is a debug message printed using termstyle::print.");
    ts::style("debug") << "This is a debug message printed using termstyle::style.";
    
    return 0;
}
```

Output

<img alt="basic-usage-output" src="https://github.com/mrmagic2020/termstyle/blob/dev-1.0.0/assets/basic-usage-output.png?raw=true" width="500">

