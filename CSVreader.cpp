#include <iostream>
#include <fstream>
#include "CSVReader.h"

using namespace std;

// Default constructor for CSVReader
CSVReader::CSVReader() {}

// Reads a CSV file and converts each line to an OrderBookEntry object
vector<OrderBookEntry> CSVReader::readCSV(string csvFilename)
{
    vector<OrderBookEntry> entries; // Vector to store OrderBookEntry objects
    ifstream csvFile{csvFilename};  // Open CSV file for reading
    string line;
    if (csvFile.is_open()) // Check if file is open
    {
        while (getline(csvFile, line)) // Read each line from the file
        {
            try
            {
                // Tokenize the line and convert tokens to an OrderBookEntry object
                OrderBookEntry obe = stringsToOBE(tokenise(line, ','));
                entries.push_back(obe); // Add the OrderBookEntry to the vector
            }
            catch (const exception &e)
            {
                cout << "CSVReader::readCSV bad data" << endl; // Handle any exceptions during conversion
            }
        }
    }
    cout << entries.size() << " entries" << endl; // Print the number of entries read
    return entries;                               // Return the vector of OrderBookEntry objects
}

// Splits a CSV line into tokens based on the separator character
vector<string> CSVReader::tokenise(string csvLine, char separator)
{
    vector<string> tokens; // Vector to store the split tokens
    signed int start, end;
    string token;
    start = csvLine.find_first_not_of(separator, 0); // Find the start of the first token
    do
    {
        end = csvLine.find_first_of(separator, start); // Find the end of the current token
        if (start == csvLine.length() || start == end) // Check if end of line or token
            break;
        // Extract the token from the line
        if (end >= 0)
            token = csvLine.substr(start, end - start);
        else
            token = csvLine.substr(start, csvLine.length() - start);
        tokens.push_back(token); // Add the token to the vector
        start = end + 1;         // Update start for the next token
    } while (end > 0);
    return tokens; // Return the vector of tokens
}

// Converts a vector of strings to an OrderBookEntry object
OrderBookEntry CSVReader::stringsToOBE(vector<string> tokens)
{
    double price, amount;
    if (tokens.size() != 5)
    {
        cout << "Bad line " << endl; // Check if the number of tokens is correct
        throw exception{};           // Throw an exception if the line is invalid
    }
    try
    {
        // Convert string tokens to double values
        price = stod(tokens[3]);
        amount = stod(tokens[4]);
    }
    catch (const exception &e)
    {
        cout << "CSVReader::stringsToOBE Bad float! " << tokens[3] << endl;
        cout << "CSVReader::stringsToOBE Bad float! " << tokens[4] << endl; // Handle any conversion errors
        throw;
    }
    // Create and return an OrderBookEntry object from the tokens
    OrderBookEntry obe{price, amount, tokens[0], tokens[1], OrderBookEntry::stringToOrderBookType(tokens[2])};
    return obe;
}

// Overloaded function to convert individual strings and an OrderBookType to an OrderBookEntry
OrderBookEntry CSVReader::stringsToOBE(string priceString, string amountString, string timestamp, string product, OrderBookType orderType)
{
    double price, amount;
    try
    {
        // Convert string values to double
        price = stod(priceString);
        amount = stod(amountString);
    }
    catch (const exception &e)
    {
        cout << "CSVReader::stringsToOBE Bad float! " << priceString << endl;
        cout << "CSVReader::stringsToOBE Bad float! " << amountString << endl; // Handle conversion errors
        throw;
    }
    // Create and return an OrderBookEntry object from the parameters
    OrderBookEntry obe{price, amount, timestamp, product, orderType};
    return obe;
}
