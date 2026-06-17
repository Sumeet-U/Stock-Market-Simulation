#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <sstream>
#include <vector>
#include "Market.h"
#include "Stock.h"
#include "Trader.h"
#include "Order.h"
#include "Transactions.h"

// Get querey string
std::string getParam(const std::string& query, const std::string& key) {
    // Find the key in the query string
    size_t pos = query.find(key + "=");
    if (pos == std::string::npos){
        return "";
    }
    // Isolate the variable in the string
    size_t start = pos + key.length() + 1;
    size_t end   = query.find("&", start);
    if (end == std::string::npos){
        end = query.length();
    }
    return query.substr(start, end - start);
}

// Print the new html (update the header, footer, and mid of the HTML page)
void printFile(const std::string& filename) {
    std::ifstream file(filename);
    std::string line;
    while (getline(file, line)) {
        std::cout << line << "\n";
    }
}

// read from your backend and print HTML
void printTransactionLog() {
    // Open queues file
    std::ifstream f("transactions.txt");
    if (!f.is_open()) return;

    // Create a vector to store the logs
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(f, line)){
        if (!line.empty()){
            lines.push_back(line);
        }
    }
    // Get the first 100 logs.
    const int MAX_DISPLAY = 100;
    int start = std::max(0, (int)lines.size() - MAX_DISPLAY);

    for (int i = (int)lines.size() - 1; i >= start; --i) {
        std::cout << "<div class='transaction-item'>"
            << "<div class='transaction-details'>"
            << lines[i]
            << "</div>"
            << "</div>\n";
    }
    
}

// Read from your backend and print HTML
void printStocks(Market& market) {
    std::cout << market.output();
}

// Print the page in the browser (update HTML page)
void printPage(Market& market) {
    printFile("templates/header.html");
    printTransactionLog();
    printFile("templates/mid.html");
    printStocks(market);

    std::cout << "<script>\n"
        << market.printStockJS()
        << "document.getElementById('totalStocks').textContent = '100';\n"
        << "document.getElementById('totalBids').textContent = '" << market.totalBids() << "';\n"
        << "document.getElementById('totalAsks').textContent = '" << market.totalAsks() << "';\n"
        << "document.getElementById('activeOrders').textContent = '" << market.activeOrders() << "';\n"
        << "document.getElementById('totalTrades').textContent = '" << market.numTrades() << "';\n"
        << "document.getElementById('avgSpread').textContent = '$" << market.numSpread() / 100.0 << "';\n"
        << "</script>\n";

    printFile("templates/footer.html");
}

int main() {
    std::cout << "Content-type: text/html\n\n";

    // Initialize Market
    Market market;
    market.loadDefaultStocks();
    market.loadStocks();
    market.loadQueues();
    market.loadOrderID();

    // Get the querey string with the data from the url
    std::string query;
    if(getenv("QUERY_STRING") != NULL){
        query = getenv("QUERY_STRING");
    }
    else{
        query = "";
    }
    // Get the action executed
    std::string action = getParam(query, "action");

    // Check if the user executed a buy
    if (action == "buy") {
        std::string ticker = getParam(query, "ticker");
        if(!getParam(query, "shares").empty()){
            int order = market.getNextOrderID();
            int shares = std::stoi(getParam(query, "shares"));
            std::string orderType  = getParam(query, "order_type");
            // Check if buy was a market buy of a limit buy
            if(orderType == "limit"){
                long limitPrice = std::stol(getParam(query, "limit_price")) * 100;
                Trader T(order, shares, 0, limitPrice);
                market.addTrader(ticker, T, "limBid");
                std::cerr << "Order added\n";
            }
            else{
                Trader T(order, shares, 0, 0);
                market.addTrader(ticker, T, "markBid");
                std::cerr << "Order added\n";
            }
        }
    }
    // Check if the user executed a sell
    else if (action == "sell") {
        std::string ticker = getParam(query, "ticker");
        if(!getParam(query, "shares").empty()){
            int order = market.getNextOrderID();
            int shares = std::stoi(getParam(query, "shares"));
            std::string orderType  = getParam(query, "order_type");
            // Check if ask was a market ask of a limit ask
            if(orderType == "limit"){
                long limitPrice = std::stol(getParam(query, "limit_price")) * 100;
                Trader T(order, 0, shares, limitPrice);
                market.addTrader(ticker, T, "limAsk");
                std::cerr << "Order added\n";
            }
            else{
                Trader T(order, 0, shares, 0);
                market.addTrader(ticker, T, "markAsk");
                std::cerr << "Order added\n";
            }
        }
    }
    //Check if the user executed a cancel
    else if (action == "cancel") {
        std::string ticker = getParam(query, "ticker");
        int orderID = std::stoi(getParam(query, "order_id"));
        market.rmTrader(ticker, orderID);
    }

    //Save market state
    if (!action.empty()) {
        market.saveQueues();
        market.saveOrderID();
        market.saveStocks();
    }

    printPage(market);
    return 0;
}