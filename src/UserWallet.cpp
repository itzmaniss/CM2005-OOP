#include "UserWallet.h"
#include "CSVReader.h"
#include <fstream>
#include <iostream>
#include <iomanip>

UserWallet::UserWallet(const std::string& filename) : filename(filename) {
    ensureFilesExist();
    load();
}


void UserWallet::ensureFilesExist() {
    std::ifstream check{filename};
    if (!check.is_open()) {
        std::ofstream create{filename};
    }
}


void UserWallet::load() {
    balances.clear();
    std::ifstream file{filename};
    if (!file.is_open()) return;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::vector<std::string> tokens = CSVReader::tokenise(line, ',');
        if (tokens.size() != 2) continue;
        try {
            balances[tokens[0]] = std::stod(tokens[1]);
        } catch (const std::exception& e) {

        }
    }
}

void UserWallet::save() {
    std::ofstream file{filename};
    if (!file.is_open()) {
        std::cerr << "UserWallet::save bad file: " << filename << std::endl;
        return;
    }
    for (const auto& entry : balances) {
        file << entry.first << ","
             << std::fixed << std::setprecision(2) << entry.second << "\n";
    }
}


void UserWallet::createAccount(std::string username, double initialBalance) {
    balances[username] = initialBalance;
    save();
}

bool UserWallet::hasAccount(std::string username) {
    return balances.find(username) != balances.end();
}

double UserWallet::getBalance(std::string username) {
    if (hasAccount(username)) return balances[username];
    return 0.0;
}

void UserWallet::deposit(std::string username, double amount) {
    balances[username] += amount;
    save();
}

bool UserWallet::withdraw(std::string username, double amount) {
    if (getBalance(username) < amount) return false;   // insufficient -> caller decides message
    balances[username] -= amount;
    save();
    return true;
}

void UserWallet::depositFlow(std::string username) {
    double amount = readAmount("Deposit amount: ");
    if (amount <= 0) { std::cout << "Deposit cancelled." << std::endl; return; }
    deposit(username, amount);
    std::cout << "Deposited. New balance: " << getBalance(username) << std::endl;
}

void UserWallet::withdrawFlow(std::string username) {
    double amount = readAmount("Withdraw amount: ");
    if (amount <= 0) { std::cout << "Withdrawal cancelled." << std::endl; return; }
    if (!withdraw(username, amount)) {
        std::cout << "Insufficient funds. Balance unchanged: "
                  << getBalance(username) << std::endl;
    } else {
        std::cout << "Withdrawn. New balance: " << getBalance(username) << std::endl;
    }
}

double UserWallet::readAmount(const std::string& prompt) {
    while (true) {
        std::cout << prompt;
        std::string line;
        if (!std::getline(std::cin, line) || line.empty()) return -1.0;  // cancel / EOF
        try {
            double value = std::stod(line);
            if (value > 0) return value;
            std::cout << "Amount must be greater than zero." << std::endl;
        } catch (const std::exception& e) {
            std::cout << "Please enter a valid number." << std::endl;
        }
    }
}