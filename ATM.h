// ATM.h
#ifndef ATM_H
#define ATM_H

#include "UserAccount.h"
#include <string>
#include <unordered_map>

class ATM {
public:
    ATM();
    void run();

private:
    bool checkDailyLimit(const UserAccount& account, double amount, bool isDeposit) const;
    bool validatePin(const std::string& cardNumber, const std::string& enteredPin) const;
    void showBalance(const std::string& cardNumber, const std::string& accountType) const;
    void deposit(const std::string& cardNumber, double amount, const std::string& accountType);
    void withdraw(const std::string& cardNumber, double amount, const std::string& accountType);
    void showMenu() const;
    void saveAccountsToFile() const;
    void loadAccountsFromFile();

    std::unordered_map<std::string, UserAccount> accounts; // Card number -> UserAccount
};

#endif // ATM_H
