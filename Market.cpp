#include "Market.h"
#include "Transactions.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>

// Initial data for our 100 stocks (using top 100 stocks by market cap)

const std::vector<StockData> STOCK_LIST = {
    { "NVDA",  "NVIDIA Corporation",                    17675 },
    { "AAPL",  "Apple Inc.",                            27377 },
    { "GOOG",  "Alphabet Inc.",                         30858 },
    { "MSFT",  "Microsoft Corporation",                 47423 },
    { "AMZN",  "Amazon.com Inc.",                       22353 },
    { "AVGO",  "Broadcom Inc.",                         34415 },
    { "META",  "Meta Platforms Inc.",                   64398 },
    { "TSLA",  "Tesla Inc.",                            47794 },
    { "BRKB",  "Berkshire Hathaway Inc.",               50000 },
    { "LLY",   "Eli Lilly and Company",                105832 },
    { "WMT",   "Walmart Inc.",                          11603 },
    { "JPM",   "JP Morgan Chase & Co.",                 32109 },
    { "V",     "Visa Inc.",                             34737 },
    { "ORCL",  "Oracle Corporation",                    18241 },
    { "JNJ",   "Johnson & Johnson",                     21395 },
    { "MA",    "Mastercard Incorporated",               57183 },
    { "XOM",   "Exxon Mobil Corporation",               11742 },
    { "PLTR",  "Palantir Technologies Inc.",            18519 },
    { "NFLX",  "Netflix Inc.",                          98000 },
    { "BAC",   "Bank of America Corporation",            5557 },
    { "ABBV",  "AbbVie Inc.",                           22695 },
    { "COST",  "Costco Wholesale Corporation",          87070 },
    { "HD",    "Home Depot Inc.",                       35948 },
    { "AMD",   "Advanced Micro Devices Inc.",           21130 },
    { "PG",    "Procter & Gamble Company",              14454 },
    { "GE",    "GE Aerospace",                          30544 },
    { "CSCO",  "Cisco Systems Inc.",                     7788 },
    { "UNH",   "UnitedHealth Group Incorporated",       33853 },
    { "KO",    "Coca-Cola Company",                      7090 },
    { "CVX",   "Chevron Corporation",                   14917 },
    { "WFC",   "Wells Fargo & Company",                  9346 },
    { "IBM",   "International Business Machines Corp.", 30794 },
    { "MS",    "Morgan Stanley",                        18039 },
    { "CAT",   "Caterpillar Inc.",                      59240 },
    { "MU",    "Micron Technology Inc.",                24329 },
    { "GS",    "Goldman Sachs Group Inc.",              60000 },
    { "AXP",   "American Express Company",              38283 },
    { "MRK",   "Merck & Company Inc.",                  10020 },
    { "PM",    "Philip Morris International Inc.",      15899 },
    { "RTX",   "RTX Corporation",                       18056 },
    { "CRM",   "Salesforce Inc.",                       25419 },
    { "APP",   "Applovin Corporation",                  68393 },
    { "MCD",   "McDonald's Corporation",                31744 },
    { "ABT",   "Abbott Laboratories",                   12793 },
    { "TMUS",  "T-Mobile US Inc.",                      19756 },
    { "TMO",   "Thermo Fisher Scientific Inc.",         56658 },
    { "SHOP",  "Shopify Inc.",                          16248 },
    { "AMAT",  "Applied Materials Inc.",                26129 },
    { "LRCX",  "Lam Research Corporation",              16441 },
    { "PEP",   "PepsiCo Inc.",                          15069 },
    { "C",     "Citigroup Inc.",                        11285 },
    { "DIS",   "Walt Disney Company",                   11081 },
    { "ISRG",  "Intuitive Surgical Inc.",               55399 },
    { "LIN",   "Linde plc",                             41818 },
    { "QCOM",  "QUALCOMM Incorporated",                 17793 },
    { "GEV",   "GE Vernova Inc.",                       68061 },
    { "BX",    "Blackstone Inc.",                       15113 },
    { "INTU",  "Intuit Inc.",                           65829 },
    { "INTC",  "Intel Corporation",                      3760 },
    { "BLK",   "BlackRock Inc.",                        50000 },
    { "BKNG",  "Booking Holdings Inc.",                 45000 },
    { "TJX",   "TJX Companies Inc.",                    15683 },
    { "VZ",    "Verizon Communications Inc.",            4111 },
    { "T",     "AT&T Inc.",                              2439 },
    { "AMGN",  "Amgen Inc.",                            31995 },
    { "SCHW",  "Charles Schwab Corporation",             9649 },
    { "UBER",  "Uber Technologies Inc.",                 8232 },
    { "ACN",   "Accenture plc",                         27204 },
    { "NEE",   "NextEra Energy Inc.",                    8083 },
    { "TXN",   "Texas Instruments Incorporated",        17812 },
    { "KLAC",  "KLA Corporation",                       55000 },
    { "NOW",   "ServiceNow Inc.",                       77480 },
    { "APH",   "Amphenol Corporation",                  13076 },
    { "BA",    "Boeing Company",                        20411 },
    { "DHR",   "Danaher Corporation",                   22532 },
    { "ANET",  "Arista Networks Inc.",                  12506 },
    { "COF",   "Capital One Financial Corporation",     23912 },
    { "SPGI",  "S&P Global Inc.",                       50020 },
    { "PFE",   "Pfizer Inc.",                            2628 },
    { "GILD",  "Gilead Sciences Inc.",                  12034 },
    { "ADBE",  "Adobe Inc.",                            34810 },
    { "UNP",   "Union Pacific Corporation",             23963 },
    { "LOW",   "Lowe's Companies Inc.",                 24763 },
    { "PGR",   "Progressive Corporation",               23616 },
    { "ADI",   "Analog Devices Inc.",                   28020 },
    { "BSX",   "Boston Scientific Corporation",          9210 },
    { "SYK",   "Stryker Corporation",                   35189 },
    { "DE",    "Deere & Company",                       48668 },
    { "PANW",  "Palo Alto Networks Inc.",               18747 },
    { "ETN",   "Eaton Corporation plc",                 33392 },
    { "MDT",   "Medtronic plc",                          9896 },
    { "CRWD",  "CrowdStrike Holdings Inc.",             49672 },
    { "HON",   "Honeywell International Inc.",          19366 },
    { "CB",    "Chubb Limited",                         31173 },
    { "KKR",   "KKR & Co. Inc.",                        13451 },
    { "SPOT",  "Spotify Technology S.A.",               58077 },
    { "SCCO",  "Southern Copper Corporation",           14476 },
    { "COP",   "ConocoPhillips",                         9396 },
    { "VRTX",  "Vertex Pharmaceuticals Incorporated",   45115 },
    { "BN",    "Brookfield Corporation",                 4524 },
};

