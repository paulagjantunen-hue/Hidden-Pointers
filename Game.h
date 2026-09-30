#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>

struct Vec2
{
    int x;
    int y;

    bool operator==(const Vec2& other) const
    {
        return x == other.x &&
            y == other.y;
    }
};

struct PowerUp
{
    Vec2 position;
    bool active;

    PowerUp()
    {
        position = {0, 0};
        active = false;
    }
};

class Game
{
public:
    Game(int width, int height);
    void run();

private:
    int baseWidth;
    int baseHeight;
    int width;
    int height;

    int level;
    int score;
    int highScore;
    int moves;
    int lives;

    bool gameOver;

    Vec2 player;
    Vec2 enemy;
    Vec2 treasure;

    std::vector<Vec2> obstacles;

    void draw();
    void update();
    void handleInput();

    void resetLevel();
    void nextLevel();
    void generateObstacles(int count);

    void moveEnemy();
    bool tryMovePlayer(char input);

    bool isObstacle(const Vec2& pos) const;
    bool isPositionOccupied(const Vec2& pos) const;
    bool isInsideBounds(const Vec2& pos) const;

    Vec2 randomPosition();
    void clearScreen();
    void waitForEnter();

    void loadHighScore();
    void saveHighScore();
    void updateHighScore();

    void playerCaught();
    void drawGameOverScreen();
};

#endif