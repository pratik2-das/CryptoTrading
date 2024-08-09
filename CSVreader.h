#pragma once

#include <vector>
#include <string>
#include "OrderBookEntry.h"

using namespace std;

// CSVReader class declaration
class CSVReader
{
public:
    // Default constructor
    CSVReader();

    // Static method to read a CSV file and return a vector of OrderBookEntry objects
    static vector<OrderBookEntry> readCSV(string csvFile);

    // Static method to split a CSV line into tokens based on a separator character
    static vector<string> tokenise(string csvLine, char separator);

    // Static method to convert individual string values and an OrderBookType to an OrderBookEntry object
    static OrderBookEntry stringsToOBE(string price,
                                       string amount,
                                       string timestamp,
                                       string product,
                                       OrderBookType OrderBookType);

private:
    // Static method to convert a vector of string tokens to an OrderBookEntry object
    static OrderBookEntry stringsToOBE(vector<string> strings);
};
