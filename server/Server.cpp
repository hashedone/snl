#include "Server.hpp"

#include <algorithm>

#include "Consts.hpp"

using namespace boost::asio;

Server::Server(boost::asio::ip::tcp::socket & s1, boost::asio::ip::tcp::socket & s2):
    network(s1, s2)
{ }

void Server::playersConnected()
{
    std::fill_n(std::inserter(player1.library, player1.library.begin()), 40, 1);
    std::fill_n(std::inserter(player1.library, player1.library.begin()), 40, 1);

    network.sendDeckSize(P1, player1.library.size());
    network.sendDeckSize(P1, player2.library.size());
}
