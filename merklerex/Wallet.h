#pragma once
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include "OrderBookEntry.h"

class Wallet
{
	public:
		Wallet();
		/**insert currency into the wallet */
		void insertCurrency(std::string type, double amount);

		/**remove money from the wallet */
		bool removeCurrency(std::string type, double amount);
		
		/** check if the wallet contains this much currency or more */
		bool containsCurrency(std::string type, double amount);

		/** update the contents of the wallet 
		* assumes the order was made by the owner of the wallet*/
		void processSale(OrderBookEntry& sale);

		/** Removes the specified amount from the first wallet and adds that amount to the second
		* assumes the request was made by the owner of the wallet*/
		void processMoneyTransfer(Wallet& wallet1, Wallet& wallet2, std::string& input);

		
		/**checks if the wallet can cope with the ask or bid process*/
		bool canFulfillOrder(OrderBookEntry order);


		/**generate a string representation of the wallet */
		std::string toString();

	private:
		std::map<std::string, double> currencies;
		std::vector<std::string> tokens;
		double amount;
};

