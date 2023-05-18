#pragma once

#include <vector>
#include "OrderBookEntry.h"
#include "OrderBook.h"
#include "Wallet.h"
#include "AutoTrader.h"

class MerkelMain
{
public:
    MerkelMain();
    /** Call this to start the sim */
    void init();

private:
    void printMenu();
    void printSubMenu();
    void printHelp();
    void printMarketStats();
    void enterAsk();
    void enterBid();
    void printWallet();
    void gotoNextTimeframe();
    void startAutoTrading();
    int getUserOption();
    void processUserOption(int userOption);
    void processUserOptionSubMenu(int userOption);
    void runSimulation();
    
    unsigned int counter = 1;
    std::string currentTime;
    std::string input;
    std::vector<std::string> tokens, tokens2;
    OrderBook orderBook{ "20200317.csv" };
    Wallet wallet;
    Wallet botWallet;
    AutoTrader autotrader;

};
