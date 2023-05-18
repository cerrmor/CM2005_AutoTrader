#pragma once
#include "OrderBookEntry.h"
#include "CSVReader.h"
#include <string>
#include <vector>

class OrderBook
{
public:
    /** construct, reading a csv data file */
    OrderBook(std::string filename);
    /** return vector of all know products in the dataset*/
    std::vector<std::string> getKnownProducts();
    /** return vector of Orders according to the sent filters*/
    std::vector<OrderBookEntry> getOrders(OrderBookType type,
        std::string product,
        std::string timestamp);

    /** returns the earliest time in the orderbook*/
    std::string getEarliestTime();
    /** returns the next time after the
     * sent time in the orderbook
     * If there is no next timestamp, wraps around to the start
     * */
    std::string getNextTime(std::string timestamp);
    /** inserts an entry of the type orderBookEntry into the order book entry array*/
    void insertOrder(OrderBookEntry& order);
    /** cycles through the orderBookEntry vector matching ask to bids based off product and timestamp.
    * returns the sorted vector of orderbook entrys  */
    std::vector<OrderBookEntry> matchAsksToBids(std::string product, std::string timestamp);
    /** takes in the orderBookEntry vector. Cycles through the vector
    * looking for the highest price. returns the highest price */
    static double getHighPrice(std::vector<OrderBookEntry>& orders);
    /** takes in the orderBookEntry vector. Cycles through the vector
    * looking for the lowest price. returns the lowest price */
    static double getLowPrice(std::vector<OrderBookEntry>& orders);
    /** takes in the orderBookEntry vector cycles through the vector
    *summing the prices and devides the total by the total number of prices
    *returns the average price*/
    static double averageOrderPrice(std::vector<OrderBookEntry>& orders);
    /** takes in the orderBookEntry vector finds the highest and lowest price
    * and subtracts them from each other. Finally divides this number by the lowest price to
    * return the distance between the highest and lowest price of the placed orders*/
    static double getPriceSpread(std::vector<OrderBookEntry>& orders);
    /** takes in the orderBookEntry vector for the bids and the vector for the asks
    * calculates the spread between the highest bid and lowest ask multiplied by 100 to form a percent
    *returns this precentage for determining the liquidity of the market.
    *percentages lower then one percent means a high turnover rate or more likely to make a trade*/
    static double getLiquidityOfProduct(std::vector<OrderBookEntry>& orders, std::vector<OrderBookEntry>& orders2);
    /** takes in a string represent a product, an unsigned interger to be used as a counter and 
    *weather it is a bid or an ask. it then cycles through the orderBookEntry vector for that product
    *from the first timeStamp to the current timeStamp summing the price and deviding that by the 
    *number of cycles. this calculates the moving average for that product and returns that average.*/
    double simpleMovingAverage(std::string p, unsigned int c, OrderBookType type);
    
private:
    std::vector<OrderBookEntry> orders, orders2;

    
    
    
};
