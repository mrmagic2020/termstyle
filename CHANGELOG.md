# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog],
and this project adheres to [Semantic Versioning].

## [Unreleased]

## [1.0.0-pre.3] - 2024-04-19

### Added

- Support for RGB colors.

### Changed

- The three types of colors (Color16, Color256, and RGB) are now stored in a union in `struct Color`.

- In `struct StyleString`, members `prestyles` and `poststyles` are now `std::vector<Color>` instead of `std::vector<Codes>`.

### Deprecated

- Some members of `struct StyleString` are deprecated, and will be removed in the next release.

  - `prestyle256` is deprecated, use `prestyles` instead.

  - `poststyle256` is deprecated, use `poststyles` instead.

## [1.0.0-pre.2] - 2024-04-17

### Added

- Support for Color256.

- Configuration for automatic new lines after printing.

### Removed

- Removed namespace `termstyle::config`.

### Fixed

- Fix an issue where `poststyles` may be applied after a new line character.

- STL include issues.

- Improve code readability.

## [1.0.0-pre.1] - 2024-04-16

- initial release

<!-- Links -->
[keep a changelog]: https://keepachangelog.com/en/1.0.0/
[semantic versioning]: https://semver.org/spec/v2.0.0.html

<!-- Versions -->
[unreleased]: https://github.com/mrmagic2020/termstyle/compare/v1.0.0-pre.3...HEAD
[1.0.0-pre.3]: https://github.com/mrmagic2020/termstyle/releases/v1.0.0-pre.3
[1.0.0-pre.2]: https://github.com/mrmagic2020/termstyle/releases/v1.0.0-pre.2
[1.0.0-pre.1]: https://github.com/mrmagic2020/termstyle/releases/v1.0.0-pre.1
