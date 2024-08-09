#pragma once

#include <string>

using namespace std;

// Enum representing different types of orders in the order book
enum class OrderBookType
{
    bid,     // Represents a bid order
    ask,     // Represents an ask order
    unknown, // Represents an unknown order type
    asksale, // Represents a sale from an ask order
    bidsale  // Represents a sale from a bid order
};

// Class representing an entry in the order book
class OrderBookEntry
{
public:
    // Constructor to initialize an OrderBookEntry with given parameters
    OrderBookEntry(double _price, double _amount, string _timestamp, string _product, OrderBookType _orderType, string username = "dataset");

    // Converts a string to its corresponding OrderBookType enum value
    static OrderBookType stringToOrderBookType(string s);

    // Comparison function for sorting by timestamp (ascending)
    static bool compareByTimestamp(OrderBookEntry &e1, OrderBookEntry &e2)
    {
        return e1.timestamp < e2.timestamp;
    }

    // Comparison function for sorting by price (ascending)
    static bool compareByPriceAsc(OrderBookEntry &e1, OrderBookEntry &e2)
    {
        return e1.price < e2.price;
    }

    // Comparison function for sorting by price (descending)
    static bool compareByPriceDesc(OrderBookEntry &e1, OrderBookEntry &e2)
    {
        return e1.price > e2.price;
    }

    // Data members
    double price;            // The price of the order
    double amount;           // The amount of the order
    string timestamp;        // The timestamp when the order was made
    string product;          // The product associated with the order
    OrderBookType orderType; // The type of the order (bid/ask/unknown)
    string username;         // The username associated with the order
};
