#include "Wallet.h"
#include "CSVReader.h"

Wallet::Wallet()
{
	
}

void Wallet::insertCurrency(std::string type, double amount)
{
	double balance;
	if (amount < 0)
	{
		throw std::exception{};
	}
	if (currencies.count(type) == 0) 
	{
		balance = 0;
	}
	else 
	{
		balance = currencies[type];
	}
	balance += amount;
	currencies[type] = balance;
}

bool Wallet::removeCurrency(std::string type, double amount)
{
	double balance;
	if (amount < 0)
	{
		return false;
	}
	if (currencies.count(type) == 0) 
	{
		return false;
	}
	else
	{
		if (containsCurrency(type, amount))
		{
			std::cout << "Removing " << type << " : " << amount << " from user wallet" << std::endl;
			currencies[type] -= amount;
			return true;
		}
		else return false; 
	}
}

bool Wallet::containsCurrency(std::string type, double amount)
{
	if (currencies.count(type) == 0) return false; 
	else return currencies[type] >= amount;
}

std::string Wallet::toString()
{
	std::string s;
	for (std::pair <std::string, double> pair : currencies)
	{
		std::string currency = pair.first;
		double amount = pair.second;
		s += currency + " : " + std::to_string(amount) + "\n";
	}
	return s;
}

bool Wallet::canFulfillOrder(OrderBookEntry order)
{
	std::vector <std::string> currs = CSVReader::tokenise(order.product, '/');
	//ask
	if (order.orderType == OrderBookType::ask)
	{
		double amount = order.amount;
		std::string currency = currs[0];
		std::cout << "Wallet::canFulfillOrder " << currency << " : " << amount << std::endl;
		return containsCurrency(currency, amount);
	}
	
	//bid
	if (order.orderType == OrderBookType::bid)
	{
		double amount = order.amount * order.price;
		std::string currency = currs[1];
		std::cout << "Wallet::canFulfillOrder " << currency << " : " << amount << std::endl;
		return containsCurrency(currency, amount);
	}
	
		
	return false;
}

void Wallet::processSale(OrderBookEntry& sale)
{
	std::vector <std::string> currs = CSVReader::tokenise(sale.product, '/');
	//ask
	if (sale.orderType == OrderBookType::asksale)
	{
		double outgoingAmount = sale.amount;
		std::string outgoingCurrency = currs[0];
		double incomingAmount = sale.amount * sale.price;
		std::string incomingCurrency = currs[1];

		currencies[incomingCurrency] += incomingAmount;
		currencies[outgoingCurrency] -= outgoingAmount;
	}

	//bid
	if (sale.orderType == OrderBookType::bidsale)
	{
		double incomingAmount = sale.amount;
		std::string incomingCurrency = currs[0];
		double outgoingAmount = sale.amount * sale.price;
		std::string outgoingCurrency = currs[1];

		currencies[incomingCurrency] += incomingAmount;
		currencies[outgoingCurrency] -= outgoingAmount;
	}
}

void Wallet::processMoneyTransfer(Wallet& wallet1, Wallet& wallet2, std::string& input)
{
	tokens = CSVReader::tokenise(input, ',');
	if (tokens.size() != 2)
	{
		std::cout << "MerkelMain::processMoneyTransfer Bad input! " << std::endl;
	}
	else
	{

		try
		{
			Wallet::amount = stod(tokens[1]);
			if (amount > 0 && wallet1.containsCurrency(tokens[0], amount))
			{
				std::cout << "wallet looks good" << std::endl;
				wallet1.removeCurrency(tokens[0], amount);
				wallet2.insertCurrency(tokens[0], amount);
				std::cout << "Transfered: " << tokens[0] << ' ' << amount << " from user wallet to AutoTraders wallet" << std::endl;
			}
			else
			{
				std::cout << "please try again wallet has insufficient funds to transfer" << std::endl;
			}
		}
		catch (const std::exception& e)
		{
			std::cout << " Wallet::processMoneyTransfer bad input " << std::endl;
		}


	}
}


