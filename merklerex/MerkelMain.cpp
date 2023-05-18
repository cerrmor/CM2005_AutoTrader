#include "MerkelMain.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include "OrderBookEntry.h"
#include "CSVReader.h"


MerkelMain::MerkelMain()
{


}

void MerkelMain::init()
{
    int input;
    currentTime = orderBook.getEarliestTime();

    wallet.insertCurrency("BTC", 20);
    wallet.insertCurrency("DOGE", 1000);
    wallet.insertCurrency("ETH", 1000);
    wallet.insertCurrency("USDT", 1000);
    botWallet.insertCurrency("BTC", 20);
    botWallet.insertCurrency("DOGE", 1000);
    botWallet.insertCurrency("ETH", 1000);
    botWallet.insertCurrency("USDT", 1000);
    
    
    
    do
    {
        printMenu();
        input = getUserOption();
        processUserOption(input);
    } while (input != 8);
}


void MerkelMain::printMenu()
{
    std::cout << std::setfill('=') << std::setw(44) << ' ' << std::endl;
    std::cout << "********" << " MAIN MENU " << "********" << std::endl;
    // 1 print help
    std::cout << "1: Print help " << std::endl;
    // 2 print exchange stats
    std::cout << "2: Print exchange stats" << std::endl;
    // 3 make an offer
    std::cout << "3: Make an ask " << std::endl;
    // 4 make a bid 
    std::cout << "4: Make a bid " << std::endl;
    // 5 print wallet
    std::cout << "5: Print wallet " << std::endl;
    // 6 start the robot
    std::cout << "6: Access auto trading" << std::endl;
    // 7 continue   
    std::cout << "7: Continue " << std::endl;
    // 8 Quit
    std::cout << "8: Exit the program " << std::endl;

    std::cout << std::setw(44) << ' ' << std::endl;

    std::cout << "Current time is: " << currentTime << std::endl;
}

void MerkelMain::printSubMenu()
{
    std::cout << std::setfill('=') << std::setw(44)<< ' ' << std::endl;
    std::cout << "********" << " AUTO TRADER MENU " << "********" << std::endl;
    // 1 print help
    std::cout << "1: Print help " << std::endl;
    // 2 print the auto traders wallet
    std::cout << "2: Print Wallet" << std::endl;
    // 3 Give money to the bot
    std::cout << "3: Give money to the AutoTrader" << std::endl;
    // 4 set a auto trading limit
    std::cout << "4: Transfer Funds to User Wallet" << std::endl;
    // 5 trade for all cycles
    std::cout << "5: Start AutoTrading trading session " << std::endl;
    // 6 exit the submenu
    std::cout << "6: Print AutoTrading transaction log" << std::endl;
    // 7 exit the submenu
    std::cout << "7: Return to the main menu" << std::endl;

    std::cout << std::setw(44) << ' ' << std::endl;

    std::cout << "Current time is: " << currentTime << std::endl;
}

void MerkelMain::printHelp()
{
    std::cout << "Help - your aim is to make money. Analyse the market and make bids and offers. " << std::endl;
}

void MerkelMain::printMarketStats()
{
    for (std::string const& p : orderBook.getKnownProducts())
    {
        std::cout << ' ' << std::endl;
        std::cout << "Product: " << p << std::endl;
        std::vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::ask,
                                                                  p, 
                                                                  currentTime);

        std::vector<OrderBookEntry> entries2 = orderBook.getOrders(OrderBookType::bid,
                                                                   p, 
                                                                   currentTime);

        std::cout << "Asks seen: " << entries.size() << std::setfill(' ') << std::setw(50) << "Bids seen: " << entries2.size() << std::endl;
        std::cout << "Max ask: " << OrderBook::getHighPrice(entries) << std::setw(44) << "Max Bid: " << OrderBook::getHighPrice(entries2) << std::endl;
        std::cout << "Min ask: " << OrderBook::getLowPrice(entries) << std::setw(44) << "Min Bid: " << OrderBook::getLowPrice(entries2) << std::endl;
        std::cout << "Average ask: " << OrderBook::averageOrderPrice(entries) << std::setw(44) << "Average Bid: " << OrderBook::averageOrderPrice(entries2) << std::endl;
        std::cout << "The spread between max & min asks is: " << OrderBook::getPriceSpread(entries) << std::setw(44) << "The spread between max & min Bids is: " << OrderBook::getPriceSpread(entries2) << std::endl;
        std::cout << "The liquidity of the market is: " << OrderBook::getLiquidityOfProduct(entries, entries2) << "%" << std::endl;
    }
    

}

