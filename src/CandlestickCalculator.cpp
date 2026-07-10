
#include "CandlestickCalculator.h"
#include "Candlestick.h"
#include <map>
#include <array>

std::vector<Candlestick> CandlestickCalculator::calculate(const std::vector<OrderBookEntry>& orders,
    std::string product,
    OrderBookType orderType,
    TimeGroup timeGroup,
    bool includeBoth) {

    /**
    [0] = open
    [1] = high
    [2] = low
    [3] = close
    */
    std::map<std::string, std::array<double, 4>> candleMap;

    for (const OrderBookEntry& order : orders) {
        bool matchType = includeBoth ? (order.orderType == OrderBookType::ask || order.orderType == OrderBookType::bid)
                                     : (order.orderType == orderType);
        if (order.product == product && matchType) {
            std::string period;
            switch (timeGroup) {
                case TimeGroup::Daily:
                    period = order.timestamp.substr(0, 10);
                    break;
                case TimeGroup::Monthly:
                    period = order.timestamp.substr(0, 7);
                    break;
                case TimeGroup::Yearly:
                    period = order.timestamp.substr(0, 4);
                    break;
            }
            if (candleMap.find(period) == candleMap.end()) {
                candleMap[period] = std::array<double, 4>{order.price, order.price, order.price, order.price};
            }
            else {
                if (order.price > candleMap[period][1]) {
                    candleMap[period][1] = order.price;
                } else if (order.price < candleMap[period][2]) {
                    candleMap[period][2] = order.price;
                }
                candleMap[period][3] = order.price;
            }
        }
    }

    std::vector<Candlestick> candles;
    candles.reserve(candleMap.size());

    for (const auto& candleData: candleMap) {
        candles.push_back(Candlestick{
        candleData.first,
        product,
        orderType,
        candleData.second[0],
        candleData.second[1],
        candleData.second[2],
        candleData.second[3]});
    }

    return candles;
}
 