
#include "Candlestick.h"

Candlestick::Candlestick(std::string timestamp,
                         std::string product,
                         OrderBookType orderType,
                         double open,
                         double high,
                         double low,
                         double close)
: timestamp(timestamp),
  product(product),
  orderType(orderType),
  open(open),
  high(high),
  low(low),
  close(close)
{}

double Candlestick::getClose() const {
    return close;
}

double Candlestick::getOpen() const {
    return open;
}

double Candlestick::getHigh() const {
    return high;
}

double Candlestick::getLow() const {
    return low;
}

std::string Candlestick::getProduct() const {
    return product;
}

std::string Candlestick::getTime() const {
    return timestamp;
}

OrderBookType Candlestick::getOrderType() const {
    return orderType;
}
