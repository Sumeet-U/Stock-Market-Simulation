#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>  
#include <queue>
#include <string>
#include "Trader.h"
#include "Stock.h"
#include "Order.h"
#include "Transactions.h"

using namespace Transactions;

// Constructor for Order class
// PRE: the company is valid, initialized Stock object with a ticker ad price
// POST: new Order object is created and limitBids ad limitAsks priority queues are empty 
Order::Order(Stock company){
    s = company;
}

// Add a market order to the market priority queue
// PRE: T has valid sharesBid (positive) ad valid order number
// POST: T's bid is matched against limit asks; any unmatched shares are cancelled
void Order::addMarkBid(Trader& T){
    setTotBids();
    while(!limitAsks.empty()){
        Trader la = limitAsks.top();
        T.setMoney(la.getMoney());
        // Check if there are more bid shares than ask shares
        if(T.getSharesBid() > la.getSharesAsk()){
            // Log the transaction and handle any increments for market price, tranaction numbers, etc.
            logTrade(la.getOrderNum(), T.getOrderNum(), la.getSharesAsk(), s.getTicker(), T.getMoney());
            increments(T);
            T.setSharesBid(T.getSharesBid() - la.getSharesAsk());
            la.setSharesAsk(0);
            limitAsks.pop();
        }
        // Check if there are more ask shares than bid shares
        else if(la.getSharesAsk() > T.getSharesBid()){
            // Log the transaction and handle any increments for market price, tranaction numbers, etc.
            logTrade(la.getOrderNum(), T.getOrderNum(), T.getSharesBid(), s.getTicker(), T.getMoney());
            increments(T);
            la.setSharesAsk(la.getSharesAsk() - T.getSharesBid());
            T.setSharesBid(0);
            limitAsks.pop();
            limitAsks.push(la);
            break;
        }
        else{
            // Log the transaction and handle any increments for market price, tranaction numbers, etc.
            logTrade(la.getOrderNum(), T.getOrderNum(), T.getSharesBid(), s.getTicker(), T.getMoney());
            increments(T);
            la.setSharesAsk(0);
            T.setSharesBid(0);
            limitAsks.pop();
            break;
        }
    }
    // If there are leftover shares cancel the remaining order
    if(T.getSharesBid() > 0){
        logMarketCancel(T.getOrderNum(), T.getSharesBid(), s.getTicker());
    }
}

// Add a limit order to the limit priority queue
// PRE: T has a valid sharesBid (positive), valid price, and valid order number
// POST: T is added to limitBids; a trade is executed if bid price >= lowest ask price 
void Order::addLimBid(Trader& T){
    setTotBids();
    limitBids.push(T);
    if(limitAsks.empty()){
        return;
    }
    Trader la = limitAsks.top();
    Trader lb = limitBids.top();
    if(lb.getMoney() >= la.getMoney()){
        // Check if there are extra shares asked
        if(la.getSharesAsk() > lb.getSharesBid()){
            // Log the transaction and handle any increments for market price, tranaction numbers, etc.
            logTrade(la.getOrderNum(), lb.getOrderNum(), lb.getSharesBid(), s.getTicker(), lb.getMoney());
            increments(lb);
            la.setSharesAsk(la.getSharesAsk() - lb.getSharesBid());
            lb.setSharesBid(0);
            limitBids.pop();
            limitAsks.pop();
            limitAsks.push(la);
        }
        // Check if there are extra shares bid
        else if(lb.getSharesBid() > la.getSharesAsk()){
            // Log the transaction and handle any increments for market price, tranaction numbers, etc.
            logTrade(la.getOrderNum(), lb.getOrderNum(), la.getSharesAsk(), s.getTicker(), lb.getMoney());
            increments(lb);
            lb.setSharesBid(lb.getSharesBid() - la.getSharesAsk());
            la.setSharesAsk(0);
            limitAsks.pop();
            limitBids.pop();
            limitBids.push(lb);
        }
        else{
            // Log the transaction and handle any increments for market price, tranaction numbers, etc.
            logTrade(la.getOrderNum(), lb.getOrderNum(), lb.getSharesBid(), s.getTicker(), lb.getMoney());
            increments(lb);
            la.setSharesAsk(0);
            lb.setSharesBid(0);
            limitAsks.pop();
            limitBids.pop();
        }
    }
}

