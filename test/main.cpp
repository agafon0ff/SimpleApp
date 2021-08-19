#include "application.h"
#include "snakegame.h"
#include "controls.h"

int main()
{
//    SnakeGame sg;
//    sg.resize(500, 500);
//    sg.show();

    Controls tt;
    tt.setGeometry(1500, 500 ,300, 200);
    tt.show();

    return SA::Application::instance().exec();
}
