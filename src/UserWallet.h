#pragma once
#include <string>
#include <map>
#include <vector>

class UserWallet {
public:
    /** Setup wallet and load data from file. */
    explicit UserWallet(const std::string& filename);                                   // loads USERS_WALLET.CSV

    /** Make new account for user and give some money. */
    void   createAccount(std::string username, double initialBalance);  // Task 2 sign-up bonus
    /** Check if this user got account already. */
    bool   hasAccount(std::string username);
    /** Check how much money user have. */
    double getBalance(std::string username);
    /** Add money to user account. */
    void   deposit(std::string username, double amount);
    /** Take money from user account. Return false if not enough. */
    bool   withdraw(std::string username, double amount);               // false if insufficient
    /** Ask user how much they want to put in. */
    void   depositFlow(std::string username);                           // interactive prompt
    /** Ask user how much they want to take out. */
    void   withdrawFlow(std::string username);                          // interactive prompt

private:
    /** Make sure CSV file is there. */
    void ensureFilesExist();
    /** Load all balances from file. */
    void load();
    /** Save all balances to file. */
    void save();

    std::string filename;
    std::map<std::string, double> balances;
    /** Print message and get number from user. */
    static double readAmount(const std::string& prompt);
};