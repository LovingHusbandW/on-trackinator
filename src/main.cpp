#include "windowcheck.h"
#include <iostream>

int main() {
    WindowCheck* checker = WindowCheck::create();

    const std::string vscodeClassName = "Code"; // or "code" depending on system

    if (checker->isWindowOpen(vscodeClassName)) {
        std::cout << "VS Code is open.\n";

        if (checker->isWindowFocused(vscodeClassName)) {
            std::cout << "VS Code is focused.\n";
        } else {
            std::cout << "VS Code is not focused.\n";
        }
    } else {
        std::cout << "VS Code is not open.\n";
    }

    delete checker;
    return 0;
}
