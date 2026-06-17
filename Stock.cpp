#include "Stock.h"

// Constructor
// PRE:  initPrice > 0, initName and initTicker are non-empty strings
// POST: Stock object is created with the given price, name, and ticker;
//       all counters (totalBids, totalAsks, totalTransactions) and
//       price fields (bestBid, bestAsk, stockSpread) are initialized to 0
Stock::Stock(long initPrice, const std::string& initName, const std::string& initTicker)
{
    bestBid = 0;
    bestAsk = 0;
    stockSpread = 0;
    totalBids = 0;
    totalAsks = 0;
    totalTransactions = 0;
    stockPrice = initPrice;
    companyName = initName;
    stockTicker = initTicker;
}

// Getters
// PRE:  Stock object has been initialized
// POST: returns the company name as a string
std::string Stock::getName() const
{
    return companyName;
}

// PRE:  Stock object has been initialized
// POST: returns the ticker symbol as a string
std::string Stock::getTicker() const
{
    return stockTicker;
}


// PRE:  Stock object has been initialized
// POST: returns the current market price
long Stock::getPrice() const
{
    return stockPrice;
}

// Best Bid & Ask
// PRE:  Stock object has been initialized
// POST: returns the current best bid price
long Stock::getBestBid() const
{
    return bestBid;
}

// PRE:  Stock object has been initialized
// POST: returns the current best ask price
long Stock::getBestAsk() const
{
    return bestAsk;
}

// PRE:  newBid > 0
// POST: bestBid is updated to newBid and spread is recalculated
void Stock::setBestBid(long newBid)
{
    bestBid = newBid;
    setSpread(bestBid, bestAsk);
}


// PRE:  newAsk > 0
// POST: bestAsk is updated to newAsk and spread is recalculated
void Stock::setBestAsk(long newAsk)
{
    bestAsk = newAsk;
    setSpread(bestBid, bestAsk);
}

// Total Bid & Ask
// PRE:  Stock object has been initialized
// POST: returns the total number of bids placed on this stock
int Stock::getTotalBids() const
{
    return totalBids;
}


// PRE:  Stock object has been initialized
// POST: returns the total number of asks placed on this stock
int Stock::getTotalAsks() const
{
    return totalAsks;
}

// PRE:  newTotalBids >= 0
// POST: totalBids is updated to newTotalBids
void Stock::setTotalBids(int newTotalBids)
{
    totalBids = newTotalBids;
}

// PRE:  newTotalAsks >= 0
// POST: totalAsks is updated to newTotalAsks
void Stock::setTotalAsks(int newTotalAsks)
{
    totalAsks = newTotalAsks;
}

// Total Transactions
// PRE:  newTotalTransactions >= 0
// POST: totalTransactions is updated to newTotalTransactions
void Stock::setTotalTransactions(int newTotalTransactions)
{
    totalTransactions = newTotalTransactions;
}

// PRE:  Stock object has been initialized
// POST: returns the total number of completed transactions for this stock
int Stock::getTotalTransactions() const
{
    return totalTransactions;
}

// Spread
// PRE:  newBestBid >= 0, newBestAsk >= 0
// POST: bestBid and bestAsk are updated; if both are greater than 0,
//       stockSpread is set to (bestAsk - bestBid), else stockSpread is set to 0
void Stock::setSpread(long newBestBid, long newBestAsk)
{
    bestBid = newBestBid;
    bestAsk = newBestAsk;

    if((bestBid > 0) && (bestAsk > 0) && (bestAsk > bestBid))
    {
        stockSpread = bestAsk - bestBid;
    }
    else
    {
        stockSpread = 0;
    }
}


// PRE:  Stock object has been initialized
// POST: returns the current spread (bestAsk - bestBid)
long Stock::getSpread() const
{
    return stockSpread;
}

// Update the market price
// PRE:  newPrice > 0
// POST: stockPrice is updated to newPrice; if newPrice <=0 then stockPrice does not change
void Stock::setPrice(long newPrice)
{
    if(newPrice > 0)
    {
        stockPrice = newPrice;
    }
}
