#include "Gui.hpp"

#include <functional>
#include <boost/asio/read.hpp>

static const int
    topBarCP = 1;

Gui::Gui(boost::asio::io_service & ios):
    ioService(ios),
    signalSet(ios, SIGWINCH),
    timer(ios)
{ 
    namespace ph = std::placeholders;

    signalSet.async_wait(std::bind(&Gui::signalHandler, this, ph::_1, ph::_2));

    int w, h;

    initscr();
    getmaxyx(stdscr, h, w);
    cbreak();
    nodelay(stdscr, true);
    noecho();
    keypad(stdscr, true);
    curs_set(0);
    start_color();

    init_pair(topBarCP, COLOR_BLACK, COLOR_MAGENTA);

    recreateWindows();

    timer.expires_from_now(boost::posix_time::milliseconds(100));
    timer.async_wait(std::bind(&Gui::timerHandler, this, ph::_1));
}

Gui::~Gui()
{
    endwin();
}

void Gui::drawFrame()
{
    refresh();
    wrefresh(mwin);
}

void Gui::updateStdWindow()
{
    int w, h;
    getmaxyx(stdscr, h, w);

    attron(COLOR_PAIR(topBarCP));
    move(0, 0);
    hline(' ', w);
    mvaddstr(0, 4, "Swords and lances");
    attroff(COLOR_PAIR(topBarCP));
}

void Gui::recreateWindows()
{
    updateStdWindow();

    int w, h;
    getmaxyx(stdscr, h, w);

    int y = 0;
    if(h % 2)
    {
        --h;
        ++y;
    }

    mwin = newwin(h, w, y, 0);
}

void Gui::signalHandler(boost::system::error_code err, int sig)
{
    namespace ph = std::placeholders;

    if(err == boost::asio::error::operation_aborted)
        return;

    switch(sig)
    {
    case SIGWINCH:
        recreateWindows();
        break;
    case SIGINT:
        ioService.stop();
        return;
    default:
        break;
    }

    signalSet.async_wait(std::bind(&Gui::signalHandler, this, ph::_1, ph::_2));
}

void Gui::timerHandler(boost::system::error_code err)
{
    namespace ph = std::placeholders;

    if(err == boost::asio::error::operation_aborted)
        return;

    handleInput();

    timer.expires_from_now(boost::posix_time::milliseconds(100));
    timer.async_wait(std::bind(&Gui::timerHandler, this, ph::_1));
}

void Gui::handleInput()
{
    int k = getch();
    switch(k)
    {
    case ERR:
        return;
    case KEY_RESIZE:
        recreateWindows();
        return;
    case KEY_F(12):
        ioService.stop();
        return;
    }
}
