#pragma once
#include <map>
#include <utility>
#include <string>
#include <vector>
#include "Stock.h"
#include "Trader.h"
#include "Order.h"

struct StockData {
    const char* ticker;
    const char* name;
    long initPrice;
};

extern const std::vector<StockData> STOCK_LIST;

class Market {
public:

    // Load all 100 stocks declared in market.cpp. creates a (Stock, Order) pair for each ticker
    void loadDefaultStocks();

    // Add a (Stock, Order) pair to the market, has unique key by ticker
    void pairStockOrder(const Stock& s, const Order& o);

    // Add a trader to the queue for a stock
    // ticker = stock name, T = the trader, type = "limBid","markBid","limAsk","markAsk"
    void addTrader(const std::string& ticker, Trader T, const std::string& type);

    // Remove a trader from the queue for a stock (cancel order)
    // ticker = which stock, orderNum = the trader's order ID
    void rmTrader(const std::string& ticker, int orderNum);

    // Get a stock by ticker
    Stock* getStock(const std::string& ticker);

    // Get an order (the queues) by ticker
    Order* getOrder(const std::string& ticker);

    // Print all stock info
    std::string output();

    // Print transaction log from transactions.txt to terminal
    void writeToFile(); 

    // Save all order queues to files (call before page reload)
    void saveQueues();
    
    // Load all order queues from files (call when program starts)
    void loadQueues();

    // total number of trades across all stocks
    int numTrades();

    // average spread across all stocks
    int numSpread();

    // total number of bids across all stocks
    int totalBids();

    // total number of asks across all stocks
    int totalAsks();

    // total number of active orders (bids and asks) across all stocks
    int activeOrders();
  
    // saving orderID
    void saveOrderID();

    // saving orderID
    void loadOrderID();

    // getting next orderid
    int getNextOrderID();

    // Saves all stock state (prices, trade history, etc.) to a file
    void saveStocks();

    // Loads all stock state from a file, restores prices and trade history
    void loadStocks();
    
    // Returns a JavaScript formatted string of stock data for rendering in frontend 
    std::string printStockJS();


private:
    // (Stock, Order) pair, each ticker has its own queues
    std::map<std::string, std::pair<Stock, Order>> stockOrders;
    // increasing counter used to assign unique IDs to each order
    int currentOrderID = 0;
};
