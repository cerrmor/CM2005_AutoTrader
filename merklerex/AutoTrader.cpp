#include "AutoTrader.h"


AutoTrader::AutoTrader()
{
	
}

bool AutoTrader::shouldMakeAsk(double liquidity, double lowPrice, double movingAverage)
{
	if (liquidity > 1 && lowPrice < movingAverage)return false;
	else return true;
}

bool AutoTrader::shouldMakeBid(double liquidity, double highPrice, double movingAverage)
{
	if (liquidity > 1 && highPrice > movingAverage)return false;
	else return true;
}

void AutoTrader::createBidToPlace(double price, std::string product, std::vector<OrderBookEntry>& orders, OrderBook& orderBook, Wallet& botWallet, std::string currentTime, std::string keyWord)
{
    
    std::string a = product;
    std::string b =  std::to_string(price);
    std::string c = "10";
    std::string s = (a + "," + b + "," + c);

    std::vector<std::string> tokens = CSVReader::tokenise(s, ',');
    if (tokens.size() != 3)
    {
        std::cout << "MerkelMain::enterBid Bad input! " << s << std::endl;
    }
    else {
        try {
            OrderBookEntry obe = CSVReader::stringsToOBE(
                tokens[1],
                tokens[2],
                currentTime,
                tokens[0],
                OrderBookType::bid
            );
            obe.username = "botuser";

            if (botWallet.canFulfillOrder(obe))
            {
                std::cout << "Wallet looks good. " << std::endl;
                keyWord = "bid";
                orderBook.insertOrder(obe);
                updateTransactionLog(createLogEntry(obe, keyWord));

            }
            else
            {
                std::cout << "Wallet has insufficient funds.  " << std::endl;
            }

        }
        catch (const std::exception& e)
        {
            std::cout << " MerkelMain::enterBid Bad input " << std::endl;
        }
    }
}

void AutoTrader::createAskToPlace(double price, std::string product, std::vector<OrderBookEntry>& orders, OrderBook& orderBook, Wallet& botWallet, std::string currentTime, std::string keyWord)
{
    
    std::string a = product;
    std::string b =  std::to_string(price);
    std::string c = "10";
    std::string s = (a+","+b+","+c);

    std::vector<std::string> tokens2 = CSVReader::tokenise(s, ',');
    if (tokens2.size() != 3)
    {
        std::cout << "MerkelMain::enterAsk Bad input! " << s << std::endl;
    }
    else {
        try {
            OrderBookEntry obe = CSVReader::stringsToOBE(
                tokens2[1],
                tokens2[2],
                currentTime,
                tokens2[0],
                OrderBookType::ask
            );
            obe.username = "botuser";

            if (botWallet.canFulfillOrder(obe))
            {
                keyWord = "ask";
                std::cout << "Wallet looks good. " << std::endl;
                orderBook.insertOrder(obe);
                updateTransactionLog(createLogEntry(obe, keyWord));
            }
            else
            {
                std::cout << "Wallet has insufficient funds.  " << std::endl;
            }

        }
        catch (const std::exception& e)
        {
            std::cout << " MerkelMain::enterAsk Bad input " << std::endl;
        }
    }
}
void AutoTrader::createOrderToPlace(OrderBook& orderBook, Wallet& botWallet, std::string currentTime, std::string keyWord)
{
    counter = 1;
    double price = 0.0;
	for (std::string const& p : orderBook.getKnownProducts())
	{
		std::cout << ' ' << std::endl;
		std::cout << "Product: " << p << std::endl;
		std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::ask,
			p, currentTime);
		std::vector<OrderBookEntry> entries2 = orderBook.getOrders(OrderBookType::bid,
			p, currentTime);
        if (shouldMakeBid(orderBook.getLiquidityOfProduct(entries,entries2),orderBook.getHighPrice(entries2),orderBook.simpleMovingAverage(p,counter,OrderBookType::bid)))
        {
            if (orderBook.getHighPrice(entries) <= orderBook.simpleMovingAverage(p, counter, OrderBookType::ask)) price = orderBook.simpleMovingAverage(p, counter, OrderBookType::bid);
            else price = orderBook.getHighPrice(entries);
            createBidToPlace(price, p, entries, orderBook, botWallet, currentTime, keyWord);
        }
        if (shouldMakeAsk(orderBook.getLiquidityOfProduct(entries, entries2), orderBook.getLowPrice(entries), orderBook.simpleMovingAverage(p, counter, OrderBookType::ask)))
        {
            if (orderBook.getLowPrice(entries2) >= orderBook.simpleMovingAverage(p, counter, OrderBookType::bid)) price = orderBook.simpleMovingAverage(p, counter, OrderBookType::ask);
            else  price = orderBook.getLowPrice(entries2);
            createAskToPlace(price, p, entries2, orderBook, botWallet, currentTime, keyWord);
        }

        
	}
    counter++;
}

void AutoTrader::printTransactionLog()
{
	for (unsigned int i = 0; i < log.size(); ++i)
	{
		std::cout << log[i] << std::endl;
	}
}

void AutoTrader::updateTransactionLog(std::string log)
{
	AutoTrader::log.push_back(log);
	writeToLogFile(log);
}

void AutoTrader::writeToLogFile(std::string& text)
{
	std::ofstream logFile("logFile.txt", std::ios_base::out | std::ios_base::app);
	logFile << text << std::endl;
}

std::string AutoTrader::createLogEntry(OrderBookEntry& order, std::string g)
{
	if (g == "ask")
	{
		std::string a = std::to_string(order.price);
		std::string b = std::to_string(order.amount);
		std::string c = order.timestamp;
		std::string d = order.product;
		std::string e = "ask";
		std::string f = order.username;
		std::string s = "Timestamp: " + c + " ->" + " " + f + " " + e + " " + d + " " + "amount: " + b + " price: " + a;
		return s;
	}
	else if (g == "bid")
	{
		std::string a = std::to_string(order.price);
		std::string b = std::to_string(order.amount);
		std::string c = order.timestamp;
		std::string d = order.product;
		std::string e = "bid";
		std::string f = order.username;
		std::string s = "Timestamp: " + c + " ->" + " " + f + " " + e + " " + d + " " + "amount: " + b + " price: " + a;
		return s;
	}
	else
	{
		std::string a = std::to_string(order.price);
		std::string b = std::to_string(order.amount);
		std::string c = order.timestamp;
		std::string d = order.product;
		std::string e = "sale";
		std::string f = order.username;
		std::string s = "Timestamp: " + c + " ->" + " " + f + " " + e + " " + d + " " + "amount: " + b + " price: " + a;
		return s;
	}
}

void AutoTrader::printHelp()
{
	std::cout << "Life is busy let this auto trader make money for you!" << std::endl;
	std::cout << "Transfer the AutoTrader some currency to work with, " << std::endl;
	std::cout << "and choose a length of simulation for it to run for." << std::endl;
}


