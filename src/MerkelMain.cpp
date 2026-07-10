#include "MerkelMain.h"
#include <iostream>
#include <iomanip>
#include "CSVReader.h"

MerkelMain::MerkelMain()
{
}

void MerkelMain::init()
{
    currentTime = orderBook.getEarliestTime();

    bool running = true;
    while (running)
    {
        if (currentUser.empty())                  // ---- logged out ----
        {
            printLoginMenu();
            int option = getUserOption(5);
            if (option == -1) break;              // EOF / stream closed
            processLoginOption(option, running);
        }
        else                                      // ---- logged in ----
        {
            printMainMenu();
            int option = getUserOption(10);
            if (option == -1) break;
            processMainOption(option);
        }
    }
    std::cout << "Goodbye." << std::endl;
}

// ======================= menus =======================

void MerkelMain::printLoginMenu()
{
    std::cout << "========== Welcome to MerkelRex ==========" << std::endl;
    std::cout << "1: Register a new account" << std::endl;
    std::cout << "2: Log in" << std::endl;
    std::cout << "3: Reset password" << std::endl;
    std::cout << "4: Help" << std::endl;
    std::cout << "5: Exit" << std::endl;
    std::cout << "==========================================" << std::endl;
}

void MerkelMain::printMainMenu()
{
    std::cout << "============== MerkelRex ==============" << std::endl;
    std::cout << "1: Print candlestick (OHLC) data" << std::endl;
    std::cout << "2: Plot candlestick chart" << std::endl;
    std::cout << "3: Deposit funds" << std::endl;
    std::cout << "4: Withdraw funds" << std::endl;
    std::cout << "5: Recent transactions (last 5)" << std::endl;
    std::cout << "6: Activity statistics" << std::endl;
    std::cout << "7: Simulate trading (50 orders)" << std::endl;
    std::cout << "8: Go to next timeframe (match)" << std::endl;
    std::cout << "9: Help" << std::endl;
    std::cout << "10: Log out" << std::endl;
    std::cout << "======================================" << std::endl;
    std::cout << "Logged in as: " << currentUser
              << " | Balance: " << std::fixed << std::setprecision(2)
              << userWallet.getBalance(currentUser) << std::endl;
    std::cout << "Current time: " << currentTime << std::endl;
}

// Validated menu input (Task 5): getline + stoi in try/catch + range check,
// loops until a valid option is entered - never crashes on bad input.
int MerkelMain::getUserOption(int maxOption)
{
    while (true)
    {
        std::cout << "Type an option (1-" << maxOption << "): ";
        std::string line;
        if (!std::getline(std::cin, line)) return -1;    // stream closed
        try
        {
            int option = std::stoi(line);
            if (option >= 1 && option <= maxOption) return option;
        }
        catch (const std::exception& e) { /* fall through to retry */ }
        std::cout << "Invalid choice, please try again." << std::endl;
    }
}

void MerkelMain::processLoginOption(int option, bool& running)
{
    if (option == 1) doRegister();
    else if (option == 2) doLogin();
    else if (option == 3) doResetPassword();
    else if (option == 4) printLoginHelp();
    else if (option == 5) running = false;
}

void MerkelMain::processMainOption(int option)
{
    if (option == 1) printCandlestickData();
    else if (option == 2) doPlotChart();
    else if (option == 3) userWallet.depositFlow(currentUser);
    else if (option == 4) userWallet.withdrawFlow(currentUser);
    else if (option == 5) txLog.printRecent(currentUser);
    else if (option == 6) txLog.printStats(currentUser);
    else if (option == 7) doSimulation();
    else if (option == 8) gotoNextTimeframe();
    else if (option == 9) printMainHelp();
    else if (option == 10) { std::cout << "Logging out " << currentUser << std::endl; currentUser = ""; }
}

// ======================= help =======================

void MerkelMain::printLoginHelp()
{
    std::cout << "\n--- Help ---" << std::endl;
    std::cout << "Register: create an account; you'll be given a 10-digit username." << std::endl;
    std::cout << "Log in:   enter that username and your password to start trading." << std::endl;
    std::cout << "Reset:    change your password by confirming your username + email." << std::endl;
    std::cout << "Exit:     close the program.\n" << std::endl;
}

