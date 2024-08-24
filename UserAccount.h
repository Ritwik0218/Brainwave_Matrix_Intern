// UserAccount.h
#ifndef USERACCOUNT_H
#define USERACCOUNT_H

#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

class UserAccount {
public:
    std::string pin;
    double savingsBalance;
    double currentBalance;
    double dailyDepositTotal;
    double dailyWithdrawTotal;
    std::time_t lastTransactionDate;

    UserAccount();
    UserAccount(std::string p, double s, double c);

    void saveToFile(std::ofstream& file) const;
    void loadFromFile(std::ifstream& file);
    void resetDailyLimits() const;

private:
    bool isNewDay() const;
};

#endif // USERACCOUNT_H
