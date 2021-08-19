#include "application.h"
#include "snakegame.h"
#include "texttest.h"

int main()
{
    SnakeGame sg;
    sg.resize(500, 500);
    sg.show();

//    TextTest tt;
//    tt.resize(300, 200);
//    tt.show();

    return SA::Application::instance().exec();
}
