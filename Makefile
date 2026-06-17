# Stock Trading Machine
CXX      = g++
OPT      = -g
STD      = -std=c++17
CXXFLAGS = $(OPT) $(STD) -pthread -I.

%.o: %.cpp
	$(CXX) -c $(CXXFLAGS) $<

SRCS = Main.cpp Trader.cpp Stock.cpp Order.cpp Market.cpp Transactions.cpp
OBJS = $(SRCS:.cpp=.o)
ALL  = stock_machine

all: $(ALL)

stock_machine: $(OBJS)
	$(CXX) -o $@ $(CXXFLAGS) $+

clean:
	/bin/rm -f $(ALL) $(OBJS) transactions.txt
	/bin/rm -rf $(ALL:.=.dSYM)

# DEPENDENTS
Main.o:         Main.cpp         Market.h Stock.h Trader.h Order.h Transactions.h
mockMain.o:     mockMain.cpp     Market.h Stock.h Trader.h Order.h Transactions.h
testGen.o:      testGen.cpp      Market.h
Market.o:       Market.cpp       Market.h Stock.h Trader.h Order.h Transactions.h
Stock.o:        Stock.cpp        Stock.h
Trader.o:       Trader.cpp       Trader.h Stock.h
Order.o:        Order.cpp        Order.h  Trader.h Stock.h Transactions.h
Transactions.o: Transactions.cpp Transactions.h
