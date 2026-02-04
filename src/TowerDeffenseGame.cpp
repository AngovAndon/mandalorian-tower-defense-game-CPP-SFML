#include "Game.h"
#include <iostream>
#include <filesystem> // Requires C++17
#include <exception>

int main() {
    try {
        std::cout << "Current working directory (before Game): " << std::filesystem::current_path() << std::endl;
        Game game;
        game.run();
    }
    catch (const std::exception& ex) {
        std::cerr << "An error occurred: " << ex.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}