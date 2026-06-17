#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <chrono>
#include <functional>
#include "Market.h"
#include "Stock.h"
#include "Trader.h"
#include "Order.h"
#include "Transactions.h"

using namespace std::chrono;

int main() {

    // ─────────────────────────────────────────
    //  Setup: load all 100 stocks into market
    // ─────────────────────────────────────────
    Market market;
    market.loadDefaultStocks();

    // Open testGen.txt which contains 10,000 randomly generated orders
    // This file is created by running testGen first
    std::ifstream file("testGen.txt");
    if (!file.is_open()) {
        std::cout << "Error: testGen.txt not found. Run testGen first!" << std::endl;
        return 1;
    }

    // Open mockResults.txt to write the summary after all orders run
    std::ofstream results("mockResults.txt");
    if (!results.is_open()) {
        std::cout << "Error: could not open mockResults.txt" << std::endl;
        return 1;
    }

    std::string line;

    // orderCounter starts at 1 and increments every time a new order is placed
    int orderCounter = 1;

    // errors counts how many lines had an invalid ticker or unrecognized command
    int errors = 0;
    
    double timeMarketBuy = 0;  long long countMarketBuy = 0;
    double timeMarketSell = 0; long long countMarketSell = 0;
    double timeLimitBuy = 0;   long long countLimitBuy = 0;
    double timeLimitSell = 0;  long long countLimitSell = 0;
    double timeCancel = 0;     long long countCancel = 0;
    double timeSpread = 0;     long long countSpread = 0;
    
    // Start clock for counter
    high_resolution_clock::time_point loopStart = high_resolution_clock::now();

    // ─────────────────────────────────────────
    //  Read testGen.txt line by line
    // ─────────────────────────────────────────
    while (std::getline(file, line)) {

        // Tokenize the line by '|' into a vector of strings
        // e.g. "LIMIT_BUY|AAPL|100|17500" → ["LIMIT_BUY", "AAPL", "100", "17500"]
        std::vector<std::string> inputs;
        std::stringstream ss(line);
        std::string token;
        while (std::getline(ss, token, '|')) {
            inputs.push_back(token);
        }

        // Skip empty lines
        if (inputs.empty()) continue;

        // First token is always the command type
        std::string cmd = inputs[0];

        // ── MARKET_BUY|TICKER|SHARES ──
        // Buyer wants to purchase at best available price immediately
        // sharesBid = shares, sharesAsk = 0, price = 0 (market = no fixed price)
        if (cmd == "MARKET_BUY" && inputs.size() == 3) {
            std::string ticker = inputs[1];
            int shares         = std::stoi(inputs[2]);

            Stock* s = market.getStock(ticker);
            if (!s) { errors++; continue; }  // skip if ticker not found

            Trader T(orderCounter++, shares, 0, 0);
            
            //  Set t0 before buy operation, set t1 after. Perform t1-t0 add
            //  to total operation time. Divide operation timer by total operation count
            high_resolution_clock::time_point t0 = high_resolution_clock::now();
            market.addTrader(ticker, T, "markBid");
            high_resolution_clock::time_point t1 = high_resolution_clock::now();
            timeMarketBuy += duration_cast<duration<double>>(t1 - t0).count();
            countMarketBuy++;
        }

        // ── MARKET_SELL|TICKER|SHARES ──
        // Seller wants to sell at best available price immediately
        // sharesBid = 0, sharesAsk = shares, price = 0 (market = no fixed price)
        else if (cmd == "MARKET_SELL" && inputs.size() == 3) {
            std::string ticker = inputs[1];
            int shares         = std::stoi(inputs[2]);

            Stock* s = market.getStock(ticker);
            if (!s) { errors++; continue; }

            Trader T(orderCounter++, 0, shares, 0);
            
            high_resolution_clock::time_point t0 = high_resolution_clock::now();
            market.addTrader(ticker, T, "markAsk");
            high_resolution_clock::time_point t1 = high_resolution_clock::now();
            timeMarketSell += duration_cast<duration<double>>(t1 - t0).count();
            countMarketSell++;

        }

        // ── LIMIT_BUY|TICKER|SHARES|PRICE ──
        // Buyer sets a max price they are willing to pay (in cents)
        // Sits in the priority queue until a matching sell comes in
        else if (cmd == "LIMIT_BUY" && inputs.size() == 4) {
            std::string ticker = inputs[1];
            int shares         = std::stoi(inputs[2]);
            long long price    = std::stoll(inputs[3]);  // price in cents

            Stock* s = market.getStock(ticker);
            if (!s) { errors++; continue; }

            Trader T(orderCounter++, shares, 0, price);
            
            high_resolution_clock::time_point t0 = high_resolution_clock::now();
            market.addTrader(ticker, T, "limBid");
            high_resolution_clock::time_point t1 = high_resolution_clock::now();
            timeLimitBuy += duration_cast<duration<double>>(t1 - t0).count();
            countLimitBuy++;
        }

        // ── LIMIT_SELL|TICKER|SHARES|PRICE ──
        // Seller sets a min price they are willing to accept (in cents)
        // Sits in the priority queue until a matching buy comes in
        else if (cmd == "LIMIT_SELL" && inputs.size() == 4) {
            std::string ticker = inputs[1];
            int shares         = std::stoi(inputs[2]);
            long long price    = std::stoll(inputs[3]);  // price in cents

            Stock* s = market.getStock(ticker);
            if (!s) { errors++; continue; }

            Trader T(orderCounter++, 0, shares, price);
            
            high_resolution_clock::time_point t0 = high_resolution_clock::now();
            market.addTrader(ticker, T, "limAsk");
            high_resolution_clock::time_point t1 = high_resolution_clock::now();
            timeLimitSell += duration_cast<duration<double>>(t1 - t0).count();
            countLimitSell++;
        }

        // ── CANCEL|TICKER|ORDER_ID ──
        // Remove an existing order from the queue by its order ID
        // market.rmTrader finds the trader by ID using operator== and removes them
        else if (cmd == "CANCEL" && inputs.size() == 3) {
            std::string ticker = inputs[1];
            int cancelID       = std::stoi(inputs[2]);

            Stock* s = market.getStock(ticker);
            if (!s) { errors++; continue; }

            high_resolution_clock::time_point t0 = high_resolution_clock::now();
            market.rmTrader(ticker, cancelID);
            high_resolution_clock::time_point t1 = high_resolution_clock::now();
            timeCancel += duration_cast<duration<double>>(t1 - t0).count();
            countCancel++;
        }

        // ── SPREAD|TICKER ──
        // Just validate the ticker exists
        // Spread is automatically tracked in Stock whenever bestBid/bestAsk update
        else if (cmd == "SPREAD" && inputs.size() == 2) {
            Stock* s = market.getStock(inputs[1]);
            if (!s) { errors++; continue; }
            
            high_resolution_clock::time_point t0 = high_resolution_clock::now();
            s->getSpread();   // O(1) read — already maintained in Stock
            high_resolution_clock::time_point t1 = high_resolution_clock::now();
            timeSpread += duration_cast<duration<double>>(t1 - t0).count();
            countSpread++;
        }

        // ── Unknown command or malformed line ──
        else {
            errors++;
        }
    }
    
    // End timer
    high_resolution_clock::time_point loopEnd = high_resolution_clock::now();
    double totalLoopSec = duration_cast<duration<double>>(loopEnd - loopStart).count();

    file.close();
    
    // Created a safe average function to avoid divide by zero errors
    std::function<double(double, long long)> avg = [](double total, long long count) -> double {
        return count > 0 ? total / (double)count : 0.0;
    };


    // ─────────────────────────────────────────
    //  Write summary to mockResults.txt
    //  Uses Market.cpp functions instead of manual counters
    //  All trade details are already in transactions.txt
    // ─────────────────────────────────────────
    results << "--------MOCK TEST RESULTS--------"                << std::endl;
    results << "Total Bids      : " << market.totalBids()         << std::endl;  // sum of getTotalBids() across all stocks
    results << "Total Asks      : " << market.totalAsks()         << std::endl;  // sum of getTotalAsks() across all stocks
    results << "Completed Trades: " << market.numTrades()         << std::endl;  // sum of getTotalTransactions() across all stocks
    results << "Active Orders   : " << market.activeOrders()      << std::endl;  // orders still in queue
    results << "Avg Spread (¢)  : " << market.numSpread()         << std::endl;  // average spread across all stocks in cents
    results << "Last Order ID   : " << orderCounter - 1           << std::endl;  // highest order ID assigned
    results << "Errors          : " << errors                     << std::endl;  // bad tickers or malformed lines
    results << "================================="                << std::endl;
    
    results << "\n--------TIMING RESULTS (seconds)--------"       << std::endl;
    results << std::fixed;
    results.precision(6);

    //  For each of operation of market buy/sell, limit buy/sell, cancellations, spread calls
    //  we will display the total number of calls, total execution time, and average execution time for each operation type
    results << "Market Buy  | calls: " << countMarketBuy
            << " | total: " << timeMarketBuy  << " s"
            << " | avg: "   << avg(timeMarketBuy,  countMarketBuy)  << " s/op" << std::endl;
    results << "Market Sell | calls: " << countMarketSell
            << " | total: " << timeMarketSell << " s"
            << " | avg: "   << avg(timeMarketSell, countMarketSell) << " s/op" << std::endl;
    results << "Limit Buy   | calls: " << countLimitBuy
            << " | total: " << timeLimitBuy   << " s"
            << " | avg: "   << avg(timeLimitBuy,   countLimitBuy)   << " s/op" << std::endl;
    results << "Limit Sell  | calls: " << countLimitSell
            << " | total: " << timeLimitSell  << " s"
            << " | avg: "   << avg(timeLimitSell,  countLimitSell)  << " s/op" << std::endl;
    results << "Cancel      | calls: " << countCancel
            << " | total: " << timeCancel     << " s"
            << " | avg: "   << avg(timeCancel,     countCancel)     << " s/op" << std::endl;
    results << "Spread Call | calls: " << countSpread
            << " | total: " << timeSpread     << " s"
            << " | avg: "   << avg(timeSpread,     countSpread)     << " s/op" << std::endl;
    results << "\nTotal loop time  : " << totalLoopSec << " s" << std::endl;

    results << "================================="                 << std::endl;
    results.close();

    // ─────────────────────────────────────────
    //  Terminal: only confirm test is done
    // ─────────────────────────────────────────
    std::cout << "Stress test complete!"          << std::endl;
    std::cout << "Summary   - mockResults.txt"    << std::endl;
    std::cout << "Trade log - transactions.txt"   << std::endl;

    return 0;
}