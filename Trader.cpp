#include "Trader.h"
#include "Stock.h"


// ─────────────────────────────────────────
//  Constructor
// ─────────────────────────────────────────


// PRE:  order > 0 (unique ID), sharesBid >= 0, sharesAsk >= 0, money >= 0
// POST: Trader object created with given order ID, shares, and balance in cents
// order     : unique trader/order ID
// sharesBid : shares trying to BUY
// sharesAsk : shares trying to SELL
// money     : balance in CENTS (e.g. 50000 = $500.00)
Trader::Trader(int order, int sharesBid, int sharesAsk, long long money) {
    this->order      = order;
    this->shares_bid = sharesBid;  // shares trying to BUY
    this->shares_ask = sharesAsk;  // shares trying to SELL
    this->cents      = money;      // already in cents, store directly
}

// ─────────────────────────────────────────
//  Getters
// ─────────────────────────────────────────

// PRE:  Trader object has been initialized
// POST: Returns the unique order/transaction ID
int Trader::getOrderNum() const {
    return order;
}

// PRE:  Trader object has been initialized
// POST: Returns number of shares trying to BUY
int Trader::getSharesBid() const {
    return shares_bid;
}

// PRE:  Trader object has been initialized
// POST: Returns number of shares trying to SELL
int Trader::getSharesAsk() const {
    return shares_ask;
}

// PRE:  Trader object has been initialized
// POST: Returns current balance in CENTS (divide by 100 to display as dollars)
//       e.g. returns 15025 for $150.25,
long long Trader::getMoney() const {
    return cents;
}

// ─────────────────────────────────────────
//  Setters
// ─────────────────────────────────────────

// PRE:  newMoney >= 0 (balance cannot be negative)
// POST: Trader's balance updated to newMoney in CENTS
void Trader::setMoney(long long newMoney) {
    this->cents = newMoney;
}

// PRE:  newSharesBid >= 0
// POST: Trader's shares trying to BUY updated to newSharesBid
void Trader::setSharesBid(int newSharesBid) {
    this->shares_bid = newSharesBid;
}

// PRE:  newSharesAsk >= 0
// POST: Trader's shares trying to SELL updated to newSharesAsk
void Trader::setSharesAsk(int newSharesAsk) {
    this->shares_ask = newSharesAsk;
}

// ─────────────────────────────────────────
//  Operators
// ─────────────────────────────────────────

// PRE:  Both Trader objects have been initialized with valid order IDs
// POST: Returns true if both traders have the same order ID, false otherwise
bool Trader::operator==(const Trader& other) const {
    return this->order == other.order;
}

bool Trader::operator<(const Trader& other) const {
    return this->cents < other.cents;
}

bool Trader::operator>(const Trader& other) const {
    return this->cents > other.cents;
}

// PRE:  Trader object has been initialized
// POST: Writes order,sharesBid,sharesAsk,cents to the output 
std::ostream& operator<<(std::ostream& os, const Trader& T) {
    os << T.order << "," << T.shares_bid << "," << T.shares_ask << "," << T.cents;
    return os;
}