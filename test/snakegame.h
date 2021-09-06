#pragma once

#include "SACore/structs.h"
#include "SAGui/widget.h"

class SnakeGame : public SA::Widget
{
public:
    SnakeGame(SA::Widget *parent = nullptr);
    ~SnakeGame();

    void keyboardEvent(const SA::KeyEvent &event);

    void timerEvent(int id);
    void paintEvent();
    void resizeEvent(const SA::Size &size);

private:

    void setGameOver();
    bool isTailHited(int x, int y);
    void addFoodPoint();
    void updateSnakePos();
    void updateGeometry();

    struct SnakeGamePrivate;
    SnakeGamePrivate * const d;
};
