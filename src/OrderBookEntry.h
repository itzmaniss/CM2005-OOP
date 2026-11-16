#pragma once
#include <string>
using namespace std;

enum class OrderBookType { bid, ask };

class OrderBookEntry {
private:
    double price;
    double amount;
    string timestamp;
    string product;
    OrderBookType orderType;

public:
    OrderBookEntry(double price,
                   double amount,
                   const string& timestamp,
                   const string& product,
                   OrderBookType orderType);

    double getPrice() const;
    double getAmount() const;
    string getTimestamp() const;
    string getProduct() const;
    OrderBookType getOrderBookType() const;
};
