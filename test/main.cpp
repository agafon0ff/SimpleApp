#include <application.h>
#include "snakegame.h"

int main()
{
    SnakeGame sg;
    sg.resize(500, 500);
    sg.show();

    return SA::Application::instance().exec();
}
