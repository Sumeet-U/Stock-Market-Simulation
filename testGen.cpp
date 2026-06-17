/*
 We will generate 100,000 test cases in the format below:
 
 MARKET_BUY|ticker|numShares
 MARKET_SELL|ticker|numShares
 LIMIT_BUY|ticker|numShares|bidPrice
 LIMIT_SELL|ticker|numShares|askPrice
 CANCEL|ticker|orderID
 SPREAD|ticker

 We will use srand seeded with time for random data generation. We ensure any limit buy & sells
 are within -1.5% to +1.5% of the current share price to make sure new orders properly match up
 so we don't have a large list of unfulfilled orders.
 */

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include "Market.h"

int main()
{
    const std::vector<StockData>& STOCKS = STOCK_LIST; // Imported initial stock list from Market
    const int TOTAL_ORDERS = 100000;

    /*  
        We will perform order on the 20 stocks (value = index in STOCK_LIST) below in our test:
        0 = NVDA  (NVIDIA), 1 = AAPL  (Apple), 2 = GOOG  (Alphabet), 3 = MSFT  (Microsoft)
        4 = AMZN  (Amazon), 6 = META  (Meta), 7 = TSLA  (Tesla), 10 = WMT   (Walmart)
        11 = JPM   (JP Morgan), 18 = NFLX  (Netflix), 19 = BAC   (Bank of America)
        21 = COST  (Costco), 22 = HD    (Home Depot), 23 = AMD   (AMD)
        28 = KO    (Coca-Cola), 31 = IBM   (IBM), 35 = GS    (Goldman Sachs)
        42 = MCD   (McDonald's), 51 = DIS   (Disney), 66 = UBER  (Uber)
    */

    const int STOCK_POOL[] = {0, 1, 2, 3, 4, 6, 7, 10, 11, 18, 19, 21, 22, 23, 28, 31, 35, 42, 51, 66};
    const int POOL_SIZE = 20;

    /*
        We want a weighted distribution of commands from the 100,000 orders.
        We chose 40% limit orders, 30% market orders, 20% cancellations, 10% spread inquiries.
        Order Types: 0 = LIMIT_BUY, 1 = LIMIT_SELL, 2 = MARKET_BUY, 3 = MARKET_SELL, 4 = CANCEL, 5 = SPREAD
    */
    const std::vector<int> generationWeights = {20, 20, 15, 15, 20, 10};
    // Lookup table for weighted random selection
    std::vector<int> generationTable;
    for(int gen = 0; gen < (int)generationWeights.size(); gen++)
    {
        for(int w = 0; w < generationWeights[gen]; w++)
        {
            generationTable.push_back(gen);
        }
    }

    // Seed random generator
    srand((unsigned int)time(NULL));

    std::ofstream fout("testGen.txt");
    if (!fout.is_open()) {
        std::cout << "Error: could not open testGen.txt" << std::endl;
        return 1;
    }

    /* 
        For cancellation to be tested properly, I will have to generate order IDs for each active order.
        Our actual code has Trader keeping track of IDs, but as this is a static test generation file.
        activeOrders[ticker] = list of order IDs currently in the order book
    */

    std::vector<std::vector<int>> activeOrders(STOCKS.size());
    int orderID = 1; // order ID counter, increments for every order placed

    for (int i = 0; i < TOTAL_ORDERS; i++)
    {

        // Pick a random command type using the weighted selection table
        int gen = generationTable[rand() % generationTable.size()];

        // Pick a random stock from stock pool of 20 selected stocks
        int ti = STOCK_POOL[rand() % POOL_SIZE];
        const std::string ticker = STOCKS[ti].ticker;
        long basePrice = STOCKS[ti].initPrice;

        // LIMIT_BUY: Bid price is within +-1.5% of base price
        if (gen == 0)
        {
            int shares = (rand() % 500) + 1; // User will buy 1–500 shares
            long lo = (long)(basePrice * 0.985);
            long hi = (long)(basePrice * 1.015);
            long price = lo + rand() % (hi - lo + 1); // will pick a random price within [lo, hi] in cents

            fout << "LIMIT_BUY|" << ticker << "|" << shares << "|" << price << "\n";
            activeOrders[ti].push_back(orderID++);
        }

        // LIMIT_SELL: Ask price is within +-1.5% of the base price
        else if (gen == 1)
        {
            int shares = (rand() % 500) + 1;
            long lo = (long)(basePrice * 0.985);
            long hi = (long)(basePrice * 1.015);
            long price = lo + rand() % (hi - lo + 1);

            fout << "LIMIT_SELL|" << ticker << "|" << shares << "|" << price << "\n";
            activeOrders[ti].push_back(orderID++);
        }

        // MARKET_BUY
        else if (gen == 2)
        {
            int shares = (rand() % 300) + 1; // 1–300 shares
            fout << "MARKET_BUY|" << ticker << "|" << shares << "\n";
            // Market orders execute immediately, either immediately filled or automatically cancelled if not enough shares, so no need to push orderID
            orderID++;
        }

        // MARKET_SELL
        else if (gen == 3)
        {
            int shares = (rand() % 300) + 1;
            fout << "MARKET_SELL|" << ticker << "|" << shares << "\n";
            orderID++;
        }

        /*
            CANCEL: For a cancel order to be created, we will check to make sure there is at least 1 active order for that stock.
            If no active orders but the generator falls on cancel, we will default to a spread inquiry instead.
        */
        else if (gen == 4)
        {
            if (!activeOrders[ti].empty())
            {
                // Pick a random active order and remove it
                int index = rand() % (int)activeOrders[ti].size();
                int cancelID = activeOrders[ti][index];
                activeOrders[ti].erase(activeOrders[ti].begin() + index);

                fout << "CANCEL|" << ticker << "|" << cancelID << "\n";
            }
            else
            {
                fout << "SPREAD|" << ticker << "\n";
            }
        }

        // SPREAD
        else {
            fout << "SPREAD|" << ticker << "\n";
        }
    }

    fout.close();
    return 0;
}