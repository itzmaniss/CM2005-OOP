#include "OrderBookEntry.h"

OrderBookEntry::OrderBookEntry(double price,
                               double amount,
                               const string& timestamp,
                               const string& product,
                               OrderBookType orderType)
    : price(price),
      amount(amount),
      timestamp(timestamp),
      product(product),
      orderType(orderType)
{}

double OrderBookEntry::getPrice() const { return price; }
double OrderBookEntry::getAmount() const { return amount; }
string OrderBookEntry::getTimestamp() const { return timestamp; }
string OrderBookEntry::getProduct() const { return product; }
OrderBookType OrderBookEntry::getOrderBookType() const { return orderType; }