// PRE:  STOCK_LIST is non-empty
// POST: stockOrders has one (Stock, Order) pair per ticker
// creates all default stocks, creates stock and order object, and adds them to the market 
// loops through every entry in STOCK_LIST, makes a Stock and an empty Order for each one,
// then stores them together in the map (populates the market on startup)
void Market::loadDefaultStocks() {
    for (int i = 0; i < (int)STOCK_LIST.size(); i++) {
        Stock s(STOCK_LIST[i].initPrice, STOCK_LIST[i].name, STOCK_LIST[i].ticker);
        Order o(s);
        pairStockOrder(s, o);
    }
}

// PRE:  s.getTicker() is non-empty
// POST: stockOrders["AAPL"] = {Stock, Order}
// adds (stock, order) pair to map using ticker as key
// stock and order into a pair and drops it into the map 
// if the ticker already exists it gets overwritten, otherwise a new entry is created
void Market::pairStockOrder(const Stock& s, const Order& o) {
    stockOrders[s.getTicker()] = std::pair<Stock, Order>(s, o);
}

// PRE:  ticker exists in stockOrders, T is a valid Trader, 
//       type is "limBid", "markBid", "limAsk", or "markAsk"
// POST: T is added to the correct queue for that stock
// adds trader's order to correct queue; finds stock, accesses order book
// looks up the stock by ticker, then figures out which queue to put the trader in
// based on the type string. after adding, we sync the stock back from the order
// because a trade might have executed inside Order and changed the price
void Market::addTrader(const std::string& ticker, Trader T, const std::string& type) {
    std::map<std::string, std::pair<Stock, Order>>::iterator it = stockOrders.find(ticker);
    if (it == stockOrders.end()) {
        std::cout << "Stock not found: " << ticker << std::endl;
        return;
    }
    Order& o = it->second.second;
    if (type == "limBid") {
        o.addLimBid(T);
    }
    else if (type == "markBid") {
        o.addMarkBid(T);
    }
    else if (type == "limAsk") {
        o.addLimAsk(T);
    }
    else if (type == "markAsk") {
        o.addMarkAsk(T);
    }
    else {
        std::cout << "Unknown order type: " << type << std::endl;
    }

    it->second.first = o.getStock();
}

