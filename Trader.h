#pragma once

#include <string>
#include <ostream>

class Stock;

class Trader {
public:

    // ─────────────────────────────────────────
    //  Constructor
    // ─────────────────────────────────────────
    Trader(int order, int sharesBid, int sharesAsk, long long money);

    // ─────────────────────────────────────────
    //  Setters
    // ─────────────────────────────────────────
    void setMoney(long long newMoney);    // update balance in CENTS
    void setSharesBid(int newSharesBid);  // update shares trying to BUY
    void setSharesAsk(int newSharesAsk);  // update shares trying to SELL

    // ─────────────────────────────────────────
    //  Getters — all const so priority_queue can call them
    // ─────────────────────────────────────────
    int getOrderNum()  const;  // return the order number (transaction ID)
    int getSharesBid() const;  // return shares trying to BUY
    int getSharesAsk() const;  // return shares trying to SELL
    long long getMoney()     const;  // return current balance in CENTS

    // ─────────────────────────────────────────
    //  Operators
    // ─────────────────────────────────────────
    bool operator==(const Trader& other) const;
    bool operator<(const Trader& other)  const;  // sell queue: lowest ask first
    bool operator>(const Trader& other)  const;  // buy queue:  highest bid first
    friend std::ostream& operator<<(std::ostream& os, const Trader& T);

private:
    int order;      // transaction/order ID
    int shares_bid; // shares trying to BUY
    int shares_ask; // shares trying to SELL
    long long cents;      // balance in cents to avoid float rounding errors
};
