#pragma once

#include <pch.h>
#include <optional>
#include <serialization/serialize_helperfunctions.h>

class FileDialog {
public:
    static std::filesystem::path SaveFile(
        const char* title,
        const char* filter
    );

    static void OpenWindow(const std::filesystem::path& _path);
};