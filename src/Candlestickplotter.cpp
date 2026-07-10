#include "Candlestickplotter.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <algorithm>

/**
 *THIS COMMENT WAS AI GENERATED FOR ME TO HAVE A CLEARLY STATED IDEA ON WHAT I WANTED TO DO
 * Grid approach: we build a 2D char grid (rows x columns) using single-byte
 * MARKER characters, then translate markers to coloured glyphs at print time.
 * (We can't store multi-byte glyphs like █ directly in the char grid without
 * breaking column alignment.)
 *
 * Markers:  'U' = up-candle body   -> green █
 *           'D' = down-candle body -> red   █
 *           'W' = wick             -> │ (coloured like its candle)
 *           ' ' = empty
 *
 * ANSI colours work in the CLion console and most modern terminals.
 * If output ever looks garbled (old Windows cmd.exe), set USE_COLOUR = false.
 */

namespace {
    const int  COL_WIDTH  = 6;      // columns per candle (3-wide body + gap)
    const bool USE_COLOUR = true;

    const std::string GREEN = "\033[32m";
    const std::string RED   = "\033[31m";
    const std::string RESET = "\033[0m";
}

/**
 * Work out which row the price belongs to.
 */
int CandlestickPlotter::priceToRow(double p, double lo, double hi, int height)
{
    double norm = (p - lo) / (hi - lo);              // 0.0 .. 1.0
    int row = static_cast<int>(std::lround(norm * (height - 1)));
    if (row < 0) row = 0;                            // clamp against rounding drift
    if (row > height - 1) row = height - 1;
    return row;
}

/**
 * Make date string shorter so it fit.
 */
std::string CandlestickPlotter::dateLabel(const std::string& time)
{
    if (time.length() >= 10) return time.substr(5, 5);   // daily:   MM-DD
    if (time.length() == 7)  return time.substr(2);      // monthly: YY-MM
    return time;                                          // yearly:  YYYY
}

/**
 * Format price properly so easier to read.
 */
std::string CandlestickPlotter::formatPrice(double v)
{
    std::ostringstream ss;
    if (v >= 1000.0)       ss << std::fixed << std::setprecision(1) << v;
    else if (v >= 1.0)     ss << std::fixed << std::setprecision(2) << v;
    else if (v >= 0.001)   ss << std::fixed << std::setprecision(5) << v;
    else                   ss << std::scientific << std::setprecision(2) << v;
    return ss.str();
}

/**
 * Print the graph to terminal. Use colours if can.
 */
void CandlestickPlotter::plot(const std::vector<Candlestick>& candles,
                              int height, int maxCandles)
{
    if (candles.empty()) {
        std::cout << "No candlestick data to plot." << std::endl;
        return;
    }

    //keep only the most recent maxCandles
    std::vector<Candlestick> shown = candles;
    if (static_cast<int>(shown.size()) > maxCandles) {
        shown.assign(candles.end() - maxCandles, candles.end());
        std::cout << "(showing the most recent " << maxCandles
                  << " of " << candles.size() << " candles)" << std::endl;
    }
    const int n = static_cast<int>(shown.size());

    //price range across all shown candles
    double lo = shown[0].getLow();
    double hi = shown[0].getHigh();
    for (const Candlestick& c : shown) {
        lo = std::min(lo, c.getLow());
        hi = std::max(hi, c.getHigh());
    }
    if (hi == lo) hi = lo + 1.0;

    // build marker grid: row 0 = botto
    const int width = n * COL_WIDTH;
    std::vector<std::string> grid(height, std::string(width, ' '));

    for (int i = 0; i < n; ++i) {
        const Candlestick& c = shown[i];
        const int centre = i * COL_WIDTH + COL_WIDTH / 2;
        const bool up    = c.getClose() >= c.getOpen();
        const char body  = up ? 'U' : 'D';

        const int rHigh = priceToRow(c.getHigh(), lo, hi, height);
        const int rLow  = priceToRow(c.getLow(),  lo, hi, height);
        const int rTop  = priceToRow(std::max(c.getOpen(), c.getClose()), lo, hi, height);
        const int rBot  = priceToRow(std::min(c.getOpen(), c.getClose()), lo, hi, height);

        for (int r = rLow; r <= rHigh; ++r)          // wick first...
            grid[r][centre] = 'W';
        for (int r = rBot; r <= rTop; ++r)           // ...body drawn over it
            for (int col = centre - 1; col <= centre + 1; ++col)
                grid[r][col] = body;
    }

    //print top-down with a y-axis label every 4th row
    const int Y_WIDTH = 12;
    for (int r = height - 1; r >= 0; --r) {
        if (r % 4 == 0) {
            double price = lo + (hi - lo) * r / (height - 1);
            std::cout << std::right << std::setw(Y_WIDTH) << formatPrice(price) << " ┤ ";
        } else {
            std::cout << std::string(Y_WIDTH, ' ') << " │ ";
        }

        for (int col = 0; col < width; ++col) {
            const char m = grid[r][col];
            // colour follows the candle this column belongs to
            const int candleIdx = col / COL_WIDTH;
            const bool up = shown[candleIdx].getClose() >= shown[candleIdx].getOpen();
            const std::string& colour = up ? GREEN : RED;

            if (m == 'U')      std::cout << (USE_COLOUR ? colour + "█" + RESET : "█");
            else if (m == 'D') std::cout << (USE_COLOUR ? colour + "█" + RESET : "█");
            else if (m == 'W') std::cout << (USE_COLOUR ? colour + "│" + RESET : "│");
            else               std::cout << ' ';
        }
        std::cout << "\n";
    }

    //x-axis and date labels centred under each candle
    std::cout << std::string(Y_WIDTH, ' ') << " └" << std::string(width, '-') << "\n";
    std::cout << std::string(Y_WIDTH, ' ') << "   ";
    for (int i = 0; i < n; ++i) {
        std::string label = dateLabel(shown[i].getTime());
        if (static_cast<int>(label.size()) > COL_WIDTH)
            label = label.substr(0, COL_WIDTH);
        const int pad = (COL_WIDTH - static_cast<int>(label.size())) / 2;
        std::cout << std::string(pad, ' ') << label
                  << std::string(COL_WIDTH - pad - label.size(), ' ');
    }
    std::cout << std::endl;
}