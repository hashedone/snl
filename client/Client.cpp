#include "Client.hpp"

Client::Client(boost::asio::ip::tcp::socket & s):
    socket(s)
{ }

void Client::connected()
{
}

