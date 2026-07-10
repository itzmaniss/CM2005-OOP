#pragma once

#include <string>
#include <random>
#include "OrderBook.h"
#include "UserWallet.h"
#include "TransactionLog.h"

// Task 4: THIS IS MY UNDERSTANDING OF TASK 4 i was pretty confused of the requirements in task 4 and hope i understood
// it correctly

// generates 50 simulated orders (5 asks + 5 bids per product) for the
// logged-in user, prices derived from the last historical price +/- variation,
// all stamped with the current system time. Settles directly against the
// user's wallet at placement (ask credits, bid debits) and logs every order.

class Simulator {
public:
    /** Setup simulator with book, wallet and log. */
    Simulator(OrderBook& book, UserWallet& wallet, TransactionLog& txLog);

    /** Run the auto trader for this user. */
    std::string run(const std::string& username);

private:
    /** Create a fake timestamp. */
    std::string makeTimestamp();
    /** Change price a bit randomly. */
    double perturb(double base);
    /** Decide how much to buy or sell. */
    double pickAmount(double price, double balance);
    OrderBook& book;
    UserWallet& wallet;
    TransactionLog& txLog;

    std::mt19937 gen{std::random_device{}()};
};