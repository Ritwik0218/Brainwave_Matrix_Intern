#include "UserAccount.h"
#include "Constants.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>

using namespace std;

UserAccount::UserAccount() : pin(""), savingsBalance(0.0), currentBalance(0.0), dailyDepositTotal(0.0), dailyWithdrawTotal(0.0) {
    lastTransactionDate = time(0); // Initialize with current time
}

UserAccount::UserAccount(string p, double s, double c) : pin(p), savingsBalance(s), currentBalance(c), dailyDepositTotal(0.0), dailyWithdrawTotal(0.0) {
    lastTransactionDate = time(0); // Initialize with current time
}

bool UserAccount::isNewDay() const {
    time_t currentTime = time(0);
    struct tm* timeInfo = localtime(&currentTime);
    struct tm* lastTimeInfo = localtime(&lastTransactionDate);

    return (timeInfo->tm_yday != lastTimeInfo->tm_yday) || (timeInfo->tm_year != lastTimeInfo->tm_year);
}

void UserAccount::resetDailyLimits() const {
    if (isNewDay()) {
        const_cast<UserAccount*>(this)->dailyDepositTotal = 0.0;
        const_cast<UserAccount*>(this)->dailyWithdrawTotal = 0.0;
        const_cast<UserAccount*>(this)->lastTransactionDate = time(0);
    }
}

void UserAccount::loadFromFile(ifstream& file) {
    // Implement loading from file
}

void UserAccount::saveToFile(ofstream& file) const {
    // Implement saving to file
}
