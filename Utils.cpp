#include "Utils.h"
#include <iostream>

using namespace std;

void clearScreen() {
    cout << "\033[2J\033[H"; // ANSI escape codes to clear terminal screen
}

void displayWelcomeScreen() {
    cout << "=============================\n";
    cout << "|       Welcome to ATM       |\n";
    cout << "=============================\n";
}
