#include <iostream>
#include "ATM.h"
#include "Utils.h"  // Include the header file for utility functions

using namespace std;

int main() {
    ATM atm;

    while (true) {
        clearScreen();             // Clear the screen
        displayWelcomeScreen();   // Display the welcome screen
        atm.run();                // Start the ATM session

        // After exiting the ATM session, automatically return to the welcome screen
        // There's no need for user input to continue; the loop will restart after the delay
    }

    return 0;
}
