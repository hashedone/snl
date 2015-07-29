#include <boost/asio/io_service.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/asio/connect.hpp>
#include <ncurses.h>
#include <sstream>

#include "gui/Gui.hpp"
#include "server/Server.hpp"
#include "client/Client.hpp"

using namespace boost::asio;

void acceptPlayers(
    const boost::system::error_code &err,
    ip::tcp::acceptor & acceptor,
    ip::tcp::socket & connection,
    std::function<void()> cb
)
{
    namespace ph = std::placeholders;

    static unsigned playersConnected = 0;
    if(err)
    {
        acceptor.async_accept(connection, std::bind(
            acceptPlayers,
            ph::_1,
            std::ref(acceptor),
            std::ref(connection),
            cb
        ));
        return;
    }

    if(playersConnected)
        cb();
    else
        playersConnected = 1;
}

int main()
{
    namespace ph = std::placeholders;

    io_service ioService;

    ip::tcp::acceptor acceptor(ioService, ip::tcp::endpoint(ip::tcp::v4(), 1441));
    boost::asio::ip::tcp::socket server1(ioService), server2(ioService);
    Server server(server1, server2);

    acceptor.async_accept(server1, [&acceptor, &server1, &server](const boost::system::error_code & err)
    {
        acceptPlayers(err, acceptor, server1, std::bind(&Server::playersConnected, &server));
    });
    acceptor.async_accept(server2, [&acceptor, &server2, &server](const boost::system::error_code & err)
    {
        acceptPlayers(err, acceptor, server2, std::bind(&Server::playersConnected, &server));
    });

    ip::tcp::resolver resolver(ioService);
    ip::tcp::resolver::query q("localhost", "1441");
    
    auto endpointIt = resolver.resolve(q);
    ip::tcp::socket client1(ioService), client2(ioService);
    Client player1(client1), player2(client2);
    boost::asio::connect(client1, endpointIt);
    player1.connected();
    boost::asio::connect(client2, endpointIt);
    player2.connected();

    Gui gui(ioService);

    gui.drawFrame();

    ioService.run();

    getch();
    return 0;
}
