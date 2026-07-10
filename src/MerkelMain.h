#pragma once

#include <vector>
#include <string>
#include "OrderBookEntry.h"
#include "OrderBook.h"
#include "Candlestick.h"
#include "CandlestickCalculator.h"
#include "Candlestickplotter.h"
#include "UserWallet.h"
#include "UserManager.h"
#include "TransactionLog.h"
#include "Simulator.h"

class MerkelMain
{
public:
    MerkelMain();
    /** Start everything. */
    void init();
private:
    // ---- menus ----
    /** Show menu for login. */
    void printLoginMenu();                       // shown when logged out
    /** Show main menu for logged in user. */
    void printMainMenu();                        // shown when logged in
    /** Wait and get what user type. Make sure it correct. */
    int  getUserOption(int maxOption);           // validated 1..maxOption, loops on bad input
    /** Decide what to do with login menu choice. */
    void processLoginOption(int option, bool& running);
    /** Decide what to do with main menu choice. */
    void processMainOption(int option);

    // ---- help (context specific) ----
    /** Explain how login work. */
    void printLoginHelp();
    /** Explain how main menu work. */
    void printMainHelp();

    // ---- Task 2 auth handlers ----
    /** Create new account. */
    void doRegister();
    /** Log into existing account. */
    void doLogin();
    /** Change password if forgot. */
    void doResetPassword();

    // ---- Task 1 ----
    /** Ask user which product and time they want to see. */
    std::vector<Candlestick> getCandlesFromUser(bool askType = true);  // shared prompt (product/type/group)
    /** Show candle table on screen. */
    void printCandlestickData();                    // OHLC table
    /** Draw the chart in terminal. */
    void doPlotChart();                             // ASCII/ANSI chart

    // ---- trading handlers ----
    /** Run the auto trader. */
    void doSimulation();                         // Task 4
    /** Jump to next time period. */
    void gotoNextTimeframe();                    // match demo

    // ---- state ----
    std::string currentTime;
    std::string currentUser;                     // empty = logged out

    // ---- members (order matters: userWallet before its referrers) ----
    OrderBook      orderBook{"./20200317.csv"};
    UserWallet     userWallet{"data/USERS_WALLET.CSV"};
    UserManager    userManager{userWallet};
    TransactionLog txLog{"data/USERS_TRADING.CSV"};
    Simulator      simulator{orderBook, userWallet, txLog};
};