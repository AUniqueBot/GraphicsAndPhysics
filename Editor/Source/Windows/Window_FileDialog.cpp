#include <windows.h>
#include <commdlg.h>
#include <Windows/Window_FileDialog.h>



namespace Windows {
    std::filesystem::path OpenSaveDialog(const char* _title, const char* _filter) {
        char fileName[MAX_PATH] = "Scene.scene";

        OPENFILENAMEA dialog{};
        dialog.lStructSize = sizeof(dialog);
        dialog.lpstrTitle = _title;
        dialog.lpstrFile = fileName;
        dialog.nMaxFile = MAX_PATH;
        dialog.lpstrFilter = _filter;
        dialog.nFilterIndex = 1;
        dialog.Flags = OFN_OVERWRITEPROMPT | OFN_PATHMUSTEXIST;

        if (GetSaveFileNameA(&dialog)) {
            return std::filesystem::path(fileName);
        }

        return {};
    }
};

namespace Linux {

};




std::filesystem::path FileDialog::SaveFile(const char* title, const char* filter) {
    auto path = Windows::OpenSaveDialog(title, filter);

    if (path.empty()) {
        return {};
    }

    return std::filesystem::relative(
        path,
        std::filesystem::current_path()
    );
}