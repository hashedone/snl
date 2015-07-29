#pragma once

#include <ncurses.h>
#include <boost/asio/io_service.hpp>
#include <boost/asio/signal_set.hpp>
#include <boost/asio/deadline_timer.hpp>

class Gui
{
public:
    Gui(boost::asio::io_service & ios);
    ~Gui();
    
    void drawFrame();
private:
    void updateStdWindow(); // Redraw all std screen
    void recreateWindows(); // Called when windows size changed

    void signalHandler(boost::system::error_code err, int sig);
    void timerHandler(boost::system::error_code err); 

    void handleInput();

    WINDOW * mwin;
    boost::asio::io_service & ioService;
    boost::asio::signal_set signalSet;
    boost::asio::deadline_timer timer;
};

