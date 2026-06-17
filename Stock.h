#pragma once

#include <string>

class Stock {
public:
    // ── Constructors ──
    Stock() = default;       // added this because needed so map can create empty pairs
    Stock(long initPrice, const std::string& initName, const std::string& initTicker);

    // Basic getters
    std::string getName() const;
    std::string getTicker() const;
    long getPrice() const;

    // Best bid / ask
    long getBestBid() const;
    long getBestAsk() const;
    void setBestBid(long newBid);
    void setBestAsk(long newAsk);

    // Total bid / ask
    int getTotalBids() const;
    int getTotalAsks() const;
    void setTotalBids(int newTotalBids);
    void setTotalAsks(int newTotalAsks);
    
    // Total transactions
    void setTotalTransactions(int newTotalTransactions);
    int getTotalTransactions() const;

    // Spread
    void setSpread(long newBestBid, long newBestAsk);
    long getSpread() const;

    // Update current stock price
    void setPrice(long newPrice);

private:
    long bestBid         = 0;   // added defaults so Stock() = default works
    long bestAsk         = 0;
    long stockSpread     = 0;
    long stockPrice      = 0;
    int totalBids        = 0;
    int totalAsks        = 0;
    int totalTransactions= 0;

    std::string companyName;
    std::string stockTicker;
};

