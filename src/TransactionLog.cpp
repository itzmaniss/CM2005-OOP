#include "./TransactionLog.h"
#include "CSVReader.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <map>
#include <set>

TransactionLog::TransactionLog(const std::string& filename) : filename(filename) {
    ensureFilesExist();
}

void TransactionLog::ensureFilesExist() {
    std::ifstream check{filename};
    if (!check.is_open()) {
        std::ofstream create{filename};
    }
}

void TransactionLog::log(const Trade& t) {
    std::ofstream out{filename, std::ios::app};                 // append mode
    if (!out.is_open()) {
        std::cerr << "TransactionLog::log bad file: " << filename << std::endl;
        return;
    }
    out << t.timestamp << "," << t.product << "," << t.orderType << ","
        << std::fixed << std::setprecision(2) << t.price << ","
        << t.amount << "," << t.username << "\n";
}

std::vector<Trade> TransactionLog::loadFor(std::string username) {
    std::vector<Trade> trades;
    std::ifstream file{filename};
    if (!file.is_open()) return trades;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::vector<std::string> tk = CSVReader::tokenise(line, ',');
        if (tk.size() != 6) continue;
        try {
            Trade t{ tk[0], tk[1], tk[2], std::stod(tk[3]), std::stod(tk[4]), tk[5] };
            if (t.username == username) trades.push_back(t);
        } catch (const std::exception& e) { }
    }
    return trades;
}

void TransactionLog::printRecent(std::string username, int n) {
    std::vector<Trade> trades = loadFor(username);
    std::cout << "--- Last " << n << " transactions ---" << std::endl;
    if (trades.empty()) { std::cout << "No transactions yet." << std::endl; return; }

    int start = static_cast<int>(trades.size()) - n;
    if (start < 0) start = 0;
    std::cout << std::left
              << std::setw(26) << "Timestamp" << std::setw(10) << "Product"
              << std::setw(6)  << "Type"      << std::setw(12) << "Price"
              << std::setw(12) << "Amount" << std::endl;
    for (int i = start; i < static_cast<int>(trades.size()); ++i) {
        const Trade& t = trades[i];
        std::cout << std::left
                  << std::setw(26) << t.timestamp << std::setw(10) << t.product
                  << std::setw(6)  << t.orderType << std::setw(12) << t.price
                  << std::setw(12) << t.amount << std::endl;
    }
}

void TransactionLog::printStats(std::string username) {
    std::vector<Trade> trades = loadFor(username);
    if (trades.empty()) { std::cout << "No transactions yet." << std::endl; return; }

    std::cout << "From (YYYY/MM/DD, blank = all): ";
    std::string from; std::getline(std::cin, from);
    std::cout << "To   (YYYY/MM/DD, blank = all): ";
    std::string to;   std::getline(std::cin, to);

    int totalAsks = 0, totalBids = 0;
    std::map<std::string, int> asksByProduct;
    std::map<std::string, int> bidsByProduct;
    double moneySpent = 0.0;

    for (const Trade& t : trades) {
        if (!from.empty() && t.timestamp < from) continue;
        if (!to.empty()   && t.timestamp > to)   continue;

        if (t.orderType == "ask") {
            ++totalAsks;
            ++asksByProduct[t.product];
        } else if (t.orderType == "bid") {
            ++totalBids;
            ++bidsByProduct[t.product];
            moneySpent += t.price * t.amount;
        }
    }

    std::cout << "\n--- Activity summary for " << username << " ---" << std::endl;
    if (!from.empty() || !to.empty()) {
        std::cout << "Timeframe: " << (from.empty() ? "start" : from)
                  << " to " << (to.empty() ? "now" : to) << std::endl;
    }
    std::cout << "Total asks: " << totalAsks << "   Total bids: " << totalBids << std::endl;

    std::cout << "\nPer product:" << std::endl;
    std::set<std::string> products;
    for (const auto& p : asksByProduct) products.insert(p.first);
    for (const auto& p : bidsByProduct) products.insert(p.first);
    for (const std::string& prod : products) {
        std::cout << "  " << std::left << std::setw(10) << prod
                  << asksByProduct[prod] << " asks, "
                  << bidsByProduct[prod] << " bids" << std::endl;
    }

    std::cout << "\nTotal money spent (bids) in timeframe: "
              << std::fixed << std::setprecision(2) << moneySpent << std::endl;
}