// Add a sell order to the sell priority queue
// PRE: T has a valid sharesAsk (positive), valid price, and valid order number
// POST: T is added to limitAsks; a trade is executed if bid price >= lowest ask price  
void Order::addLimAsk(Trader& T){
    setTotAsks();
    limitAsks.push(T);
    if(limitBids.empty()){
        return;
    }
    Trader la = limitAsks.top();
    Trader lb = limitBids.top();
    // If money matches execute the corresponding transaction
    if(lb.getMoney() >= la.getMoney()){
        // Check if there are extra shares asked
        if(la.getSharesAsk() > lb.getSharesBid()){
            // Log the transaction and handle any increments for market price, tranaction numbers, etc.
            logTrade(la.getOrderNum(), lb.getOrderNum(), lb.getSharesBid(), s.getTicker(), lb.getMoney());
            increments(lb);
            la.setSharesAsk(la.getSharesAsk() - lb.getSharesBid());
            lb.setSharesBid(0);
            limitBids.pop();
            limitAsks.pop();
            limitAsks.push(la);
        }
        // Check if there are extra shares bid
        else if(lb.getSharesBid() > la.getSharesAsk()){
            // Log the transaction and handle any increments for market price, tranaction numbers, etc.
            logTrade(la.getOrderNum(), lb.getOrderNum(), la.getSharesAsk(), s.getTicker(), lb.getMoney());
            increments(lb);
            lb.setSharesBid(lb.getSharesBid() - la.getSharesAsk());
            la.setSharesAsk(0);
            limitAsks.pop();
            limitBids.pop();
            limitBids.push(lb);
        }
        else{
            // Log the transaction and handle any increments for market price, tranaction numbers, etc.
            logTrade(la.getOrderNum(), lb.getOrderNum(), la.getSharesAsk(), s.getTicker(), lb.getMoney());
            increments(lb);
            la.setSharesAsk(0);
            lb.setSharesBid(0);
            limitAsks.pop();
            limitBids.pop();
        }
    }
}

// Add a sell market order to the sell market priority queue
// PRE: T has postive value for sharesAsk and valid order number 
// POST: T's asj is matched against limit bids; if there are any unmatched shares, they are canelled 
void Order::addMarkAsk(Trader& T){
    setTotAsks();
    while(!limitBids.empty()){
        Trader lb = limitBids.top();
        T.setMoney(lb.getMoney());
        // Check if there are more bid shares than ask shares
        if(T.getSharesAsk() > lb.getSharesBid()){
            // Log the transaction and handle any increments for market price, tranaction numbers, etc.
            logTrade(T.getOrderNum(), lb.getOrderNum(), lb.getSharesBid(), s.getTicker(), T.getMoney());
            increments(T);
            T.setSharesAsk(T.getSharesAsk() - lb.getSharesBid());
            lb.setSharesBid(0);
            limitBids.pop();
        }
        // Check if there are more ask shares than bid shares
        else if(lb.getSharesBid() > T.getSharesAsk()){
            // Log the transaction and handle any increments for market price, tranaction numbers, etc.
            logTrade(T.getOrderNum(), lb.getOrderNum(), T.getSharesAsk(), s.getTicker(), T.getMoney());
            increments(T);
            lb.setSharesBid(lb.getSharesBid() - T.getSharesAsk());
            T.setSharesAsk(0);
            limitBids.pop();
            limitBids.push(lb);
            break;
        }
        else{
            // Log the transaction and handle any increments for market price, tranaction numbers, etc.
            logTrade(T.getOrderNum(), lb.getOrderNum(), T.getSharesAsk(), s.getTicker(), T.getMoney());
            increments(T);
            lb.setSharesBid(0);
            T.setSharesAsk(0);
            limitBids.pop();
            break;
        }
    }
    // If there are leftover shares cancel the remaining order
    if(T.getSharesAsk() > 0){
        logMarketCancel(T.getOrderNum(), T.getSharesAsk(), s.getTicker());
    }
}

