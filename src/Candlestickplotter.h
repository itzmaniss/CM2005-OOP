#pragma once
#include <vector>
#include <string>
#include "Candlestick.h"

/**
 *THIS COMMENT WAS AI GENERATED FOR ME TO HAVE A CLEARLY STATED IDEA ON WHAT I WANTED TO DO
 * CandlestickPlotter — renders a vector of Candlestick objects as an ASCII/ANSI
 * chart in the terminal. Green (█) = close >= open, red (░) = close < open.
 * Wicks drawn as │. Stateless utility, hence static.
 */
class CandlestickPlotter {
public:
    /** Draws the chart. Shows at most maxCandles (the most recent ones). */
    static void plot(const std::vector<Candlestick>& candles,
                     int height = 20,
                     int maxCandles = 18);

private:
    /** This one change price to row number. Low price at bottom, high at top. */
    static int priceToRow(double p, double lo, double hi, int height);

    /** Cut date string so it can fit under candle. */
    static std::string dateLabel(const std::string& time);

    /** Make price look nice, no matter if it big or small. */
    static std::string formatPrice(double v);
};