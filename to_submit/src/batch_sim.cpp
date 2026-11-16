#include "OrderBook.h"
#include "UserWallet.h"
#include "UserManager.h"
#include "TransactionLog.h"
#include "Simulator.h"
#include <iostream>
#include <vector>
#include <string>

/** Custom script to create and run many user simulations at once to populate csv data. */
int main() {
    std::cout << "Starting automated account creation and trading simulation..." << std::endl;

    // Initialize components
    OrderBook orderBook{"./20200317.csv"};
    UserWallet userWallet{"data/USERS_WALLET.CSV"};
    UserManager userManager{userWallet};
    TransactionLog txLog{"data/USERS_TRADING.CSV"};
    Simulator simulator{orderBook, userWallet, txLog};

    // Data for 5 accounts
    struct UserData {
        std::string name;
        std::string email;
        std::string password;
    };

    std::vector<UserData> newUsers = {
        {"Alice Trader", "alice@example.com", "pass123"},
        {"Bob Investor", "bob@example.com", "secure456"},
        {"Charlie Scalper", "charlie@example.com", "charlie789"},
        {"David Holder", "david@example.com", "dave_pass"},
        {"Eve Bot", "eve@example.com", "eve_secret"}
    };

    std::vector<std::string> registeredUsernames;

    for (const auto& user : newUsers) {
        std::cout << "\nRegistering: " << user.name << " (" << user.email << ")" << std::endl;
        if (userManager.registerUserProgrammatic(user.name, user.email, user.password)) {
            std::string username = userManager.getLatestUsername();
            registeredUsernames.push_back(username);
            std::cout << "Successfully registered " << user.name << " with username: " << username << std::endl;
        } else {
            std::cout << "Failed to register " << user.name << " (maybe already exists?)" << std::endl;
            // If already exists, we might want to find the username if we wanted to run sim anyway,
            // but let's assume we want fresh ones or we can just proceed if it's already there.
        }
    }

    std::cout << "\nRunning trading simulation for all accounts..." << std::endl;
    for (const auto& username : registeredUsernames) {
        std::cout << "Simulating for user: " << username << std::endl;
        simulator.run(username);
    }

    std::cout << "\nBatch process complete!" << std::endl;
    return 0;
}
