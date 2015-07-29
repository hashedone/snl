#pragma once

#include <boost/asio/ip/tcp.hpp>

#include "common/PlayerArea.hpp"
#include "Network.hpp"

class Server
{
public:
    Server(boost::asio::ip::tcp::socket & s1, boost::asio::ip::tcp::socket & s2);

    void playersConnected();

private:
    PlayerArea player1, player2;

    Network network;
};
