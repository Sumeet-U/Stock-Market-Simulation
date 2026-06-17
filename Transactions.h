#pragma once
#include <string>

namespace Transactions
{
    // Successful trade between two orders
    void logTrade(int orderID1, int orderID2, int shares, const std::string& ticker, long price);

    // Log a cancelled limit buy/sell order
    void logCancel(int orderID, int shares, const std::string& ticker, long price);

    // Log a cancelled market buy/sell order
    void logMarketCancel(int orderID, int shares, const std::string& ticker);

    // Print the full transaction log
    void printLog();
}