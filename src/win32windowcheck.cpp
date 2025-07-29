#ifdef _WIN32
#include "windowcheck.h"
#include <string>
#include <windows.h>

class win32windowcheck : public WindowCheck{

public:

    bool isWindowOpen(const std::string& title) override {

    }

    bool isWindowFocused()
};

#endif