#include "Transactions.h"
#include <fstream>
#include <iostream>

namespace Transactions
{

    void logTrade(int orderID1, int orderID2, int shares, const std::string& ticker, long price)
    {
        std::ofstream fout("transactions.txt", std::ios::app);
        if (fout.is_open())
        {
            fout << "TRADE: Order #" << orderID1 << " matched with Order #" << orderID2 
            << " for " << shares << " shares of " << ticker << " at $" << price / 100.0 << '\n';
        }
    }

    void logCancel(int orderID, int shares, const std::string& ticker, long price)
    {
        std::ofstream fout("transactions.txt", std::ios::app);
        if (fout.is_open())
        {
            fout << "CANCEL: Order #" << orderID << " for " << shares << " shares of " << ticker 
            << " at $" << price / 100.0 << " was canceled\n";
        }
    }

    void logMarketCancel(int orderID, int shares, const std::string& ticker)
    {
        std::ofstream fout("transactions.txt", std::ios::app);
        if (fout.is_open())
        {
            fout << "CANCEL: Order #" << orderID << " for " << shares << " shares of " << ticker 
            << " was canceled because no shares were available to trade.\n";
        } 
    }

    void printLog()
    {
        std::ifstream fin("transactions.txt");
        if (!fin.is_open())
        {
            std::cout << "No transaction history found.\n";
            return;
        }
        std::string line;
        while (std::getline(fin, line))
            std::cout << line << '\n';
    }

}