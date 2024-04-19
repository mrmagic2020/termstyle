# termstyle

<div align="center">

<img alt="banner" src="https://github.com/mrmagic2020/termstyle/blob/main/assets/banner.png?raw=true" width="500">

</br>

![GitHub License](https://img.shields.io/github/license/mrmagic2020/termstyle)
![GitHub Release Date](https://img.shields.io/github/release-date-pre/mrmagic2020/termstyle)
![GitHub Tag](https://img.shields.io/github/v/tag/mrmagic2020/termstyle)

</div>

An open-source C++ library that enables developers to easily define and apply customizable style presets for terminal outputs such as warnings, infos, and errors.

## Features

- Cross-platform. (Tested on macOS, Windows, and Ubuntu)

- Header-only.

- Customizable style presets.

- Supports ANSI Colors and Styles.

- Supports 256 Color Codes.

- Supports RGB Colors.

## Usage

Full documentation [here](https://mrmagic2020.github.io/termstyle/index.html).

### Creating a preset

Preset configurations are constructed with `termstyle::PresetConfig`.

```hpp
struct PresetConfig
{
    StyleString prefix;
    StyleString suffix;
    Config config;
};
```

It contains two `termstyle::StyleString` structs - `prefix` and `suffix`.

- A `prefix` is inserted before the text.

- A `suffix` is inserted after the text.

And a `termstyle::config` struct.

A `StyleString` struct looks like

```hpp
struct StyleString
{
    /**
    * The text to be styled.
    */
    std::string text = "";

    /**
    * A vector container that stores objects of type Color, applied before `text`.
    * 
    * @see Color
    */
    std::vector<Color> prestyles = {};

    /**
    * A vector container that stores objects of type Color, applied after `text`.
    * 
    * @see Color
    */
    std::vector<Color> poststyles = {};
};
```

> Do NOT include a new line character `\n` in your suffix. Instead, adjust the `config` to your needs.

A `Config` struct looks like

```hpp
struct Config
{
    /**
    * Indicates whether leading restore is enabled or not.
    * If set to true, it means that leading restore is enabled.
    * If set to false, leading restore is disabled.
    */
    bool leading_restore = true;

    /**
    * Indicates whether trailing restore is enabled or not.
    * If set to true, trailing restore will be performed.
    * If set to false, trailing restore will be skipped.
    */
    bool trailing_restore = true;

    /**
    * Specifies whether a trailing newline character should be added when printing.
    * If set to true, a newline character will be added at the end of the printed output.
    * If set to false, no newline character will be added.
    */
    bool trailing_newline = true;
};
```

#### Example

```cpp
#include <iostream>
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
    return 0;
}
```

### Registering a preset

After [creating a preset](#creating-a-preset), you nned to register it for use. Simply call

```cpp
termstyle::addPreset(std::string name, termstyle::PresetConfig config)
```

- `name` is what you want to name the preset. Must be unique, or an error will be thrown.

- `config` is the preset you just created.

#### Example

```cpp
#include "./include/termstyle.hpp"
namespace ts = termstyle;

int main()
{
    ts::PresetConfig debug_preset = { ... };
    ts::addPreset("debug", debug_preset);
    return 0;
}
```

### Using a preset

Finally, you can use the preset you created to print output to the terminal. There are two ways do to it:

1. Call `termstyle::print(std::string name, std::string msg);`

   - `name` is the name of the preset you registered before.

   - `msg` is the content you want to print.

2. Perform `termstyle::style(std::string name) << std::string msg << ...;`

   - `name` is the name of the preset you registered before.

   - Use it as an `ostream`.

   > Note that `suffix` in this case will be applied after the last `msg` of the line.

#### Example

```cpp
#include "./include/termstyle.hpp"
namespace ts = termstyle;

int main()
{
    ts::PresetConfig debug_preset = { ... };
    ts::addPreset("debug", debug_preset);

    // Method 1
    ts::print("debug", "This is a debug message.");
    // Method 2
    ts::style("debug") << "This is another debug message. " << "Suffix is applied after me.";

    return 0;
}
```

## Examples

### Basic usage

```cpp
#include "./include/termstyle.hpp"
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
```

Output

<img alt="basic-usage-output" src="https://github.com/mrmagic2020/termstyle/blob/main/assets/basic-usage-output.png?raw=true" width="750">

### Color256

```cpp
#include "../include/termstyle.hpp"
namespace ts = termstyle;

int main()
{
    std::vector<ts::PresetConfig> configs(256);
    for (int i = 0; i < 256; i++)
    {
        ts::PresetConfig config = {
            .prefix = {
                .prestyles = {ts::Color(ts::Col256(ts::ColorMode::FOREGROUND, i))},
                .text = "[Color #" + std::to_string(i) + "]",
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
```

#### Output

<img alt="color256-output" src="https://github.com/mrmagic2020/termstyle/blob/main/assets/col256-output.png?raw=true" width="300">

### RGB Colors

```cpp
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
```

#### Output

<img alt="colrgb-output" src="https://github.com/mrmagic2020/termstyle/blob/main/assets/colrgb-output.png?raw=true" width="750">

### Fancy Input

```cpp
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
            .prestyles = {ts::Codes::FLASH},
            .poststyles = {ts::Codes::FLASH_RESET, ts::Codes::BACKGROUND_GREEN}
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
```

#### Output

<img alt="fancy-input-output" src="https://github.com/mrmagic2020/termstyle/blob/main/assets/fancy-input-output.png?raw=true" width="750">
