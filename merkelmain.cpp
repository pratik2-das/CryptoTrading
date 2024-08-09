#include <iostream>
#include <vector>
#include "MerkelMain.h"
#include "OrderBookEntry.h"
#include "CSVReader.h"

using namespace std;

// Default constructor for MerkelMain
MerkelMain::MerkelMain() {}

// Initializes the application by setting the current time and inserting initial currency
void MerkelMain::init()
{
    int input;                                 // Variable to store user input
    currentTime = orderBook.getEarliestTime(); // Initialize currentTime to the earliest time in the order book
    wallet.insertCurrency("BTC", 10);          // Add initial currency to the wallet
    while (true)                               // Main application loop
    {
        printMenu();              // Display the menu
        input = getUserOption();  // Get user input
        processUserOption(input); // Process the user's choice
    }
}

// Prints the menu options to the console
void MerkelMain::printMenu()
{
    cout << "1: Print help " << endl;
    cout << "2: Print exchange stats" << endl;
    cout << "3: Make an offer " << endl;
    cout << "4: Make a bid " << endl;
    cout << "5: Print wallet " << endl;
    cout << "6: Continue " << endl;
    cout << "============== " << endl;
    cout << "Current time is: " << currentTime << endl; // Display the current time
}

// Prints help information to the console
void MerkelMain::printHelp()
{
    cout << "Help - your aim is to make money. Analyse the market and make bids and offers. " << endl;
}

// Prints market statistics for known products
void MerkelMain::printMarketStats()
{
    // Iterate over each known product
    for (string const &p : orderBook.getKnownProducts())
    {
        cout << "Product: " << p << endl;
        // Get the list of orders of type 'ask' for the current product
        vector<OrderBookEntry> entries = orderBook.getOrders(OrderBookType::ask, p, currentTime);
        cout << "Asks seen: " << entries.size() << endl;
        cout << "Max ask: " << OrderBook::getHighPrice(entries) << endl;
        cout << "Min ask: " << OrderBook::getLowPrice(entries) << endl;
    }
}

// Allows the user to enter an ask order
void MerkelMain::enterAsk()
{
    cout << "Make an ask - enter the amount: product,price, amount, eg  ETH/BTC,200,0.5" << endl;
    string input;
    getline(cin, input); // Read the input from the user

    vector<string> tokens = CSVReader::tokenise(input, ','); // Tokenize the input string
    if (tokens.size() != 3)                                  // Validate the number of tokens
    {
        cout << "MerkelMain::enterAsk Bad input! " << input << endl; // Display an error message
    }
    else
    {
        try
        {
            // Convert tokens to an OrderBookEntry object
            OrderBookEntry obe = CSVReader::stringsToOBE(tokens[1], tokens[2], currentTime, tokens[0], OrderBookType::ask);
            obe.username = "simuser";        // Set the username for the order
            if (wallet.canFulfillOrder(obe)) // Check if the wallet has sufficient funds
            {
                cout << "Wallet looks good. " << endl;
                orderBook.insertOrder(obe); // Insert the order into the order book
            }
            else
            {
                cout << "Wallet has insufficient funds . " << endl; // Display an error message if funds are insufficient
            }
        }
        catch (const exception &e)
        {
            cout << "MerkelMain::enterAsk Bad input " << endl; // Handle exceptions from input conversion
        }
    }
}

// Allows the user to enter a bid order
void MerkelMain::enterBid()
{
    cout << "Make an bid - enter the amount: product,price, amount, eg  ETH/BTC,200,0.5" << endl;
    string input;
    getline(cin, input);                                     // Read the input from the user
    vector<string> tokens = CSVReader::tokenise(input, ','); // Tokenize the input string
    if (tokens.size() != 3)                                  // Validate the number of tokens
    {
        cout << "MerkelMain::enterBid Bad input! " << input << endl; // Display an error message
    }
    else
    {
        try
        {
            // Convert tokens to an OrderBookEntry object
            OrderBookEntry obe = CSVReader::stringsToOBE(tokens[1], tokens[2], currentTime, tokens[0], OrderBookType::bid);
            obe.username = "simuser"; // Set the username for the order

            if (wallet.canFulfillOrder(obe)) // Check if the wallet has sufficient funds
            {
                cout << "Wallet looks good. " << endl;
                orderBook.insertOrder(obe); // Insert the order into the order book
            }
            else
            {
                cout << "Wallet has insufficient funds . " << endl; // Display an error message if funds are insufficient
            }
        }
        catch (const exception &e)
        {
            cout << "MerkelMain::enterBid Bad input " << endl; // Handle exceptions from input conversion
        }
    }
}

// Prints the current state of the wallet
void MerkelMain::printWallet()
{
    cout << wallet.toString() << endl; // Display wallet information
}

// Advances to the next time frame and processes sales
void MerkelMain::gotoNextTimeframe()
{
    cout << "Going to next time frame. " << endl;
    // Iterate over each known product
    for (string p : orderBook.getKnownProducts())
    {
        cout << "matching " << p << endl;
        // Match asks to bids for the current product
        vector<OrderBookEntry> sales = orderBook.matchAsksToBids(p, currentTime);
        cout << "Sales: " << sales.size() << endl;
        // Process each sale
        for (OrderBookEntry &sale : sales)
        {
            cout << "Sale price: " << sale.price << " amount " << sale.amount << endl;
            if (sale.username == "simuser") // Check if the sale is for the current user
            {
                wallet.processSale(sale); // Update wallet with the sale
            }
        }
    }
    currentTime = orderBook.getNextTime(currentTime); // Advance to the next time frame
}

// Prompts the user for an option and returns the selected option
int MerkelMain::getUserOption()
{
    int userOption = 0;
    string line;
    cout << "Type in 1-6" << endl;
    getline(cin, line); // Read user input
    try
    {
        userOption = stoi(line); // Convert input to integer
    }
    catch (const exception &e)
    {
        // Handle conversion errors silently
    }
    cout << "You chose: " << userOption << endl; // Display the selected option
    return userOption;
}

// Processes the user's menu selection
void MerkelMain::processUserOption(int userOption)
{
    // Handle each menu option
    if (userOption == 0)
    {
        cout << "Invalid choice. Choose 1-6" << endl; // Handle invalid choices
    }
    if (userOption == 1)
    {
        printHelp(); // Print help information
    }
    if (userOption == 2)
    {
        printMarketStats(); // Print market statistics
    }
    if (userOption == 3)
    {
        enterAsk(); // Enter an ask order
    }
    if (userOption == 4)
    {
        enterBid(); // Enter a bid order
    }
    if (userOption == 5)
    {
        printWallet(); // Print wallet information
    }
    if (userOption == 6)
    {
        gotoNextTimeframe(); // Advance to the next time frame
    }
}
