/**
 * @file termstyle.hpp
 * @brief Header file for the termstyle library.
 * 
 * This file contains the declarations of classes, structs, and functions used in the termstyle library.
 * The library allows you to create presets and style your terminal output using ANSI escape codes.
 */

#pragma once
#ifndef TERMSTYLE_HPP
#define TERMSTYLE_HPP

#include <iostream>
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
    BadColorID,
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

class BadColorID : public Error
{
    TERMSTYLE_ERROR_DEF(Error, BadColorID)
    TERMSTYLE_ERROR_SIMPLE(BadColorID);
};

#undef TERMSTYLE_ERROR_DEF
#undef TERMSTYLE_ERROR_SIMPLE

/** @}*/

#include <string>
#include <vector>
#include <map>

/**
 * @brief Namespace for the termstyle library.
 */
namespace termstyle
{
    /**
     * @brief Configuration namespace.
    */
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

    /**
     * @defgroup Codes Col16
     * Contents related to Col16.
     * @{
    */

    /**
     * @brief Enum class for ANSI escape codes.
     */
    enum class Codes : int
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
     * Converts a vector of Color16 codes to a string representation.
     *
     * @param codelist The vector of codes to convert.
     * @return The string representation of the codes.
     */
    template<typename T>
    std::string code2string(const std::vector<T>& codelist) noexcept
    {
        if (codelist.empty()) return "";
        std::string res = "\033[";
        for (size_t i = 0; i < codelist.size(); i++)
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

    /** @} */

    /**
     * @brief Enum class for color modes.
    */
    enum class ColorMode : int
    {
        FOREGROUND = 38,
        BACKGROUND = 48
    };

    /**
     * @defgroup validateColorID Col256
     * Contents related to Col256.
     * @{
     */

    /**
     * @brief Validates a color 256 ID.
     *
     * This function takes an integer color ID as input and validates it.
     * It returns true if the color ID is valid, and false otherwise.
     *
     * @param ID The color ID to be validated.
     * @return True if the color ID is valid, false otherwise.
     */
    bool validateColorID(int ID)
    {
        return ID >= 0 && ID <= 255;
    }

    /**
     * @brief Struct for storing 256-color codes.
    */
    struct Col256
    {
        /**
         * The color mode (foreground or background).
         * 
         * @see ColorMode
        */
        ColorMode mode;
        /**
         * The color ID.
         * 
         * @see https://gist.github.com/fnky/458719343aabd01cfb17a3a4f7296797#256-colors
        */
        int ID;
        explicit Col256(ColorMode mode, int ID) : mode(mode), ID(ID) {
            if (!validateColorID(ID))
            {
                throw BadColorID("Invalid color ID: " + std::to_string(ID));
            }
        }
    };

    /**
     * Converts a vector of Col256 objects to a string representation.
     *
     * @param codelist The vector of Col256 objects to convert.
     * @return A string representation of the Col256 objects.
     */
    std::string col256_2string(std::vector<Col256> codelist) noexcept
    {
        if (codelist.empty()) return "";
        std::string res = "\033[";
        for (size_t i = 0; i < codelist.size(); i++)
        {
            res += std::to_string(static_cast<int>(codelist[i].mode)) + ";5;" + std::to_string(codelist[i].ID);
            if (i != codelist.size() - 1)
            {
                res += ";";
            }
        }
        res += "m";
        return res;
    }

    /** @} */

    /**
     * @brief Struct for storing styled strings.
     */
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

    /**
     * @brief Struct for storing preset configurations.
     */
    struct PresetConfig
    {
        StyleString prefix;
        StyleString suffix;
        Config config;
    };

    /**
     * @brief A map that stores preset configurations.
     *
     * This map is used to store preset configurations for termstyle.
     * The keys are strings representing the names of the presets,
     * and the values are instances of the `PresetConfig` class.
     */
    std::map<std::string, PresetConfig> presets = {};

    /**
     * @brief Enum class for parse modes.
     */
    enum class ParseMode : int
    {
        ALL = 0,
        PREFIX = 1,
        SUFFIX = 2
    };

    /**
     * Parses the given `preset` configuration using the specified `mode`.
     *
     * @param preset The preset configuration to parse.
     * @param mode The parse mode to use (default: `ParseMode::ALL`).
     * @return A string representing the parsed configuration.
     */
    std::string parse(const PresetConfig &preset, ParseMode mode = ParseMode::ALL) noexcept
    {
        if (mode == ParseMode::PREFIX)
        {
            return code2string(preset.prefix.prestyles)
                   + col256_2string(preset.prefix.prestlye256)
                   + preset.prefix.text
                   + code2string(preset.prefix.poststyles)
                   + col256_2string(preset.prefix.poststyle256);
        }
        if (mode == ParseMode::SUFFIX)
        {
            return code2string(preset.suffix.prestyles)
                   + col256_2string(preset.suffix.prestlye256)
                   + preset.suffix.text
                   + code2string(preset.suffix.poststyles)
                   + col256_2string(preset.suffix.poststyle256)
                   + (preset.config.trailing_newline ? "\n" : "");
        }
        if (mode == ParseMode::ALL)
        {
            return code2string(preset.prefix.prestyles)
                   + col256_2string(preset.prefix.prestlye256)
                   + preset.prefix.text
                   + code2string(preset.prefix.poststyles)
                   + col256_2string(preset.prefix.poststyle256)
                   + code2string(preset.suffix.prestyles)
                   + col256_2string(preset.suffix.prestlye256)
                   + preset.suffix.text
                   + code2string(preset.suffix.poststyles)
                   + col256_2string(preset.suffix.poststyle256)
                   + (preset.config.trailing_newline ? "\n" : "");
        }
        return "";
    }

    /**
     * @brief A class that provides styled output to the standard output stream.
     * 
     * The `StyledCout` class allows you to output text with custom styles to the standard output stream (`std::cout`).
     * It takes a `PresetConfig` object as a parameter, which contains the configuration for the styles to be applied.
     * The styles are applied to the text when it is printed to the standard output stream.
     */
    class StyledCout
    {
    private:
        /**
         * The configuration for the preset style.
         * 
         * @see PresetConfig
        */
        const PresetConfig &config;

    public:
        explicit StyledCout(const PresetConfig &config) : config(config)
        {
            std::cout << parse(config, ParseMode::PREFIX);
        }

        ~StyledCout()
        {
            std::cout << parse(config, ParseMode::SUFFIX);
        }

        template<typename T>
        std::ostream& operator<<(const T& value)
        {
            return std::cout << value;
        }
    };

    /**
     * Applies a specific style preset to the output stream.
     *
     * @param preset The name of the style preset to apply.
     * @return A `StyledCout` object that can be used to chain additional styling or output operations.
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
     * @brief Adds a preset with the given name and configuration.
     *
     * This function adds a preset with the specified name and configuration to the termstyle library.
     *
     * @param name The name of the preset.
     * @param preset The configuration for the preset.
     */
    void addPreset(std::string name, PresetConfig preset)
    {
        if (presets.find(name) != presets.end()) // preset already exists
        {
            throw PresetNameUsed(name);
        }

        if (preset.config.leading_restore)
        {
            preset.prefix.prestyles.insert(preset.prefix.prestyles.begin(), Codes::RESTORE);
        }
        if (preset.config.trailing_restore)
        {
            preset.suffix.poststyles.emplace_back(Codes::RESTORE);
        }

        presets[name] = std::move(preset);
    }

    
    /**
     * Prints the specified text using the given preset style.
     *
     * @param preset The preset style to apply to the text.
     * @param text   The text to be printed. If not provided, an empty string will be printed.
     */
    void print(std::string preset, std::string text = "")
    {
        if (presets.find(preset) == presets.end())
        {
            throw PresetNotFound(preset);
        }
        const PresetConfig &config = presets[preset];
        std::cout << parse(config, ParseMode::PREFIX) << text << parse(config, ParseMode::SUFFIX);
    }

    /**
     * @brief The `OnExit` class is a helper class that performs an action when it goes out of scope.
     * 
     * This class is designed to be used with the RAII (Resource Acquisition Is Initialization) idiom.
     * When an object of the `OnExit` class is created, it performs the specified action in its destructor
     * when it goes out of scope, regardless of how the scope is exited (e.g., normal exit, exception, etc.).
     * 
     * In this specific implementation, the `OnExit` class is used to restore the terminal style by printing
     * the restore code when the object goes out of scope.
     */
    class OnExit
    {
    public:
        /**
         * @brief Destructor for the `OnExit` class.
         * 
         * This destructor is responsible for performing the specified action when the `OnExit` object goes out of scope.
         * In this implementation, it prints the restore code to restore the terminal style.
         */
        ~OnExit()
        {
            std::cout << code2string<Codes>({Codes::RESTORE});
        }
    };

    OnExit onExitInstance;

} // namespace termstyle

#endif // TERMSTYLE_HPP