// PRE:  ticker exists in stockOrders, orderNum matches a trader in  queue
// POST: trader is removed from the queue and order is cancelled
// cancels existing order 
// looks up the stock, then tries to cancel the order with the matching ID 
// cancelOrder() returns true if it found and removed it, false if the ID didn't exist
void Market::rmTrader(const std::string& ticker, int orderNum) {
    std::map<std::string, std::pair<Stock, Order>>::iterator it = stockOrders.find(ticker);
    if (it == stockOrders.end()) {
        std::cout << "Stock not found: " << ticker << std::endl;
        return;
    }
    Order& o = it->second.second;
    bool cancelled = o.cancelOrder(orderNum);
    if (cancelled) {
        std::cout << "Order #" << orderNum << " cancelled." << std::endl;
    }
    else {
        std::cout << "Order #" << orderNum << " not found in queue." << std::endl;
    }

    it->second.first = o.getStock();
}

// PRE:  ticker exists in stockOrders
// POST: returns pointer to Stock, or NULL
// returns pointer to stock object 
// pair.first = Stock, pair.second = Order
// we return a pointer instead of a copy so the caller can modify the stock directly
Stock* Market::getStock(const std::string& ticker) {
    std::map<std::string, std::pair<Stock, Order>>::iterator it = stockOrders.find(ticker);
    if (it != stockOrders.end()) {
        return &it->second.first;
    }
    return NULL;
}

// PRE:  ticker exists in stockOrders
// POST: returns pointer to Order (the queues), or NULL
// returns pointer to order book
// same idea as getStock, pointer so the caller can actually modify the queues
Order* Market::getOrder(const std::string& ticker) {
    std::map<std::string, std::pair<Stock, Order>>::iterator it = stockOrders.find(ticker);
    if (it != stockOrders.end()) {
        return &it->second.second;
    }
    return NULL;
}

// builds one HTML table row per stock for the frontend to display
// prices are stored in cents so we divide by 100.0 to get dollar values
std::string Market::output() {
    std::ostringstream html;
    html << std::fixed << std::setprecision(2);
    std::map<std::string, std::pair<Stock, Order>>::iterator it;
    for (it = stockOrders.begin(); it != stockOrders.end(); it++) {
        Stock s = it->second.first;
        Order o = it->second.second;
        html << "<tr>"
             << "<td>"  << s.getTicker()           << "</td>"
             << "<td>"  << s.getName()              << "</td>"
             << "<td>"  << o.numBids()              << "</td>"
             << "<td>"  << o.numAsks()              << "</td>"
             << "<td>$" << s.getBestBid() / 100.0  << "</td>"
             << "<td>$" << s.getBestAsk() / 100.0  << "</td>"
             << "<td>$" << s.getSpread()  / 100.0  << "</td>"
             << "<td>$" << s.getPrice()   / 100.0  << "</td>"
             << "</tr>";
    }
    return html.str();
}

