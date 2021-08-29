#include "application.h"
#include "snakegame.h"
#include "controls.h"
#include <csignal>

void signalHandler(int)
{
   SA::Application::instance().quit(0);
}

int main()
{
    signal(SIGTERM, signalHandler);
    signal(SIGINT, signalHandler);

#ifdef __linux__
    signal(SIGKILL, signalHandler);
    signal(SIGQUIT, signalHandler);
#endif // Q_OS_WIN

    SnakeGame sg;
    sg.resize(500, 500);
    sg.show();

//    Controls tt;
//    tt.setTitle("Controls");
//    tt.setGeometry(500, 200 , 600, 600);
//    tt.show();

    return SA::Application::instance().exec();
}
