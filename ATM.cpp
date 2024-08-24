// ATM.cpp
#include "ATM.h"
#include "UserAccount.h"
#include "Constants.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>  // For sleep_for
#include <chrono>  // For seconds

using namespace std;

ATM::ATM() {
    // Initializing accounts with some default values
    accounts["1234567890"] = UserAccount("1111", 1000.0, 500.0);
    accounts["0987654321"] = UserAccount("2222", 800.0, 200.0);
    accounts["1122334455"] = UserAccount("3333", 1500.0, 700.0);

    loadAccountsFromFile(); // Load accounts from file if available
}

bool ATM::checkDailyLimit(const UserAccount& account, double amount, bool isDeposit) const {
    UserAccount tempAccount = account; // Use a temporary copy to avoid modifying the original
    tempAccount.resetDailyLimits();

    if (isDeposit) {
        return tempAccount.dailyDepositTotal + amount <= DAILY_LIMIT;
    } else {
        return tempAccount.dailyWithdrawTotal + amount <= DAILY_LIMIT;
    }
}

bool ATM::validatePin(const std::string& cardNumber, const std::string& enteredPin) const {
    auto it = accounts.find(cardNumber);
    if (it != accounts.end() && it->second.pin == enteredPin) {
        return true;
    }
    return false;
}

void ATM::showBalance(const std::string& cardNumber, const std::string& accountType) const {
    auto it = accounts.find(cardNumber);
    if (it != accounts.end()) {
        if (accountType == "Savings") {
            cout << "\nYour current Savings balance is: Rs." << it->second.savingsBalance << endl;
        } else if (accountType == "Current") {
            cout << "\nYour current Current balance is: Rs." << it->second.currentBalance << endl;
        }
    } else {
        cout << "\nAccount not found!" << endl;
    }
}

void ATM::deposit(const std::string& cardNumber, double amount, const std::string& accountType) {
    if (amount < MIN_TRANSACTION_AMOUNT) {
        cout << "\nMinimum deposit amount is Rs." << MIN_TRANSACTION_AMOUNT << "." << endl;
        return;
    }

    auto it = accounts.find(cardNumber);
    if (it == accounts.end()) {
        cout << "\nAccount not found!" << endl;
        return;
    }

    UserAccount& account = it->second;

    if (!checkDailyLimit(account, amount, true)) {
        cout << "\nDaily deposit limit exceeded!" << endl;
        return;
    }

    if (accountType == "Savings") {
        account.savingsBalance += amount;
        account.dailyDepositTotal += amount;
        cout << "\nSuccessfully deposited Rs." << amount << " into your Savings account." << endl;
    } else if (accountType == "Current") {
        account.currentBalance += amount;
        account.dailyDepositTotal += amount;
        cout << "\nSuccessfully deposited Rs." << amount << " into your Current account." << endl;
    }

    showBalance(cardNumber, accountType);
}

void ATM::withdraw(const std::string& cardNumber, double amount, const std::string& accountType) {
    if (amount < MIN_TRANSACTION_AMOUNT) {
        cout << "\nMinimum withdrawal amount is Rs." << MIN_TRANSACTION_AMOUNT << "." << endl;
        return;
    }

    auto it = accounts.find(cardNumber);
    if (it == accounts.end()) {
        cout << "\nAccount not found!" << endl;
        return;
    }

    UserAccount& account = it->second;

    if (!checkDailyLimit(account, amount, false)) {
        cout << "\nDaily withdrawal limit exceeded!" << endl;
        return;
    }

    if (accountType == "Savings" && amount <= account.savingsBalance) {
        account.savingsBalance -= amount;
        account.dailyWithdrawTotal += amount;
        cout << "\nSuccessfully withdrew Rs." << amount << " from your Savings account." << endl;
    } else if (accountType == "Current" && amount <= account.currentBalance) {
        account.currentBalance -= amount;
        account.dailyWithdrawTotal += amount;
        cout << "\nSuccessfully withdrew Rs." << amount << " from your Current account." << endl;
    } else {
        cout << "\nInsufficient balance or invalid withdrawal amount!" << endl;
    }

    showBalance(cardNumber, accountType);
}

void ATM::showMenu() const {
    cout << "=============================\n";
    cout << "|     ATM Main Menu         |\n";
    cout << "=============================\n";
    cout << "| 1. Check Balance          |\n";
    cout << "| 2. Deposit Money          |\n";
    cout << "| 3. Withdraw Money         |\n";
    cout << "| 4. Exit                   |\n";
    cout << "=============================\n";
}

void ATM::saveAccountsToFile() const {
    ofstream file("accounts.txt");
    for (const auto& pair : accounts) {
        pair.second.saveToFile(file);
    }
    file.close();
}

void ATM::loadAccountsFromFile() {
    ifstream file("accounts.txt");
    if (!file.is_open()) {
        return; // No file to load from
    }

    string cardNumber, pin, line;
    double savings, current;

    while (getline(file, line)) {
        stringstream ss(line);
        ss >> cardNumber >> pin >> savings >> current;
        accounts[cardNumber] = UserAccount(pin, savings, current);
    }
    file.close();
}

void ATM::run() {
    string cardNumber, pin, accountType;
    int option;
    double amount;

    cout << "\nPlease insert your card (enter card number): ";
    cin >> cardNumber;

    cout << "\nEnter your PIN: ";
    cin >> pin;

    if (!validatePin(cardNumber, pin)) {
        cout << "\nInvalid card number or PIN. Access denied!" << endl;
        return;
    }

    do {
        cout << "\nChoose your account type:\n";
        cout << "1. Savings\n";
        cout << "2. Current\n";
        cout << "Select an option: ";
        int accountOption;
        cin >> accountOption;

        if (accountOption == 1) {
            accountType = "Savings";
        } else if (accountOption == 2) {
            accountType = "Current";
        } else {
            cout << "\nInvalid option! Please choose 1 or 2.\n";
        }
    } while (accountType != "Savings" && accountType != "Current");

    int exitOption = 0;
    do {
        showMenu();
        cout << "\nSelect an option: ";
        cin >> option;

        switch (option) {
            case 1:
                showBalance(cardNumber, accountType);
                break;
            case 2:
                cout << "\nEnter the amount to deposit: Rs.";
                cin >> amount;
                deposit(cardNumber, amount, accountType);
                break;
            case 3:
                cout << "\nEnter the amount to withdraw: Rs.";
                cin >> amount;
                withdraw(cardNumber, amount, accountType);
                break;
            case 4:
                cout << "\nThank you for using the ATM. Goodbye!" << endl;
                saveAccountsToFile(); // Save account data to file
                exitOption = 1; // Break the loop
                // Wait for 4 seconds before returning to the welcome screen
                this_thread::sleep_for(chrono::seconds(4));
                break;
            default:
                cout << "\nInvalid option! Please try again." << endl;
        }

        if (option != 4) {
            cout << "\nPress Enter to return to the menu...";
            cin.ignore(); // To ignore the newline character
            cin.get(); // Wait for Enter key
        }
    } while (exitOption == 0);
}