// POST: prints the transaction log from file
void Market::writeToFile() {
    Transactions::printLog();
}

// When the page reloads all pending orders disapear from memory. 
// These two functions fix that by saving orders to a file before reload
// and loading them back when the program starts up again

// PRE:  the market has stocks loaded (loadDefaultStocks() was called first)
// POST: every stock's buy/sell queue is saved to a file 
// clears the file first so we don't end up with duplicate data from the last save,
// then goes through every stock and tells its Order to write its queues to the file
void Market::saveQueues() {
    std::ofstream clear("queues.txt");
    clear.close();
    
    std::map<std::string, std::pair<Stock, Order>>::iterator it;
    for (it = stockOrders.begin(); it != stockOrders.end(); it++) {
        it->second.second.writeQueueFile();
    }
}

// PRE:  saveQueues() has been called before so the files exist
// POST: every stock's buy/sell queue is restored from the file back into memory
// goes through every stock and tells its Order to read its queues back from the file
void Market::loadQueues() {
    std::map<std::string, std::pair<Stock, Order>>::iterator it;
    for (it = stockOrders.begin(); it != stockOrders.end(); it++) {
        it->second.second.readQueueFile();
        it->second.second.setStock(it->second.first);
        it->second.second.setActiveOrders();
    }
}

// POST: returns total number of completed transactions across all stocks
// loops through every stock and adds up their individual transaction counts
int Market::numTrades() {
    int total = 0;
    std::map<std::string, std::pair<Stock, Order>>::iterator it;
    for (it = stockOrders.begin(); it != stockOrders.end(); it++) {
        Stock s = it->second.first;
        total += s.getTotalTransactions();
    }
    return total;
}

// POST: returns average spread in cents across all stocks
// uses long for the running total to avoid overflow, then divides by stock count at the end
int Market::numSpread() {
    if (stockOrders.empty()) {
        return 0;
    }
    long total = 0;
    std::map<std::string, std::pair<Stock, Order>>::iterator it;
    for (it = stockOrders.begin(); it != stockOrders.end(); it++) {
        Stock s = it->second.first;
        total += s.getSpread();
    }
    return (int)(total / (long)stockOrders.size());
}

// POST: returns total number of bids across all stocks
// loops and sums up each stock's cumulative bid count
int Market::totalBids() {
    int total = 0;
    std::map<std::string, std::pair<Stock, Order>>::iterator it;
    for (it = stockOrders.begin(); it != stockOrders.end(); it++) {
        Stock s = it->second.first;
        total += s.getTotalBids();
    }
    return total;
}

// POST: returns total number of asks across all stocks
// same as totalBids but for the ask side
int Market::totalAsks() {
    int total = 0;
    std::map<std::string, std::pair<Stock, Order>>::iterator it;
    for (it = stockOrders.begin(); it != stockOrders.end(); it++) {
        Stock s = it->second.first;
        total += s.getTotalAsks();
    }
    return total;
}

// POST: returns total number of active orders across all stocks
// counts orders currently sitting in queues waiting to be matched (not yet filled or cancelled)
int Market::activeOrders() {
    int total = 0;
    std::map<std::string, std::pair<Stock, Order>>::iterator it;
    for (it = stockOrders.begin(); it != stockOrders.end(); it++) {
        Order o = it->second.second;
        total += o.getActiveOrders();
    }
    return total;
}

// writes currentOrderID to a file so it survives a server restart 
// without this, IDs would reset to 0 and conflict with existing orders
void Market::saveOrderID() {
    std::ofstream f("/var/www/html/cgi-bin/orderID.txt");
    f << currentOrderID;
}

