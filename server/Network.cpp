#include "Network.hpp"

#include <boost/asio/write.hpp>

#include "common/Network/Types/ServerMessage.pb.h"
#include "Consts.hpp"

#include <ncurses.h>

using namespace boost::asio;

static Player mapPlayer(unsigned player, unsigned receiver)
{
    return player == receiver ? Player::Me : Player::Enemy;
}

Network::Network(boost::asio::ip::tcp::socket & p1, boost::asio::ip::tcp::socket & p2):
    player1(p1),
    player2(p2)
{
}

void Network::sendDeckSize(unsigned player, unsigned size)
{
    ServerMessage msg;
    auto deckSize = msg.mutable_decksize();

    deckSize->set_player(mapPlayer(player, P1));
    deckSize->set_size(size);
    sendMessage(msg, P1);

    deckSize->set_player(mapPlayer(player, P2));
    sendMessage(msg, P2);
}

void Network::sendMessage(const google::protobuf::Message & msg, unsigned player)
{
    boost::system::error_code err;

    std::string serialized = msg.SerializeAsString();
    ip::tcp::socket & s = player == P1 ? player1 : player2;
    write(s, buffer(serialized), err);
}
