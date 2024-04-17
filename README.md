# termstyle

<div align="center">

<img alt="banner" src="https://github.com/mrmagic2020/termstyle/blob/main/assets/banner.png?raw=true" width="500">

</br>

![GitHub License](https://img.shields.io/github/license/mrmagic2020/termstyle)
![GitHub Release Date](https://img.shields.io/github/release-date-pre/mrmagic2020/termstyle)
![GitHub Tag](https://img.shields.io/github/v/tag/mrmagic2020/termstyle)

</div>

An open-source C++ header file that enables developers to easily define and apply customizable style presets for terminal outputs such as warnings, infos, and errors.

## Features

- Header-only.

- Customized style presets.

- Supports ANSI Colors and Styles.

## Usage

Documentation [here](https://mrmagic2020.github.io/termstyle/index.html).

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
         * A vector container that stores objects of type Codes, applied before `text`.
         * 
         * @see Codes
         */
        std::vector<Codes> prestyles = {};
        /**
         * A vector container that stores objects of type Codes, applied after `text`.
         * 
         * @see Codes
        */
        std::vector<Codes> poststyles = {};
        /**
         * A vector container that stores objects of type Col256, applied before `text`.
         * 
         * @see Col256
        */
        std::vector<Col256> prestlye256 = {};
        /**
         * A vector container that stores objects of type Col256, applied after `text`.
         * 
         * @see Col256
        */
        std::vector<Col256> poststyle256 = {};
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
            .prestyles = {ts::Codes::DIM, ts::Codes::FOREGROUND_CYAN},
            .text = "[DEBUG] ",
            .poststyles = {ts::Codes::DIM_RESET}
        },
        .suffix = {
            .text = "\n" // prestyles and poststyles are omitted
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
            .prestyles = {ts::Codes::DIM, ts::Codes::FOREGROUND_CYAN},
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
```

#### Output

<img alt="color256-output" src="https://github.com/mrmagic2020/termstyle/blob/main/assets/col256-output.png?raw=true" width="300">

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
