#pragma once

#include <vector>
#include <string>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/buffer.hpp>
#include <google/protobuf/message.h>

class Network
{
public:
    Network(boost::asio::ip::tcp::socket & p1, boost::asio::ip::tcp::socket & p2);

    void sendDeckSize(unsigned player, unsigned size);
private:
    void sendMessage(const google::protobuf::Message & msg, unsigned player);

    boost::asio::ip::tcp::socket & player1;
    boost::asio::ip::tcp::socket & player2;
};