// reads currentOrderID back from the file . if the file doesn't exist yet
// (first ever run) we just default to 0
void Market::loadOrderID() {
    std::ifstream f("/var/www/html/cgi-bin/orderID.txt");
    if (!f.is_open()) {
        currentOrderID = 0;
        return;
    }
    f >> currentOrderID;
}

// increments first so the first ID handed out is 1, not 0
int Market::getNextOrderID() {
    currentOrderID++;
    return currentOrderID;
}

// Pre:  stockOrders is populated with at least one entry
// Post: stocks.txt is created with all stock data
// saves each stock as one comma-separated line so we can restore prices
// and counters after a restart — format is ticker,price,bid,ask,spread,bids,asks,trades
void Market::saveStocks() {
    std::ofstream f("/var/www/html/cgi-bin/stocks.txt");
    std::map<std::string, std::pair<Stock, Order>>::iterator it;
    for (it = stockOrders.begin(); it != stockOrders.end(); it++) {
        Stock s = it->second.first;
        f << s.getTicker() << ","
          << s.getPrice() << ","
          << s.getBestBid() << ","
          << s.getBestAsk() << ","
          << s.getSpread() << ","
          << s.getTotalBids() << ","
          << s.getTotalAsks() << ","
          << s.getTotalTransactions() << "\n";
    }
}

// Pre:  stocks.txt exists and contains valid comma-separated stock data
// Post: all matching stocks in stockOrders have their state restored
// stol: string to long, stoi: string to int 
// reads each line, splits by commas into tokens, then finds the matching stock
// in the map and calls all its setters to restore the saved state 
// skips any line that has a ticker we don't recognize
void Market::loadStocks() {
    std::ifstream f("/var/www/html/cgi-bin/stocks.txt");
    if (!f.is_open()) 
        return;
    std::string line;
    while (std::getline(f, line)) {
        if (line.empty()) 
            continue;
        std::istringstream ss(line);
        std::vector<std::string> tokens;
        std::string token;
        while (std::getline(ss, token, ','))
            tokens.push_back(token);
        if (tokens.size() < 8) 
            continue;
        std::string ticker = tokens[0];
        long price         = std::stol(tokens[1]);
        long bestBid       = std::stol(tokens[2]);
        long bestAsk       = std::stol(tokens[3]);
        long spread        = std::stol(tokens[4]);
        int totalBids      = std::stoi(tokens[5]);
        int totalAsks      = std::stoi(tokens[6]);
        int totalTrans     = std::stoi(tokens[7]);
        std::map<std::string, std::pair<Stock, Order>>::iterator it = stockOrders.find(ticker);
        if (it == stockOrders.end()) 
            continue;
        Stock& s = it->second.first;
        s.setPrice(price);
        s.setBestBid(bestBid);
        s.setBestAsk(bestAsk);
        s.setSpread(bestBid, bestAsk);
        s.setTotalBids(totalBids);
        s.setTotalAsks(totalAsks);
        s.setTotalTransactions(totalTrans);
    }
}

// function that returns string of javascript code
// builds a JS array of all stocks that gets injected into the HTML page
// so the frontend knows all tickers and current prices on load 
// change and changePercent are hardcoded to 0 for now as placeholders
std::string Market::printStockJS() {
    std::ostringstream js;
    js << std::fixed << std::setprecision(2);
    js << "const stocks = [\n";
    std::map<std::string, std::pair<Stock, Order>>::iterator it;
    for (it = stockOrders.begin(); it != stockOrders.end(); it++) {
        Stock s = it->second.first;
        js << "{ ticker: \"" << s.getTicker() << "\", "
           << "name: \"" << s.getName() << "\", "
           << "currentPrice: " << s.getPrice() / 100.0 << ", "
           << "change: 0, changePercent: 0 },\n";
    }
    js << "];\n";
    return js.str();
}
