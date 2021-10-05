#include <iostream>
#include <cstdlib>
#include <vector>
#include <ctime>
#include <tuple>

#include "snakegame.h"

enum class Direction
{
    Right,
    Left,
    Up,
    Down
};

struct Point
{
    int x, y;
    Point(int _x, int _y): x(_x), y(_y){}
};

struct Rect
{
    int x, y, width, height;
    Rect(int _x, int _y, int _w, int _h):
        x(_x), y(_y), width(_w), height(_h){}
};

struct SnakeGame::SnakeGamePrivate
{
    int timerId = 0;
    int cellsCount = 16;
    bool gameOver = false;
    Rect gameRect = Rect(0, 0, 400, 400);
    Point foodPoint = Point(3, 4);
    Direction dirCurrent = Direction::Left;
    Direction dirNext = Direction::Left;

    std::vector<Point> snake = { Point(0,7), Point(1,7), Point(2,7), Point(3,7)};
};

SnakeGame::SnakeGame(Widget *parent) : SA::Widget(parent),
    d(new SnakeGamePrivate)
{
    std::srand(std::time(nullptr));

    setTitle("Snake game");

    d->timerId = startTimer(150);

    std::cout << __PRETTY_FUNCTION__ << std::endl;
}

SnakeGame::~SnakeGame()
{
    std::cout << __PRETTY_FUNCTION__ << std::endl;
    delete d;
}

void SnakeGame::keyboardEvent(const SA::KeyEvent &event)
{
    if (!event.pressed) return;

    if (d->dirCurrent == Direction::Left || d->dirCurrent == Direction::Right)
    {
        if (event.keycode == SA::Key_Up || event.keycode == SA::Key_W) d->dirNext = Direction::Up;
        if (event.keycode == SA::Key_Down || event.keycode == SA::Key_S) d->dirNext = Direction::Down;
    }
    else if (d->dirCurrent == Direction::Up || d->dirCurrent == Direction::Down)
    {
        if (event.keycode == SA::Key_Left || event.keycode == SA::Key_A) d->dirNext = Direction::Left;
        if (event.keycode == SA::Key_Right || event.keycode == SA::Key_D) d->dirNext = Direction::Right;
    }

//    std::cout << __PRETTY_FUNCTION__ << " keyCode: " << std::hex << keyCode << std::endl;
}

void SnakeGame::timerEvent(int id)
{
    if (d->timerId == id)
        updateSnakePos();
}

void SnakeGame::paintEvent()
{
    setPen(30, 30, 30, 1);
    setBrush(30, 30, 30);
    drawRect(0, 0, width(), height());

    setPen(20, 20, 20, 2);
    setBrush(50, 50, 50);
    drawRect(d->gameRect.x, d->gameRect.y, d->gameRect.width, d->gameRect.height);

    setPen(50, 50, 50, 1);
    int cellWidth = d->gameRect.width / d->cellsCount;
    int cellHeight = d->gameRect.height / d->cellsCount;

    setBrush(200, 80, 80);
    for (size_t i=0; i<d->snake.size(); ++i)
    {
        drawRect(d->snake[i].x * cellWidth + d->gameRect.x,
                 d->snake[i].y * cellHeight + d->gameRect.y,
                 cellWidth, cellHeight);
    }

    setBrush(90, 160, 90);
    drawRect(d->foodPoint.x * cellWidth + d->gameRect.x,
             d->foodPoint.y * cellHeight + d->gameRect.y,
             cellWidth, cellHeight);

    setPen(230, 160, 90, 1);

    if (d->gameOver)
    {
        std::string gameOver = "GAME OVER";
        drawText(width() / 2 - textWidth(gameOver) / 2,
                 height() / 2 - textHeight() / 2,
                 gameOver);
    }

    drawText(d->gameRect.x + 6, d->gameRect.y + 3,
             "Score: " + std::to_string(d->snake.size() - 4));
}

void SnakeGame::resizeEvent(const SA::Size &size)
{
    std::ignore = size;
    updateGeometry();
}

void SnakeGame::setGameOver()
{
    killTimer(d->timerId);
    d->gameOver = true;
}

bool SnakeGame::isTailHited(int x, int y)
{
    int size = d->snake.size();
    if (size < 2) return false;

    for (int i=1; i<size; ++i)
        if (d->snake[i].x == x && d->snake[i].y == y)
            return true;

    return false;
}

void SnakeGame::addFoodPoint()
{
    int x, y;
    for (int i=0; i<1000; ++i)
    {
        x = std::rand()/((RAND_MAX + 1u)/d->cellsCount);
        y = std::rand()/((RAND_MAX + 1u)/d->cellsCount);

        if (isTailHited(x, y)) continue;
        if (x == d->foodPoint.x && y == d->foodPoint.y) continue;

        break;
    }

    d->foodPoint = Point(x, y);
}

void SnakeGame::updateSnakePos()
{
    int size = d->snake.size();
    if (size < 2) return;

    if (d->dirCurrent != d->dirNext)
        d->dirCurrent = d->dirNext;

    for (int i=size-1; i>=1; --i)
    {
        d->snake[i].x = d->snake[i-1].x;
        d->snake[i].y = d->snake[i-1].y;
    }

    Point head = d->snake[0];

    switch (d->dirCurrent)
    {
    case Direction::Right: ++head.x; break;
    case Direction::Left: --head.x; break;
    case Direction::Down: ++head.y; break;
    case Direction::Up: --head.y; break;
    }

    if (head.x >= d->cellsCount) head.x = 0;
    else if (head.x < 0) head.x = d->cellsCount - 1;

    if (head.y >= d->cellsCount) head.y = 0;
    else if (head.y < 0) head.y = d->cellsCount - 1;

    d->snake[0].x = head.x;
    d->snake[0].y = head.y;

    if (isTailHited(head.x, head.y))
        setGameOver();

    if (head.x == d->foodPoint.x && head.y == d->foodPoint.y)
    {
        d->snake.push_back(d->snake[size-1]);
        addFoodPoint();
    }

    update();
}

void SnakeGame::updateGeometry()
{
    d->gameRect = Rect(width() / 2 - d->gameRect.width / 2,
                       height() / 2 - d->gameRect.height / 2,
                       d->gameRect.width, d->gameRect.height);

    update();
}
