#ifdef _WIN32
#include "windowcheck.h"
#include <string>
#include <windows.h>
#include <iostream>


class win32windowcheck : public WindowCheck {

private:
    // Convert std::string (UTF-8) to std::wstring (UTF-16) using Windows API
    std::wstring toWString(const std::string& str) {
        if (str.empty()) return std::wstring();
        int size_needed = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), NULL, 0);
        std::wstring wstr(size_needed, 0);
        MultiByteToWideChar(CP_UTF8, 0, str.c_str(), (int)str.size(), &wstr[0], size_needed);
        return wstr;
    }

public:
    // Wide string version
    bool isWindowOpen(const std::wstring& title) {
        struct SearchData {
            std::wstring target;
            bool found;

            // Constructor to allow initialization
            SearchData(const std::wstring& t, bool f) : target(t), found(f) {}
        } data{title, false};

        auto callback = [](HWND hwnd, LPARAM lParam) -> BOOL {
            wchar_t windowTitle[256];
            GetWindowTextW(hwnd, windowTitle, sizeof(windowTitle) / sizeof(wchar_t));

            if (wcslen(windowTitle) > 0) {
                SearchData* data = reinterpret_cast<SearchData*>(lParam);
                if (std::wstring(windowTitle).find(data->target) != std::wstring::npos) {
                    data->found = true;
                    return FALSE; // Stop enumeration
                }
            }
            return TRUE; // Continue enumeration
        };

        EnumWindows(callback, reinterpret_cast<LPARAM>(&data));
        return data.found;
    }

    // Narrow string version (overload)
    bool isWindowOpen(const std::string& title) {
        return isWindowOpen(toWString(title));
    }

    // Wide string version
    bool isWindowFocused(const std::wstring& title) {
        HWND foreground = GetForegroundWindow();
        if (!foreground) return false;

        wchar_t wTitle[256];
        GetWindowTextW(foreground, wTitle, sizeof(wTitle) / sizeof(wchar_t));

        return std::wstring(wTitle).find(title) != std::wstring::npos;
    }

    // Narrow string version (overload)
    bool isWindowFocused(const std::string& title) {
        return isWindowFocused(toWString(title));
    }

    // Play a .wav sound from the sounds/ directory asynchronously without opening an external program
    void playSound(const std::string& sound) {
        const std::string fullPath = "sounds/" + sound;
        std::wstring wPath = toWString(fullPath);
        PlaySoundW(wPath.c_str(), NULL, SND_FILENAME | SND_ASYNC);
    }

};

WindowCheck* WindowCheck::create() {
    return new win32windowcheck();
}

#endif
