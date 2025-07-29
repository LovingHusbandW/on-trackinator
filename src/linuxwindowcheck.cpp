#ifdef __linux__

#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <iostream>
#include <cstring>
#include <cstdlib>
#include "windowcheck.h"

class LinuxWindowCheck : public WindowCheck {
public:
    bool isWindowOpen(const std::string& title) override {
        Display* display = XOpenDisplay(nullptr);
        if (!display) {
            std::cerr << "Unable to open X display\n";
            return false;
        }

        Window root = DefaultRootWindow(display);
        Window parent;
        Window* children;
        unsigned int nchildren;

        if (!XQueryTree(display, root, &root, &parent, &children, &nchildren)) {
            std::cerr << "XQueryTree failed\n";
            XCloseDisplay(display);
            return false;
        }

        bool found = false;

        for (unsigned int i = 0; i < nchildren && !found; ++i) {
            XClassHint classHint;
            if (XGetClassHint(display, children[i], &classHint)) {
                if (classHint.res_class && std::strcmp(classHint.res_class, title.c_str()) == 0) {
                    found = true;
                }
                if (classHint.res_name) XFree(classHint.res_name);
                if (classHint.res_class) XFree(classHint.res_class);
            }
        }

        if (children) XFree(children);
        XCloseDisplay(display);
        return found;
    }



    bool isWindowFocused(const std::string& title) override {
        Display* display = XOpenDisplay(nullptr);
        if (!display) {
            std::cerr << "Cannot open display\n";
            return false;
        }

        Window focusedWindow;
        int revertTo;
        XGetInputFocus(display, &focusedWindow, &revertTo);

        // 0 or PointerRoot are invalid for property lookup
        if (focusedWindow == None || focusedWindow == PointerRoot) {
            std::cerr << "No valid focused window\n";
            XCloseDisplay(display);
            return false;
        }

        XClassHint classHint;
        if (XGetClassHint(display, focusedWindow, &classHint)) {
            bool match = (classHint.res_class && std::strcmp(classHint.res_class, "Code") == 0);

            if (classHint.res_name) XFree(classHint.res_name);
            if (classHint.res_class) XFree(classHint.res_class);

            XCloseDisplay(display);
            return match;
        }

        XCloseDisplay(display);
        return false;
    }



    void playSound(){
        system("paplay sounds/metal_pipe.wav");
    }
    
};

WindowCheck* WindowCheck::create() {
    return new LinuxWindowCheck();
}

#endif