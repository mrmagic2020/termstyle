#pragma once
#ifndef TERMSTYLE_HPP
#define TERMSTYLE_HPP

#include <stdexcept>

#define TERMSTYLE_NODISCARD [[nodiscard]]

#define TERMSTYLE_ERROR_DEF(parent, name)                         \
protected:                                                        \
    name(std::string ename, std::string msg, int exit_code)       \
        : parent(std::move(ename), std::move(msg), exit_code) {}  \
    name(std::string ename, std::string msg, ExitCodes exit_code) \
        : parent(std::move(ename), std::move(msg), exit_code) {}  \
                                                                  \
public:                                                           \
    name(std::string msg, ExitCodes exit_code)                    \
        : parent(#name, std::move(msg), exit_code) {}             \
    name(std::string msg, int exit_code)                          \
        : parent(#name, std::move(msg), exit_code) {}

#define TERMSTYLE_ERROR_SIMPLE(name) \
    explicit name(std::string msg) : name(#name, msg, ExitCodes::name) {}

enum class ExitCodes : int
{
    SUCCESS = 0,
    IncorrectConstruction = 100,
    PresetNameUsed,
    PresetNotFound,
    BaseClass = 200
};

/**
 * @defgroup Error Errors
 * @brief Base class for all exceptions thrown by the library.
 * @{
 */

/**
 * @brief Base class for all exceptions thrown by the library.
 */
class Error : public std::runtime_error
{
    int exit_code;
    std::string error_name{"Error"};

public:
    TERMSTYLE_NODISCARD int getExitCode() const
    {
        return exit_code;
    }

    TERMSTYLE_NODISCARD std::string getErrorName() const
    {
        return error_name;
    }

    Error(std::string name, std::string msg, int exit_code = static_cast<int>(ExitCodes::BaseClass))
        : std::runtime_error(msg), exit_code(exit_code), error_name(name) {}

    Error(std::string name, std::string msg, ExitCodes exit_code) : Error(name, msg,
                                                                          static_cast<int>(exit_code)) {}
};

/**
 * @brief Thrown when a preset name is already used.
 */
class PresetNameUsed : public Error
{
    TERMSTYLE_ERROR_DEF(Error, PresetNameUsed)
    explicit PresetNameUsed(std::string name)
        : PresetNameUsed("Preset name \"" + name + "\" is already used.", ExitCodes::PresetNameUsed) {}
};

/**
 * @brief Thrown when a preset is not found.
 
*/
class PresetNotFound : public Error
{
    TERMSTYLE_ERROR_DEF(Error, PresetNotFound)
    explicit PresetNotFound(std::string name)
        : PresetNotFound("Preset \"" + name + "\" not found.", ExitCodes::PresetNotFound) {}
};

#undef TERMSTYLE_ERROR_DEF
#undef TERMSTYLE_ERROR_SIMPLE

/** @}*/

#include <ostream>
#include <string>
#include <vector>
#include <map>

/**
 * @brief Namespace for the termstyle library.
 */
namespace termstyle
{

    /**
     * @brief Enum class for ANSI escape codes.
     */
    enum class codes : int
    {
        RESTORE = 0,
        BRIGHT = 1,
        DIM = 2,
        ITALIC = 3,
        UNDERLINE = 4,
        FLASH = 5,
        REVERSE = 7,
        HIDDEN = 8,
        STRIKE = 9,

        BRIGHT_RESET = 22, // reset bright and dim
        DIM_RESET = 22,
        ITALIC_RESET = 23,
        UNDERLINE_RESET = 24,
        FLASH_RESET = 25,
        REVERSE_RESET = 27,
        HIDDEN_RESET = 28,
        STRIKE_RESET = 29,

        FOREGROUND_BLACK = 30,
        FOREGROUND_RED = 31,
        FOREGROUND_GREEN = 32,
        FOREGROUND_YELLOW = 33,
        FOREGROUND_BLUE = 34,
        FOREGROUND_PURPLE = 35,
        FOREGROUND_CYAN = 36,
        FOREGROUND_WHITE = 37,

        FOREGROUND_RESET = 39,

        BACKGROUND_BLACK = 40,
        BACKGROUND_RED = 41,
        BACKGROUND_GREEN = 42,
        BACKGROUND_YELLOW = 43,
        BACKGROUND_BLUE = 44,
        BACKGROUND_PURPLE = 45,
        BACKGROUND_CYAN = 46,
        BACKGROUND_WHITE = 47,

        BACKGROUND_RESET = 49
    };

    /**
     * @brief Converts a list of ANSI escape codes to a string.
    */
    std::string code2string(std::vector<codes> codelist) noexcept
    {
        std::string res = "\033[";
        for (int i = 0; i < codelist.size(); i++)
        {
            res += std::to_string(static_cast<int>(codelist[i]));
            if (i != codelist.size() - 1)
            {
                res += ";";
            }
        }
        res += "m";
        return res;
    }

    /**
     * @brief Struct for storing styled strings.
     */
    struct StyleString
    {
        std::string text;
        std::vector<codes> prestyles;
        std::vector<codes> poststyles;
    };

    /**
     * @brief Struct for storing preset configurations.
     */
    struct PresetConfig
    {
        StyleString prefix;
        StyleString suffix;
    };

    /**
     * @brief Stores all presets.
     */
    std::map<std::string, PresetConfig> presets = {};

    /**
     * @brief Class for styling std::cout.
    */
    class StyledCout
    {
    private:
        const PresetConfig &config;

    public:
        explicit StyledCout(const PresetConfig &config) : config(config)
        {
            std::cout << code2string(config.prefix.prestyles)
                      << config.prefix.text
                      << code2string(config.prefix.poststyles);
        }

        ~StyledCout()
        {
            std::cout << code2string(config.suffix.prestyles)
                      << config.suffix.text
                      << code2string(config.suffix.poststyles);
        }

        template<typename T>
        std::ostream& operator<<(const T& value)
        {
            return std::cout << value;
        }
    };

    /**
     * @brief Returns a StyledCout object with the specified preset.
    */
    StyledCout style(const std::string &preset)
    {
        if (presets.find(preset) == presets.end())
        {
            throw PresetNotFound(preset);
        }
        return StyledCout(presets[preset]);
    }

    /**
     * @brief Adds a preset to the presets map.
    */
    void addPreset(std::string name, PresetConfig config)
    {
        if (presets.find(name) != presets.end()) // preset already exists
        {
            throw PresetNameUsed(name);
        }
        presets[name] = std::move(config);
    }

    /**
     * @brief Prints a styled string using a specified preset.
     */
    void print(std::string preset, std::string text)
    {
        if (presets.find(preset) == presets.end())
        {
            throw PresetNotFound(preset);
        }
        const PresetConfig &config = presets[preset];
        std::cout << code2string(config.prefix.prestyles)
                  << config.prefix.text
                  << code2string(config.prefix.poststyles)
                  << text
                  << code2string(config.suffix.prestyles)
                  << config.suffix.text
                  << code2string(config.suffix.poststyles);
    }

} // namespace termstyle

#endif // TERMSTYLE_HPP
