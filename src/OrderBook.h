#pragma once
#include "OrderBookEntry.h"
#include "CSVReader.h"
#include <string>
#include <vector>

class OrderBook
{
    public:
    /** Load all the order data from CSV file. */
        OrderBook(std::string filename);
    /** Tell me what coins/products we have. */
        std::vector<std::string> getKnownProducts();
    /** Find orders that match what you looking for. */
        std::vector<OrderBookEntry> getOrders(OrderBookType type,
                                              std::string product, 
                                              std::string timestamp);

        /** Give the first timestamp in the book. */
        std::string getEarliestTime();
        /** Find what time come after this one. If end, go back to start. */
        std::string getNextTime(std::string timestamp);

        /** Put new order into the book. */
        void insertOrder(OrderBookEntry& order);

        /** Match buy and sell orders. */
        std::vector<OrderBookEntry> matchAsksToBids(std::string product, std::string timestamp);

        /** Find highest price in this list of orders. */
        static double getHighPrice(std::vector<OrderBookEntry>& orders);
        /** Find lowest price in this list of orders. */
        static double getLowPrice(std::vector<OrderBookEntry>& orders);

        /** Get the whole list of orders. */
        const std::vector<OrderBookEntry>& getOrderBook() const;

        //my code ends here

    private:
        std::vector<OrderBookEntry> orders;


};
