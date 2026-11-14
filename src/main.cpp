#include "gui/Application.hpp"
#include <iostream>

int main() {
    try {
        gui::Application app;
        app.run();
    } catch (const std::exception& e) {
        std::cerr << "Fatal Exception occurred: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown fatal error occurred." << std::endl;
        return 2;
    }
    return 0;
}
