#include "windowcheck.h"
#include <iostream>
#include <random>
#include <string>
#include <chrono>
#include <thread>

int main() {

    WindowCheck* checker = WindowCheck::create();
    bool running = true;
    char cont = 'Y';

    while(running) {
        std::cout << "You better be working I'll check" << std::endl;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dist(2,30); //Random time check in min
        int time = dist(gen);
        std::cout << "[INFO] time in minutes: " + std::to_string(time) << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(time));
        if(!checker->isWindowOpen("Visual Studio Code")) {
            std::cout << "[INFO] VSCode not open" << std::endl;
            checker->playSound(""); //VSCode not open, plays bad sound
        }
        else if(!checker->isWindowFocused("Visual Studio Code")) {
            std::cout << "[INFO] VSCode is open, not focused" << std::endl;
            checker->playSound("metal_pipe.wav"); //VSCode open, not focused, plays metal pipes
        }
        else{
            std::cout << "[INFO] VSCode open and focused" << std::endl;
            checker->playSound(""); //VSCode open and focused, plays happy sound
        }
        
        std::cout << "Are you still working? (Y/N)" << std::endl;
        std::cin >> cont;
        if(toupper(cont) == 'N') {
            std::cout << "Are you sure you don't still have work to do? (Y/N)" << std::endl;
            std::cin >> cont;
            if(toupper(cont) == 'N') {
                running = false;
            }
        }
    }

    return 0;
}
