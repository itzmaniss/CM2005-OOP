
#pragma once
#include <string>
#include "OrderBookEntry.h"

class Candlestick {
public:
    /** Make new candle object with all the price info. */
    Candlestick(std::string timestamp,
        std::string product,
        OrderBookType orderType,
        double open,
        double high,
        double low,
        double close);

    /** Get price when market open. */
    double getOpen() const;
    /** Get highest price. */
    double getHigh() const;
    /** Get lowest price. */
    double getLow() const;
    /** Get price when market close. */
    double getClose() const;
    /** Get which product this is for. */
    std::string getProduct() const;
    /** Get when this candle happen. */
    std::string getTime() const;
    /** Get if this is ask or bid. */
    OrderBookType getOrderType() const;

private:
    double open;
    double high;
    double low;
    double close;
    std::string timestamp;
    std::string product;
    OrderBookType orderType;

};