// Cancel an order by trader
// PRE: orderID is postive int that can exist in limitBids or limitAsks
// POST: if found, order is removed and logged and returns true
bool Order::cancelOrder(int orderID){
    std::priority_queue<Trader> bid = limitBids;
    std::priority_queue<Trader, std::vector<Trader>, std::greater<Trader>> ask = limitAsks;
    std::priority_queue<Trader> newBid;
    std::priority_queue<Trader, std::vector<Trader>, std::greater<Trader>> newAsk;
    int flag = 0;
    // Loop through limit bid queue to find the trader
    while(!bid.empty()){
        // If person found remove from queue
        if(orderID == bid.top().getOrderNum()){
            logCancel(orderID, bid.top().getSharesBid(), s.getTicker(), bid.top().getMoney());
            flag = 1;
            bid.pop();
        }
        else{
            newBid.push(bid.top());
            bid.pop();
        }
    }
    // Check if the person has been found
    if(flag == 1){
        limitBids = newBid;
        return true;
    }
    else{
        // Loop through limit ask queue to find the trader
        while(!ask.empty()){
            // If person found remove from queue
            if(orderID == ask.top().getOrderNum()){
                logCancel(orderID, ask.top().getSharesAsk(), s.getTicker(), ask.top().getMoney());
                flag = 1;
                ask.pop();
            }
            else{
                newAsk.push(ask.top());
                ask.pop();
            }
        }
    }
    // Check if the person has been found
    if(flag == 1){
        limitAsks = newAsk;
        return true;
    }

    return false;
}

// Set the best bid price for a stock
// PRE: limitBids can be empty or contain data 
// POST: s.bestBid is updated to highest bid price 
void Order::bestBid(){
    if(!limitBids.empty()){
        s.setBestBid(limitBids.top().getMoney());
    } else {
        s.setBestBid(0);  // reset if no bids in queue
    }
}

// Set the best ask price for a stock
// PRE: limitAsks can be empty or contain data
// POST: s.bestAsk is updated to lowest ask price 
void Order::bestAsk(){
    if(!limitAsks.empty()){
        s.setBestAsk(limitAsks.top().getMoney());
    } else {
        s.setBestAsk(0);  // reset if no asks in queue
    }
}

// Set the market price for a stock
// PRE: T's price reflects most recently executed trade
// POST: s.price is updated to T's price 
void Order::marketPrice(Trader& T){
    // Set the market price to the most recent sale price
    if(T.getMoney() != 0){
        s.setPrice(T.getMoney());
    }
}

// Set spread
// PRE: s.bestBid and s.bestAsk are set
// POST: s.spread is updated to (bestAsk - bestBid)
void Order::spread(){
    // Get the current bestBid and bestAsk
    long bestBid = s.getBestBid();
    long bestAsk = s.getBestAsk();
    s.setSpread(bestBid, bestAsk);
}

// Set the number of total asks for a stock
// PRE: s.totalAsks holds current ask count
// POST: s.totalAsks increments by 1
void Order::setTotAsks(){
    // Get the current total number of asks and increment by 1
    int totalAsks = s.getTotalAsks();
    totalAsks++;
    s.setTotalAsks(totalAsks);
}

// Set the number of total bids for a stock
// PRE: s.totalBids holds current bid count
// POST: s.totalBids increments by 1
void Order::setTotBids(){
    // Get the current total number of bids and increment by 1
    int totalBids = s.getTotalBids();
    totalBids++;
    s.setTotalBids(totalBids);
}

