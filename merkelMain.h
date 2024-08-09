#pragma once

#include <vector>
#include "OrderBookEntry.h"
#include "OrderBook.h"
#include "Wallet.h"

using namespace std;

// MerkelMain class declaration
class MerkelMain
{
public:
    // Default constructor
    MerkelMain();

    // Initializes the application
    void init();

private:
    // Prints the menu options
    void printMenu();

    // Prints help information
    void printHelp();

    // Prints market statistics for known products
    void printMarketStats();

    // Handles input and processing for placing an ask order
    void enterAsk();

    // Handles input and processing for placing a bid order
    void enterBid();

    // Prints the current state of the wallet
    void printWallet();

    // Advances to the next time frame and processes sales
    void gotoNextTimeframe();

    // Prompts the user for an option and returns the selected option
    int getUserOption();

    // Processes the user's menu selection
    void processUserOption(int userOption);

    string currentTime; // Stores the current time in the simulation

    OrderBook orderBook{"datasheet.csv"}; // Order book initialized with a CSV file

    Wallet wallet; // User's wallet for managing funds
};
