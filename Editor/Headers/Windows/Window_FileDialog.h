#pragma once

#include <pch.h>
#include <optional>

class FileDialog {
public:
    static std::optional<std::filesystem::path> SaveFile(
        const char* title,
        const char* filter
    );

    static void OpenWindow(const std::filesystem::path& _path);
};