void MerkelMain::printMainHelp()
{
    std::cout << "\n--- Help ---" << std::endl;
    std::cout << "1/2: view market history as an OHLC table or a candlestick chart." << std::endl;
    std::cout << "3/4: deposit or withdraw funds from your wallet." << std::endl;
    std::cout << "5/6: review your recent trades and activity statistics." << std::endl;
    std::cout << "7:   generate 50 simulated orders priced from the historical data." << std::endl;
    std::cout << "8:   advance the clock and match orders in the current timeframe.\n" << std::endl;
}

// ======================= Task 2 handlers =======================

void MerkelMain::doRegister()  { userManager.registerUser(); }

void MerkelMain::doLogin()
{
    std::string user;
    if (userManager.login(user)) currentUser = user;   // session starts
}

void MerkelMain::doResetPassword() { userManager.resetPassword(); }

// ======================= Task 1 =======================

// Shared prompt used by both the OHLC table and the chart, so the
// product/order-type/time-group questions live in one place.
std::vector<Candlestick> MerkelMain::getCandlesFromUser(bool askType)
{
    std::string product;
    std::cout << "Enter product (e.g. ETH/BTC): ";
    std::getline(std::cin, product);

    OrderBookType orderType = OrderBookType::unknown;
    if (askType) {
        std::string orderTypeInput;
        std::cout << "Enter order type (ask/bid): ";
        std::getline(std::cin, orderTypeInput);
        orderType = OrderBookEntry::stringToOrderBookType(orderTypeInput);
    }

    std::string timeGroupInput;
    std::cout << "Enter time group (1 = Daily, 2 = Monthly, 3 = Yearly): ";
    std::getline(std::cin, timeGroupInput);
    TimeGroup timeGroup = TimeGroup::Daily;
    try {
        switch (std::stoi(timeGroupInput)) {
            case 2: timeGroup = TimeGroup::Monthly; break;
            case 3: timeGroup = TimeGroup::Yearly;  break;
            default: timeGroup = TimeGroup::Daily;  break;
        }
    } catch (const std::exception& e) { timeGroup = TimeGroup::Daily; }

    return CandlestickCalculator::calculate(
        orderBook.getOrderBook(), product, orderType, timeGroup, !askType);
}

void MerkelMain::printCandlestickData()
{
    std::vector<Candlestick> candles = getCandlesFromUser();

    std::string startDate, endDate;
    std::cout << "Enter start date (e.g. 2020-03-17, leave blank to skip): ";
    std::getline(std::cin, startDate);
    std::cout << "Enter end date (e.g. 2020-03-17, leave blank to skip): ";
    std::getline(std::cin, endDate);

    std::cout << std::left
              << std::setw(20) << "Date" << std::setw(15) << "Open"
              << std::setw(15) << "High" << std::setw(15) << "Low"
              << std::setw(15) << "Close" << std::endl;
    std::cout << std::string(80, '-') << std::endl;

    for (const Candlestick& candle : candles) {
        if (!startDate.empty() && candle.getTime() < startDate) continue;
        if (!endDate.empty()   && candle.getTime() > endDate)   continue;
        std::cout << std::left
                  << std::setw(20) << candle.getTime()
                  << std::setw(15) << candle.getOpen()
                  << std::setw(15) << candle.getHigh()
                  << std::setw(15) << candle.getLow()
                  << std::setw(15) << candle.getClose()
                  << std::endl;
    }
}

void MerkelMain::doPlotChart()
{
    std::vector<Candlestick> candles = getCandlesFromUser(false);
    CandlestickPlotter::plot(candles);
}

// ======================= trading handlers =======================

void MerkelMain::doSimulation()
{
    std::string timestamp = simulator.run(currentUser);
    currentTime = timestamp;   // advance so 'next timeframe' lands on new orders
}

void MerkelMain::gotoNextTimeframe()
{
    std::cout << "Going to next time frame... " << std::endl;
    for (const std::string& p : orderBook.getKnownProducts())
    {
        std::cout << "matching " << p << std::endl;
        std::vector<OrderBookEntry> entries = orderBook.matchAsksToBids(p, currentTime);
        std::cout << "got " << entries.size() << " matches" << std::endl;
        for (OrderBookEntry& e : entries)
        {
            std::cout << "Match: " << e.price << " amount " << e.amount << std::endl;
            if (e.username == currentUser)
            {
                std::string type = (e.orderType == OrderBookType::asksale) ? "ask" : "bid";
                txLog.log({ e.timestamp, e.product, type, e.price, e.amount, e.username });
            }
        }
    }
    currentTime = orderBook.getNextTime(currentTime);
}