// Set the number of total transactions
// PRE: s.totalTransactions holds current transaction count
// POST: s.totalTransactions increments by 1
void Order::setTotTransact(){
    // Get the current total number of transactions and increment by 1
    int totalTransactions = s.getTotalTransactions();
    totalTransactions++;
    s.setTotalTransactions(totalTransactions);
}

// Get num asks
// PRE: order is initialized
// POST: returns number of orders in limitAsks
int Order::numAsks() const {
    return(limitAsks.size());
}

// Get num bids
// PRE: order is initialized
// POST: returns number of orders in limitBids
int Order::numBids() const {
    return(limitBids.size());
}

// Get stock
// PRE: order is initialized with valid Stock
// POST: returns copy of s
Stock Order::getStock() const {
    return(s);
}

// Set stock
// PRE: company is valid Stock object
// POST: s is reaplced with company
void Order::setStock(Stock company){
    s = company;
}

// Get active ordres
// PRE: order is initialized 
// POST: returns current activeOrders count
int Order::getActiveOrders() const {
    return(activeOrders);
}

// Set active orders
// PRE: limitBids ad limitAsks show the current pending orders
// POST: activeOrders is set to total size of both queues 
void Order::setActiveOrders(){
    activeOrders = 0;
    activeOrders += limitBids.size();
    activeOrders += limitAsks.size();
}

// Write the current queues to a file
void Order::writeQueueFile(){
    // Create queues file
    std::ofstream fout("queues.txt", std::ios::app);
    std::priority_queue<Trader> bid = limitBids;
    std::priority_queue<Trader, std::vector<Trader>, std::greater<Trader>> ask = limitAsks;
    // Check if the file is open
    if (fout.is_open())
    {
        fout << "Stock Ticker:" << s.getTicker() << '\n';
        // Loop through limit ask queue to write to file
        std::cerr << "Writing queue for " << s.getTicker() << " asks: " << limitAsks.size() << " bids: " << limitBids.size() << "\n";
        while(!ask.empty()){
            fout << "ASK," << ask.top() << '\n';
            ask.pop();
        }
        // Loop through the limit buy queue to write to file
        while(!bid.empty()){
            fout << "BID," << bid.top() << '\n';
            bid.pop();
        }
    }
}

// Read the queue file to initialize queues
void Order::readQueueFile(){
    // Open queues file
    std::ifstream file("queues.txt");
    if (!file.is_open()) {
        return;
    }

    std::string line;
    int flag = 0;
    // Loop through the file line by line
    while(std::getline(file, line)){
        // Check if line contains the correct ticker
        if(line.find("Stock Ticker:") != std::string::npos){
            std::string ticker = line.substr(line.find(':') + 1);
            if(ticker == s.getTicker()){
                flag = 1;
                continue;
            }
            else if(flag == 1){
                return;
            }
            else{
                continue;
            }
        }

        // Check if ticker was found
        if(flag == 0){
            continue;
        }
        
        // Turn each line into a vector of strings
        std::vector<std::string> inputs;
        std::stringstream ss(line);
        std::string token;

        // Create a vector with each token from trader
        while(std::getline(ss,token, ',')){
            inputs.push_back(token);
        }

        // Make sure there is no missing data
        if(inputs.size() < 5){
            continue;
        }

        // Initialze Trader data
        int orderID = std::stoi(inputs[1]);
        int shares_bid = std::stoi(inputs[2]);
        int shares_ask = std::stoi(inputs[3]);
        long cents = std::stol(inputs[4]);
        Trader T(orderID, shares_bid, shares_ask, cents);

        // Check if trader is an ask or a bid
        if(inputs[0] == "ASK"){
            limitAsks.push(T);
        }

        else{
            limitBids.push(T);
        }
    }
}

// Run all increment functions
// PRE: T's price reflects most recently executed trade
// POST: updates bestBid, bestAsk, market price, total counts, and activeOrders
void Order::increments(Trader& T){
    marketPrice(T);
    bestAsk();
    bestBid();
    setTotTransact();
    setActiveOrders();
    return;
}


