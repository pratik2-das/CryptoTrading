#pragma once

#include <string>
#include <map>
#include <iostream>
#include "OrderBookEntry.h"

using namespace std;

class Wallet
{
public:
    Wallet();                                                // Default constructor for initializing the wallet
    void insertCurrency(string type, double amount);         // Adds currency to the wallet
    bool removeCurrency(string type, double amount);         // Removes currency from the wallet if sufficient funds are available
    bool containsCurrency(string type, double amount);       // Checks if the wallet has enough of the specified currency
    bool canFulfillOrder(OrderBookEntry order);              // Determines if the wallet can fulfill a given order
    void processSale(OrderBookEntry &sale);                  // Updates wallet balance based on a completed sale
    string toString();                                       // Converts the wallet's contents to a string representation
    friend ostream &operator<<(ostream &os, Wallet &wallet); // Overloads the << operator for outputting wallet details

private:
    map<string, double> currencies; // Stores currency type and amount in the wallet
};