void MerkelMain::enterAsk()
{
    std::cout << "Make an ask - enter the amount: product,price, amount, eg  ETH/BTC,200,0.5" << std::endl;
    std::string input;
    std::getline(std::cin, input);

    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if (tokens.size() != 3)
    {
        std::cout << "MerkelMain::enterAsk Bad input! " << input << std::endl;
    }
    else 
    {
        try 
        {
            OrderBookEntry obe = CSVReader::stringsToOBE(
                tokens[1],
                tokens[2],
                currentTime,
                tokens[0],
                OrderBookType::ask
            );
            obe.username = "simuser";

            if (wallet.canFulfillOrder(obe))
            {
                std::cout << "Wallet looks good. " << std::endl;
                orderBook.insertOrder(obe);
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

void MerkelMain::enterBid()
{
    std::cout << "Make an bid - enter the amount: product,price, amount, eg  ETH/BTC,200,0.5" << std::endl;
    std::string input;
    std::getline(std::cin, input);

    std::vector<std::string> tokens = CSVReader::tokenise(input, ',');
    if (tokens.size() != 3)
    {
        std::cout << "MerkelMain::enterBid Bad input! " << input << std::endl;
    }
    else
    {
        try 
        {
            OrderBookEntry obe = CSVReader::stringsToOBE(
                tokens[1],
                tokens[2],
                currentTime,
                tokens[0],
                OrderBookType::bid
            );
            obe.username = "simuser";

            if (wallet.canFulfillOrder(obe))
            {
                std::cout << "Wallet looks good. " << std::endl;
                orderBook.insertOrder(obe);
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

void MerkelMain::printWallet()
{
    std::cout << "**** User Wallet ****" << std::endl;
    std::cout << wallet.toString() << std::endl;
    std::cout << "**** AutoTrader Wallet ****" << std::endl;
    if (botWallet.toString().empty())
    {
        std::cout << "This wallet is currently empty" << std::endl;
    }
    else
    {
        std::cout << botWallet.toString() << std::endl;
    }
}

void MerkelMain::gotoNextTimeframe()
{
    autotrader.updateTransactionLog("\n""Wallet Bigging of Trading Session:");
    autotrader.updateTransactionLog(botWallet.toString());
    std::string keyWord;
    std::cout << "Going to next time frame. " << std::endl;
    autotrader.updateTransactionLog("Completed Sales:");
    for (std::string& p : orderBook.getKnownProducts())
    {
        std::cout << "matching " << p << std::endl;
        std::vector<OrderBookEntry> sales = orderBook.matchAsksToBids(p, currentTime);
        std::cout << "Sales: " << sales.size() << std::endl;
        for (OrderBookEntry& sale : sales)
        {
            std::cout << "Sale price: " << sale.price << " amount " << sale.amount << std::endl;
            if (sale.username == "simuser")
            {
                //update the wallet
                wallet.processSale(sale);
            }
            else if (sale.username == "botuser")
            {
                keyWord = "sale";
                //update the wallet
                botWallet.processSale(sale);
                autotrader.updateTransactionLog(autotrader.createLogEntry(sale, keyWord));
            }
        }
    }
    autotrader.updateTransactionLog("\n""Wallet Trading Session Completed:");
    autotrader.updateTransactionLog(botWallet.toString());
    currentTime = orderBook.getNextTime(currentTime);
    counter += 1;
}

void MerkelMain::startAutoTrading()
{
    int input;
    printSubMenu();
    input = getUserOption();
    processUserOptionSubMenu(input);
}

int MerkelMain::getUserOption()
{
    int userOption = 0;
    std::string line;
    std::getline(std::cin, line);
    try {
        userOption = std::stoi(line);
    }
    catch (const std::exception& e)
    {
        // 
    }
    std::cout << "You chose: " << userOption << std::endl;
    return userOption;
}

void MerkelMain::processUserOption(int userOption)
{
    switch (userOption)
    {
        case 1:
            printHelp();
            break;
        case 2:
            printMarketStats();
            break;
        case 3:
            enterAsk();
            break;
        case 4:
            enterBid();
            break;
        case 5:
            printWallet();
            break;
        case 6:
            startAutoTrading();
            break;
        case 7:
            gotoNextTimeframe();
            break;
        case 8:
            break;
        default:
            std::cout << "Invalid choice. Choose 1-8" << std::endl;
            break;
    }
}

void MerkelMain::processUserOptionSubMenu(int userOption)
{
    switch (userOption)
    {
        case 1:
            autotrader.printHelp();
            startAutoTrading();
            break;
        case 2:
            printWallet();
            startAutoTrading();
            break;
        case 3:
            std::cout << "Please enter the type and amount of currency you would like " << std::endl;
            std::cout << "to give the AutoTrader to work with eg. BTC,200" << std::endl;
            std::getline(std::cin, input);
            wallet.processMoneyTransfer(wallet, botWallet, input);
            startAutoTrading();
            break;
        case 4:
            std::cout << "Please enter the type and amount of currency you would like " << std::endl;
            std::cout << "to your wallet eg. BTC,200" << std::endl;
            std::getline(std::cin, input);
            botWallet.processMoneyTransfer(botWallet, wallet, input);
            startAutoTrading();
            break;
        case 5:
            std::cout << "This will begin the auto trading process" << std::endl;
            autotrader.updateTransactionLog("--------------------------");
            autotrader.updateTransactionLog("*** BIGGING OF SESSION ***");
            autotrader.updateTransactionLog("--------------------------");
            runSimulation();
            autotrader.updateTransactionLog("----------------------");
            autotrader.updateTransactionLog("*** END OF SESSION ***");
            autotrader.updateTransactionLog("----------------------");
            startAutoTrading();
            break;
        case 6:
            autotrader.printTransactionLog();
            startAutoTrading();
            break;
        case 7:
            break;
        default:
            std::cout << "Invalid choice. Choose 1-7" << std::endl;
            startAutoTrading();
            break;
    }
}

void MerkelMain::runSimulation()
{
    int i = 0;
    int input;
    std::string keyWord;
    std::cout << "Please enter the length of simulation you would like to run" << std::endl;
    input = getUserOption();
    while(i < input)
    {
        autotrader.updateTransactionLog("Bids/Asks Made:");
        autotrader.createOrderToPlace(orderBook, botWallet, currentTime, keyWord);        
        gotoNextTimeframe();
        ++i;
    }
}
