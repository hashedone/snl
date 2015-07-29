#pragma once

#include <boost/asio/ip/tcp.hpp>

class Client
{
public:
    Client(boost::asio::ip::tcp::socket & s);

    void connected();

private:
    boost::asio::ip::tcp::socket & socket;
};
