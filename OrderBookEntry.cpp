#include "OrderBookEntry.h"

// Constructor that initializes an OrderBookEntry with provided values
OrderBookEntry::OrderBookEntry(double _price, double _amount, std::string _timestamp, std::string _product, OrderBookType _orderType, std::string _username)
    : price(_price), amount(_amount), timestamp(_timestamp), product(_product), orderType(_orderType), username(_username) {}

// Converts a string to the corresponding OrderBookType enumeration
OrderBookType OrderBookEntry::stringToOrderBookType(std::string s)
{
  if (s == "ask")
  {
    return OrderBookType::ask; // Convert "ask" to OrderBookType::ask
  }
  if (s == "bid")
  {
    return OrderBookType::bid; // Convert "bid" to OrderBookType::bid
  }
  return OrderBookType::unknown; // Return unknown for any other string
}
