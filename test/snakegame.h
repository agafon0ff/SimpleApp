#pragma once

#include "SAGui/widget.h"

class SnakeGame : public SA::Widget
{
public:
    SnakeGame(SA::Widget *parent = nullptr);
    ~SnakeGame();

    void keyPressEvent(bool state, unsigned int keyCode);

    void timerEvent(int id);
    void paintEvent();
    void resizeEvent(int width, int height);

private:

    void setGameOver();
    bool isTailHited(int x, int y);
    void addFoodPoint();
    void updateSnakePos();
    void updateGeometry();

    struct SnakeGamePrivate;
    SnakeGamePrivate * const d;
};
