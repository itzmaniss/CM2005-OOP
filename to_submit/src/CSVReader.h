#pragma once

#include "OrderBookEntry.h"
#include <vector>
#include <string>


class CSVReader
{
    public:
     /** Just a constructor. */
     CSVReader();

     /** Read CSV file and turn into list of orders. */
     static std::vector<OrderBookEntry> readCSV(std::string csvFile);
     /** Split string into pieces based on separator. */
     static std::vector<std::string> tokenise(std::string csvLine, char separator);
    
     /** Convert simple strings to OrderBookEntry object. */
     static OrderBookEntry stringsToOBE(std::string price, 
                                        std::string amount, 
                                        std::string timestamp, 
                                        std::string product, 
                                        OrderBookType OrderBookType);

    private:
     /** Convert list of strings to OrderBookEntry object. */
     static OrderBookEntry stringsToOBE(std::vector<std::string> strings);
     
};