# Stock Market Simulation

A stock market simulator built in C++ for CSEN 79. Runs as a CGI web app and simulates a real order book with market and limit orders across 100 stocks.

## What it does

- Place **market** or **limit** buy/sell orders on any of 100 stocks
- Orders get matched against the order book in real time
- Live transaction log showing recent trades
- Market stats displayed on the page (total bids, asks, active orders, avg spread, trade count)
- State persists between page loads (queues and prices saved to files)

## How it works

The backend is a C++ CGI program. Each page load reads the current market state from disk, processes any incoming order from the URL query string, and then renders the full HTML page to stdout.

**Classes:**
- `Market` — holds all 100 stocks and their order queues, handles matching logic
- `Stock` — tracks price history and trade data for a single stock
- `Order` — the order book (bid/ask queues) for a single stock
- `Trader` — represents a single order (buy or sell, market or limit)
- `Transactions` — handles writing/reading the transaction log

## Building

```bash
make
```

## Running

Needs to be served through a web server with CGI enabled (e.g. Apache). Drop the compiled binary and `templates/` folder in your CGI directory.

Or run it manually from the command line:

```bash
./mock
```

There's also a test generator (`testGen.cpp`) for generating mock orders to test the order matching logic.

## Files

| File | Description |
|---|---|
| `Main.cpp` | CGI entry point — parses query string, dispatches buy/sell/cancel, renders page |
| `Market.cpp/h` | Core market logic, order matching, state persistence |
| `Stock.cpp/h` | Per-stock price and trade tracking |
| `Order.cpp/h` | Order book (bid/ask queues) |
| `Trader.cpp/h` | Individual order representation |
| `Transactions.cpp/h` | Transaction log |
| `testGen.cpp` | Generates random orders for testing |
| `templates/` | HTML header, footer, and mid-page templates |
