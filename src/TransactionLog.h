#pragma once
#include <string>
#include <vector>


struct Trade { std::string timestamp, product, orderType; double price, amount; std::string username; };

class TransactionLog {
public:
    /** Setup where to save the trade history. */
    explicit TransactionLog(const std::string& filename);

    /** Save one trade and update wallet. */
    void log(const Trade& t);                          // append + adjust wallet
    /** Show last few trades for this user. */
    void printRecent(std::string username, int n = 5);
    /** Show some statistics like total volume. */
    void printStats(std::string username);
    /** Make sure the CSV file is there. */
    void ensureFilesExist();

private:
    /** Load trades from file for this user. */
    std::vector<Trade> loadFor(std::string username);
    std::string filename;
};