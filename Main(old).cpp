#include <iostream>
#include <string>
#include "Market.h"
#include "Stock.h"
#include "Trader.h"
#include "Order.h"  

// ─────────────────────────────────────────
//  Display Menu
// ─────────────────────────────────────────
void displayMenu() {
    std::cout << "\n===== Stock Trading Machine =====" << std::endl;
    std::cout << "1.  Place Buy Limit Order"           << std::endl;
    std::cout << "2.  Place Buy Market Order"          << std::endl;
    std::cout << "3.  Place Sell Limit Order"          << std::endl;
    std::cout << "4.  Place Sell Market Order"         << std::endl;
    std::cout << "5.  Cancel Order"                    << std::endl;
    std::cout << "6.  View Stock Info"                 << std::endl;
    std::cout << "7.  View Order Book"                 << std::endl;
    std::cout << "8.  View Trade History"              << std::endl;
    std::cout << "9.  Admin Dashboard"                 << std::endl;
    std::cout << "10. Add Trader"                      << std::endl;
    std::cout << "0.  Exit"                            << std::endl;
    std::cout << "================================="   << std::endl;
    std::cout << "Enter choice: ";
}

void displayStockInfo(Stock* s) {
    if (!s) { std::cout << "Stock not found!" << std::endl; return; }
    std::cout << "\n--- " << s->getTicker() << " | " << s->getName() << " ---" << std::endl;
    std::cout << "Market Price : $" << s->getPrice()   / 100.0 << std::endl;
    std::cout << "Best Bid     : $" << s->getBestBid() / 100.0 << std::endl;
    std::cout << "Best Ask     : $" << s->getBestAsk() / 100.0 << std::endl;
    std::cout << "Spread       : $" << s->getSpread()  / 100.0 << std::endl;
}

// ─────────────────────────────────────────
//  Admin Dashboard
// ─────────────────────────────────────────
void displayAdmin(Market& market) {
    std::cout << "\n========= ADMIN DASHBOARD =========" << std::endl;
    std::cout << "Ticker | Bids | Asks | Price"          << std::endl;
    std::cout << "-----------------------------------"    << std::endl;

    int totalBids = 0, totalAsks = 0;

    for (const auto& sd : STOCK_LIST) {
        Stock* s = market.getStock(sd.ticker);
        if (!s) continue;
        int bids = s->getTotalBids();
        int asks = s->getTotalAsks();
        totalBids += bids;
        totalAsks += asks;
        if (bids > 0 || asks > 0) {
            std::cout << sd.ticker
                      << " | " << bids
                      << " | " << asks
                      << " | $" << s->getPrice() / 100.0
                      << std::endl;
        }
    }

    std::cout << "-----------------------------------" << std::endl;
    std::cout << "TOTAL BIDS : " << totalBids          << std::endl;
    std::cout << "TOTAL ASKS : " << totalAsks          << std::endl;
    std::cout << "===================================" << std::endl;
}

// ─────────────────────────────────────────
//  Main
// ─────────────────────────────────────────
int main() {
    Market market;

    // ── Load all 100 stocks ──
    for (int i = 0; i < (int)STOCK_LIST.size(); i++) {
        Stock s(STOCK_LIST[i].initPrice,
                STOCK_LIST[i].name,
                STOCK_LIST[i].ticker);
        market.addStock(s);
    }
    std::cout << "✓ Loaded " << STOCK_LIST.size() << " stocks." << std::endl;

    // ─────────────────────────────────────────
    //  ID Counters — both start at 1
    // ─────────────────────────────────────────
    int traderCounter = 1;  // trader IDs: 1, 2, 3, ...
    int orderCounter  = 1;  // order IDs:  1, 2, 3, ...

    int choice;
    std::cout << "Welcome to the Stock Trading Machine!" << std::endl;

    do {
        displayMenu();
        std::cin >> choice;

        // ── Add Trader ──
        if (choice == 10) {
            int sharesBid, sharesAsk;
            long long balance;
            std::cout << "Shares to BUY  : "; std::cin >> sharesBid;
            std::cout << "Shares to SELL : "; std::cin >> sharesAsk;
            std::cout << "Balance (cents): "; std::cin >> balance;

            Trader t(traderCounter, sharesBid, sharesAsk, balance);
            market.addTrader(t);
            std::cout << "Trader #" << traderCounter << " added!" << std::endl;
            traderCounter++;  // 1 → 2 → 3 → ...
        }

        // ── Buy Limit Order ──
        else if (choice == 1) {
           
        }

        // ── Buy Market Order ──
        else if (choice == 2) {
            
        }

        // ── Sell Limit Order ──
        else if (choice == 3) {
            
        }

        // ── Sell Market Order ──
        else if (choice == 4) {
            
        }

        // ── Cancel Order ──
        else if (choice == 5) {
            
        }

        // ── View Stock Info ──
        else if (choice == 6) {
            std::string ticker;
            std::cout << "Ticker: "; std::cin >> ticker;
            displayStockInfo(market.getStock(ticker));
        }

        // ── View Order Book ──
        else if (choice == 7) {
            std::string ticker;
            std::cout << "Ticker: "; std::cin >> ticker;
            Stock* s = market.getStock(ticker);
            if (s) {
                std::cout << "\n--- Order Book: " << ticker << " ---" << std::endl;
                std::cout << "Total Bids : " << s->getTotalBids() << std::endl;
                std::cout << "Total Asks : " << s->getTotalAsks() << std::endl;
            } else {
                std::cout << "✗ Stock not found!" << std::endl;
            }
        }

        // ── View Trade History ──
        else if (choice == 8) {
            market.printTransactionLog();
        }

        // ── Admin Dashboard ──
        else if (choice == 9) {
            displayAdmin(market);
        }

        else if (choice != 0) {
            std::cout << "✗ Invalid choice!" << std::endl;
        }

    } while (choice != 0);

    std::cout << "Goodbye!" << std::endl;
    return 0;
}
