#pragma once
#include <queue>
#include <string>
#include "Trader.h"
#include "Stock.h"

class Order{
    public:
        // ── Constructors ──
        Order() = default;       // needed so map can create empty pairs
        Order(Stock S);

        // Add a market buy order — executes immediately at best ask price
        void addMarkBid(Trader& T);

        // Add a limit buy order — sits in queue until price is matched
        void addLimBid(Trader& T);

        // Add a limit sell order — sits in queue until price is matched
        void addLimAsk(Trader& T);

        // Add a market sell order — executes immediately at best bid price
        void addMarkAsk(Trader& T);

        // Cancel an order by order ID — searches both queues
        bool cancelOrder(int orderID);

        // Set the best bid price for a stock (top of limitBids queue)
        void bestBid();

        // Set the best ask price for a stock (top of limitAsks queue)
        void bestAsk();

        // Set the market price for a stock (last trade price)
        void marketPrice(Trader& T);

        // Calculate and set the spread (bestAsk - bestBid)
        void spread();

        // Increment total asks counter in Stock
        void setTotAsks();

        // Increment total bids counter in Stock
        void setTotBids();

        // Increment total transactions counter in Stock
        void setTotTransact();

        // Get number of active asks in the queue
        int numAsks() const;

        // Get number of active bids in the queue
        int numBids() const;

        // Get the stock this order book belongs to
        Stock getStock() const;

        // Set the stock for this order book
        void setStock(Stock company);

        // Get total active orders (bids + asks)
        int getActiveOrders() const;

        // Set active orders count
        void setActiveOrders();

        // Run all increment functions after a trade executes
        void increments(Trader& T);

        // Write the order queue to a file
        void writeQueueFile();

        // Read the queue file to initialize queues
        void readQueueFile();

    private:
        std::priority_queue<Trader> limitBids;                                            // max heap — highest bid first
        std::priority_queue<Trader, std::vector<Trader>, std::greater<Trader>> limitAsks; // min heap — lowest ask first
        Stock s;
        int activeOrders = 0;
};