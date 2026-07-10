#include "Simulator.h"
#include "OrderBookEntry.h"
#include <map>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <iostream>

Simulator::Simulator(OrderBook& book, UserWallet& wallet, TransactionLog& txLog)
    : book(book), wallet(wallet), txLog(txLog) {}


std::string Simulator::makeTimestamp() {
    std::time_t t = std::time(nullptr);
    std::ostringstream ss;
    ss << std::put_time(std::localtime(&t), "%Y/%m/%d %H:%M:%S");
    return ss.str();
}


double Simulator::perturb(double base) {
    std::uniform_real_distribution<double> dist(0.95, 1.10);
    return base * dist(gen);
}


double Simulator::pickAmount(double price, double balance) {
    std::uniform_real_distribution<double> dist(0.001, 0.01);
    return (balance * dist(gen)) / price;
}

std::string Simulator::run(const std::string& username) {
    std::string timestamp = makeTimestamp();


    std::map<std::string, double> lastAsk, lastBid;
    for (const OrderBookEntry& e : book.getOrderBook()) {
        if (e.username != "dataset") continue;
        if (e.orderType == OrderBookType::ask) lastAsk[e.product] = e.price;
        if (e.orderType == OrderBookType::bid) lastBid[e.product] = e.price;
    }

    int made = 0;
    for (const std::string& product : book.getKnownProducts()) {
        double baseAsk = lastAsk.count(product) ? lastAsk[product] : 0.0;
        double baseBid = lastBid.count(product) ? lastBid[product] : 0.0;
        if (baseAsk <= 0.0 || baseBid <= 0.0) {
            std::cout << "Skipping " << product << " - no baseline price." << std::endl;
            continue;
        }

        for (int i = 0; i < 5; ++i) {

            double askPrice = perturb(baseAsk);
            double askAmt   = pickAmount(askPrice, wallet.getBalance(username));
            OrderBookEntry askEntry{askPrice, askAmt, timestamp, product,
                                    OrderBookType::ask, username};
            book.insertOrder(askEntry);
            txLog.log(Trade{timestamp, product, "ask", askPrice, askAmt, username});
            wallet.deposit(username, askPrice * askAmt);
            ++made;

            double bidPrice = perturb(baseBid);
            double bidAmt   = pickAmount(bidPrice, wallet.getBalance(username));
            double cost     = bidPrice * bidAmt;
            if (!wallet.withdraw(username, cost)) {
                std::cout << "Insufficient balance - skipping bid for " << product << std::endl;
                continue;
            }
            OrderBookEntry bidEntry{bidPrice, bidAmt, timestamp, product,
                                    OrderBookType::bid, username};
            book.insertOrder(bidEntry);
            txLog.log(Trade{timestamp, product, "bid", bidPrice, bidAmt, username});
            ++made;
        }
    }

    std::cout << "Simulation complete: " << made
              << " orders placed at " << timestamp << std::endl;
    return timestamp;
}