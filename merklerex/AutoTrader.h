#pragma once
#include<iostream>
#include<fstream>
#include "Wallet.h"
#include "OrderBook.h"
#include "OrderBookEntry.h"



class AutoTrader
{
	public:
		AutoTrader();
		/** returns a helpfull dialog explaning the autotrader */
		static void printHelp();
		/** takes in the orderbook class, the wallet class, and the current time. 
		* controls when bids/asks are made for the AutoTrader*/
		void createOrderToPlace(OrderBook& orderBook, Wallet& botWallet, std::string currentTime, std::string keyWord);
		/** takes in an array of orderbook entries and a keyword indicating a bid/ask/sale
		*creates a log entry as a string and returns it*/
		std::string createLogEntry(OrderBookEntry& order, std::string g);
		/** when called prints the array of log entries to the screen */
		void printTransactionLog();
		/** takes in a string, which is then pushed to an array storing log entries
		* also updates the log txt file with the same string.*/
		void updateTransactionLog(std::string log);
		
	private:
		bool shouldMakeAsk(double liquidity, double lowPrice, double movingAverage);
		bool shouldMakeBid(double liquidity, double highPrice, double movingAverage);
		void createBidToPlace(double price, std::string s, std::vector<OrderBookEntry>& orders, OrderBook& orderBook, Wallet& botWallet, std::string currentTime, std::string keyWord);
		void createAskToPlace(double price, std::string s, std::vector<OrderBookEntry>& orders, OrderBook& orderBook, Wallet& botWallet, std::string currentTime, std::string keyWord);
		void writeToLogFile(std::string& text);
		std::vector<std::string> log, tokens;
		double liquidity;
		unsigned int counter;
};

