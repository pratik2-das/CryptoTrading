#include <map>
#include <algorithm>
#include <iostream>
#include "OrderBook.h"
#include "CSVReader.h"

using namespace std;

// Constructor that initializes the order book with data from a CSV file
OrderBook::OrderBook(string filename)
{
    orders = CSVReader::readCSV(filename);
}

// Returns a list of known products by extracting unique product names from orders
vector<string> OrderBook::getKnownProducts()
{
    vector<string> products;
    map<string, bool> prodMap; // Use a map to ensure uniqueness
    for (OrderBookEntry &e : orders)
    {
        prodMap[e.product] = true; // Mark the product as seen
    }
    for (auto const &e : prodMap)
    {
        products.push_back(e.first); // Collect unique products
    }
    return products;
}

// Retrieves orders based on type, product, and timestamp
vector<OrderBookEntry> OrderBook::getOrders(OrderBookType type, string product, string timestamp)
{
    vector<OrderBookEntry> orders_sub;
    for (OrderBookEntry &e : orders)
    {
        if (e.orderType == type &&
            e.product == product &&
            e.timestamp == timestamp)
        {
            orders_sub.push_back(e); // Add matching orders to the result
        }
    }
    return orders_sub;
}

// Finds the highest price from a list of orders
double OrderBook::getHighPrice(vector<OrderBookEntry> &orders)
{
    double max = orders[0].price;
    for (OrderBookEntry &e : orders)
    {
        if (e.price > max)
            max = e.price; // Update max price
    }
    return max;
}

// Finds the lowest price from a list of orders
double OrderBook::getLowPrice(vector<OrderBookEntry> &orders)
{
    double min = orders[0].price;
    for (OrderBookEntry &e : orders)
    {
        if (e.price < min)
            min = e.price; // Update min price
    }
    return min;
}

// Returns the earliest timestamp from the orders
string OrderBook::getEarliestTime()
{
    return orders[0].timestamp; // Assume orders are sorted by timestamp
}

// Finds the next timestamp after the given one
string OrderBook::getNextTime(string timestamp)
{
    string next_timestamp = "";
    for (OrderBookEntry &e : orders)
    {
        if (e.timestamp > timestamp)
        {
            next_timestamp = e.timestamp; // Update next timestamp
            break;
        }
    }
    if (next_timestamp == "")
    {
        next_timestamp = orders[0].timestamp; // Wrap around if no later timestamp found
    }
    return next_timestamp;
}

// Inserts a new order into the order book and sorts the orders by timestamp
void OrderBook::insertOrder(OrderBookEntry &order)
{
    orders.push_back(order);
    sort(orders.begin(), orders.end(), OrderBookEntry::compareByTimestamp); // Sort by timestamp
}

// Matches asks and bids for a given product and timestamp
vector<OrderBookEntry> OrderBook::matchAsksToBids(string product, string timestamp)
{
    vector<OrderBookEntry> asks = getOrders(OrderBookType::ask, product, timestamp);
    vector<OrderBookEntry> bids = getOrders(OrderBookType::bid, product, timestamp);
    vector<OrderBookEntry> sales;
    if (asks.size() == 0 || bids.size() == 0)
    {
        cout << "OrderBook::matchAsksToBids no bids or asks" << endl;
        return sales; // No matching if no asks or bids
    }
    sort(asks.begin(), asks.end(), OrderBookEntry::compareByPriceAsc);  // Sort asks by price ascending
    sort(bids.begin(), bids.end(), OrderBookEntry::compareByPriceDesc); // Sort bids by price descending
    cout << "max ask " << asks[asks.size() - 1].price << endl;
    cout << "min ask " << asks[0].price << endl;
    cout << "max bid " << bids[0].price << endl;
    cout << "min bid " << bids[bids.size() - 1].price << endl;
    // Match asks with bids
    for (OrderBookEntry &ask : asks)
    {
        for (OrderBookEntry &bid : bids)
        {
            if (bid.price >= ask.price)
            {
                OrderBookEntry sale{ask.price, 0, timestamp, product, OrderBookType::asksale};

                // Assign username and orderType based on the matched order
                if (bid.username == "simuser")
                {
                    sale.username = "simuser";
                    sale.orderType = OrderBookType::bidsale;
                }
                if (ask.username == "simuser")
                {
                    sale.username = "simuser";
                    sale.orderType = OrderBookType::asksale;
                }
                // Handle different cases based on amounts
                if (bid.amount == ask.amount)
                {
                    sale.amount = ask.amount;
                    sales.push_back(sale);
                    bid.amount = 0; // Reset bid amount
                    break;
                }
                if (bid.amount > ask.amount)
                {
                    sale.amount = ask.amount;
                    sales.push_back(sale);
                    bid.amount = bid.amount - ask.amount; // Adjust bid amount
                    break;
                }
                if (bid.amount < ask.amount &&
                    bid.amount > 0)
                {
                    sale.amount = bid.amount;
                    sales.push_back(sale);
                    ask.amount = ask.amount - bid.amount; // Adjust ask amount
                    bid.amount = 0;
                    continue;
                }
            }
        }
    }
    return sales;
}
