#pragma once

#include <string>
#include <vector>
#include "OrderBookEntry.h"
#include "CSVReader.h"

// OrderBook class declaration
class OrderBook
{
public:
    // Constructor that initializes the order book with data from a CSV file
    OrderBook(std::string filename);

    // Returns a list of known product names
    std::vector<std::string> getKnownProducts();

    // Retrieves orders based on type, product, and timestamp
    std::vector<OrderBookEntry> getOrders(OrderBookType type, std::string product, std::string timestamp);

    // Returns the earliest timestamp from the orders
    std::string getEarliestTime();

    // Finds the next timestamp after the given one
    std::string getNextTime(std::string timestamp);

    // Inserts a new order into the order book and sorts orders by timestamp
    void insertOrder(OrderBookEntry &order);

    // Matches asks and bids for a given product and timestamp, returns sales
    std::vector<OrderBookEntry> matchAsksToBids(std::string product, std::string timestamp);

    // Static method to find the highest price from a list of orders
    static double getHighPrice(std::vector<OrderBookEntry> &orders);

    // Static method to find the lowest price from a list of orders
    static double getLowPrice(std::vector<OrderBookEntry> &orders);

private:
    std::vector<OrderBookEntry> orders; // Stores the list of orders
};
