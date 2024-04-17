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

### Creating a preset

Preset configurations are constructed with `termstyle::PresetConfig`.

```hpp
struct PresetConfig
{
    StyleString prefix;
    StyleString suffix;
};
```

It contains two `termstyle::StyleString` structs - `prefix` and `suffix`.

- A `prefix` is inserted before the text.

- A `suffix` is inserted after the text.

A `StyleString` struct looks like

```hpp
struct StyleString
{
    std::string text = "";
    std::vector<Codes> prestyles = {};
    std::vector<Codes> poststyles = {};
};
```

- `text` is the body of text to be inserted.

- `prestyles` and `poststyles` are lists of ANSI escape codes to be applied before and after `text`, respectively. A list of `Codes` can be found [here](https://mrmagic2020.github.io/termstyle/namespacetermstyle.html#a452219ccabb47a06a4046ae22252ed6c).

> Note that you don't need to include `termstyle::Codes::RESTORE` at the end of a PresetConfig. It is, by default, automatically inserted at the end of `preset.suffix.poststyle`. For more information on `termstyle::config`, see [Adjusting Configuraion](#adjusting-configuration).

#### Example

```cpp
#include "./include/termstyle.hpp"
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

### Adjusting Configuration

There are two variables stored in `termstyle::config`.

- `bool leading restore = true` Whether to automatically restore style before printing.

- `bool trailing restore = true` Whether to automatically restore style after printing.

You might need to adjust them when [adding presets for fancy input](#fancy-input). Only presets added after changing these values are affected by the change.

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
    };

    ts::addPreset("debug", debug_preset);
    ts::print("debug", "This is a debug message printed using termstyle::print.");
    ts::style("debug") << "This is a debug message printed using termstyle::style.";
    
    return 0;
}
```

#### Output

<img alt="basic-usage-output" src="https://github.com/mrmagic2020/termstyle/blob/dev-1.0.0/assets/basic-usage-output.png?raw=true" width="750">

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
        }
    };

    // We need to disable auto restore after the line
    // since we want to change the style of user input
    ts::config::trailing_restore = false;

    ts::addPreset("input", input_preset);

    std::string input;
    ts::print("input");
    scanf("%s", &input);
    ts::style("input");
    scanf("%s", &input);
    
    return 0;
}
```

#### Output

<img alt="fancy-input-output" src="https://github.com/mrmagic2020/termstyle/blob/main/assets/fancy-input-output.png?raw=true" width="750">
