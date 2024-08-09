#include <iostream>
#include "Wallet.h"
#include "CSVReader.h"

using namespace std;

// Default constructor for Wallet
Wallet::Wallet() {}

// Inserts a specified amount of currency into the wallet
void Wallet::insertCurrency(string type, double amount)
{
    double balance;
    // Throw exception if the amount is negative
    if (amount < 0)
    {
        throw exception{};
    }
    // Initialize balance if currency type is new
    if (currencies.count(type) == 0)
    {
        balance = 0;
    }
    else
    {
        balance = currencies[type];
    }
    // Add amount to the balance
    balance += amount;
    currencies[type] = balance;
}

// Removes a specified amount of currency from the wallet
bool Wallet::removeCurrency(string type, double amount)
{
    // Return false if the amount is negative
    if (amount < 0)
    {
        return false;
    }
    // Return false if currency type is not found
    if (currencies.count(type) == 0)
    {
        return false;
    }
    else
    {
        // Check if sufficient currency is available and adjust balance
        if (containsCurrency(type, amount))
        {
            currencies[type] -= amount;
            return true;
        }
        else
            return false;
    }
}

// Checks if the wallet contains sufficient currency
bool Wallet::containsCurrency(string type, double amount)
{
    // Return false if currency type is not found
    if (currencies.count(type) == 0)
        return false;
    else
        return currencies[type] >= amount; // Check if sufficient amount is available
}

// Converts the wallet's contents to a string representation
string Wallet::toString()
{
    string s;
    // Iterate through each currency and its balance
    for (pair<string, double> pair : currencies)
    {
        string currency = pair.first;
        double amount = pair.second;
        s += currency + " : " + to_string(amount) + "\n";
    }
    return s;
}

// Checks if the wallet can fulfill a given order
bool Wallet::canFulfillOrder(OrderBookEntry order)
{
    vector<string> currs = CSVReader::tokenise(order.product, '/');
    // Check if the wallet can fulfill an ask order
    if (order.orderType == OrderBookType::ask)
    {
        double amount = order.amount;
        string currency = currs[0];
        cout << "Wallet::canFulfillOrder " << currency << " : " << amount << endl;

        return containsCurrency(currency, amount);
    }
    // Check if the wallet can fulfill a bid order
    if (order.orderType == OrderBookType::bid)
    {
        double amount = order.amount * order.price;
        string currency = currs[1];
        cout << "Wallet::canFulfillOrder " << currency << " : " << amount << endl;
        return containsCurrency(currency, amount);
    }

    return false; // Order type not recognized
}

// Processes a sale by updating the wallet's balances
void Wallet::processSale(OrderBookEntry &sale)
{
    vector<string> currs = CSVReader::tokenise(sale.product, '/');
    // Handle the sale from an ask order
    if (sale.orderType == OrderBookType::asksale)
    {
        double outgoingAmount = sale.amount;
        string outgoingCurrency = currs[0];
        double incomingAmount = sale.amount * sale.price;
        string incomingCurrency = currs[1];

        currencies[incomingCurrency] += incomingAmount;
        currencies[outgoingCurrency] -= outgoingAmount;
    }
    // Handle the sale from a bid order
    if (sale.orderType == OrderBookType::bidsale)
    {
        double incomingAmount = sale.amount;
        string incomingCurrency = currs[0];
        double outgoingAmount = sale.amount * sale.price;
        string outgoingCurrency = currs[1];

        currencies[incomingCurrency] += incomingAmount;
        currencies[outgoingCurrency] -= outgoingAmount;
    }
}

// Overloads the << operator to output the wallet's contents
ostream &operator<<(ostream &os, Wallet &wallet)
{
    os << wallet.toString();
    return os;
}
