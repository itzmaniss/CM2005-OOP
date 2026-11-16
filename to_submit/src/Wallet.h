#pragma once

#include <string>
#include <map>
#include "OrderBookEntry.h"
#include <iostream>

class Wallet 
{
    public:
        /** Just a constructor. */
        Wallet();
        /** Put some money in. */
        void insertCurrency(std::string type, double amount);
        /** Take some money out. Return false if not enough. */
        bool removeCurrency(std::string type, double amount);
        
        /** Check if got enough money. */
        bool containsCurrency(std::string type, double amount);
        /** Check if can pay for this order. */
        bool canFulfillOrder(OrderBookEntry order);
        /** Update wallet after trade happen. */
        void processSale(OrderBookEntry& sale);


        /** Show wallet contents as string. */
        std::string toString();
        /** Allow printing wallet directly. */
        friend std::ostream& operator<<(std::ostream& os, Wallet& wallet);

        
    private:
        std::map<std::string,double> currencies;

};



	

