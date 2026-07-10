#pragma once
#include <vector>
#include <string>
#include "OrderBookEntry.h"
#include "Candlestick.h"

enum class TimeGroup {Daily, Monthly, Yearly};

class CandlestickCalculator {
public:
    /** Calculate candlesticks from a bunch of orders. Can group by day, month or year. */
    static std::vector<Candlestick> calculate(const std::vector<OrderBookEntry>& orders,
        std::string product,
        OrderBookType orderType,
        TimeGroup timeGroup,
        bool includeBoth = false);
private:


};



