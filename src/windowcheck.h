#pragma once
#include <string>

class WindowCheck {
public:
    static WindowCheck* create();
    virtual ~WindowCheck() = default;

    virtual bool isWindowOpen(const std::string& title) = 0;
    virtual bool isWindowFocused(const std::string& title) = 0;
    virtual void playSound(const std::string& sound) = 0;
};