cmake_minimum_required(VERSION 4.2)
project(merklerex)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)

configure_file(${CMAKE_SOURCE_DIR}/src/20200317.csv ${CMAKE_BINARY_DIR}/20200317.csv COPYONLY)
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR}/data)

add_executable(merklerex
        src/CSVReader.cpp
        src/CSVReader.h
        src/main.cpp
        src/MerkelMain.cpp
        src/MerkelMain.h
        src/OrderBook.cpp
        src/OrderBook.h
        src/OrderBookEntry.cpp
        src/OrderBookEntry.h
        src/Wallet.cpp
        src/Wallet.h
        src/Candlestick.cpp
        src/Candlestick.h
        src/CandlestickCalculator.cpp
        src/CandlestickCalculator.h
        src/UserManager.cpp
        src/UserManager.h
        src/UserWallet.cpp
        src/UserWallet.h
        src/TransactionLog.cpp
        src/TransactionLog.h
        src/Simulator.cpp
        src/Simulator.h
        src/Candlestickplotter.cpp
        src/Candlestickplotter.h)

add_executable(batch_sim
        src/CSVReader.cpp
        src/batch_sim.cpp
        src/OrderBook.cpp
        src/OrderBookEntry.cpp
        src/Wallet.cpp
        src/Candlestick.cpp
        src/CandlestickCalculator.cpp
        src/UserManager.cpp
        src/UserWallet.cpp
        src/TransactionLog.cpp
        src/Simulator.cpp
        src/Candlestickplotter.cpp)
