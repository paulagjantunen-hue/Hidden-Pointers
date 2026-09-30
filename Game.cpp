#include "Game.h"

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>

Game::Game(int w, int h)
    : baseWidth(w),
      baseHeight(h),
      width(w),
      height(h),
      level(1),
      score(0),
      highScore(0),
      moves(0),
      lives(3),
      timeRemaining(60),
      gameOver(false),
      treasuresCollected(0),
      treasuresRequired(3),
      enemiesAvoided(0),
      speedBoost(false),
      speedBoostTurns(0)
{
    std::srand(static_cast<unsigned> (std::time(nullptr)));

    loadHighScore();
    resetStatistics();
    resetLevel();
}

void Game::run()
{
    while (!gameOver)
    {
        draw();
        handleInput();
        update();
    }

    drawGameOverScreen();
}

void Game::draw()
{
    clearScreen();

    drawHUD();

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            Vec2 pos{x, y};

            if (player == pos)
            {
                std::cout << "P ";
            }
            else if (enemy == pos)
            {
                std::cout << "E ";
            }
            else if (powerUp.active && powerUp.position == pos)
            {
                std::cout << "* ";
            }
            else if (isTreasure(pos))
            {
                std::cout << "T ";
            }
            else if (isObstacle(pos))
            {
                std::cout << "# ";
            }
            else
            {
                std::cout << ". ";
            }
        }

        std::cout << '\n';
    }

    std::cout << "\nW A S D = Move\n";
    std::cout << "Q = Quit\n";
}

void Game::drawHUD()
{
    std::cout << "=== OutToC Treasure Hunt v1.4 ===\n";

    std::cout << "Level: " << level
              << " | Score: " << score
              << " | High Score: " << highScore
              << '\n';

    std::cout << "Lives: " << lives
              << " | Time: " << timeRemaining
              << " | Treasures: "
              << treasuresCollected << "/"
              << treasuresRequired
              << '\n';

    if (speedBoost)
    {
        std::cout << "Speed Boost Active ("
                  << speedBoostTurns
                  << " turns)\n";
    }

    std::cout << '\n';
}

void Game::handleInput()
{
    char input;

    std::cout << "\nMove: ";
    std::cin >> input;

    input = std::tolower(input);

    if (input == 'q')
    {
        gameOver = true;
        return;
    }

    if (speedBoost)
    {
        tryMovePlayer(input);
        tryMovePlayer(input);
    }
    else
    {
        tryMovePlayer(input);
    }

    moves++;
}

bool Game::tryMovePlayer(char input)
{
    switch (input)
    {
        case 'w':
            return tryMovePlayerDirection(0, -1);

        case 's':
            return tryMovePlayerDirection(0, 1);

        case 'a':
            return tryMovePlayerDirection(-1, 0);

        case 'd':
            return tryMovePlayerDirection(1, 0);

        default:
            return false;
    }
}

bool Game::tryMovePlayerDirection(int dx, int dy)
{
    Vec2 next = player;

    next.x += dx;
    next.y += dy;

    if (!isInsideBounds(next))
        return false;

    if (isObstacle(next))
        return false;

    player = next;
    return true;
}

void Game::update()
{
    collectTreasure();
    collectPowerUp();

    moveEnemy();

    if (enemy == player)
    {
        playerCaught();
    }

    updatePowerUps();
    updateTimer();

    if (allTreasuresCollected())
    {
        nextLevel();
    }

    updateHighScore();
}

bool Game::isInsideBounds(const Vec2& pos) const
{
    return pos.x >= 0 &&
           pos.y >= 0 &&
           pos.x < width &&
           pos.y < height;
}

bool Game::isObstacle(const Vec2& pos) const
{
    return std::find(
        obstacles.begin(),
        obstacles.end(),
        pos
    ) != obstacles.end();
}

bool Game::isTreasure(const Vec2& pos) const
{
    return std::find(
        treasures.begin(),
        treasures.end(),
        pos
    ) != treasures.end();
}

bool Game::isPositionOccupied(const Vec2& pos) const
{
    if (player == pos) return true;
    if (enemy == pos) return true;

    if (isObstacle(pos))
        return true;

    if (isTreasure(pos))
        return true;

    return false;
}

Vec2 Game::randomPosition()
{
    return {
        std::rand() % width,
        std::rand() % height
    };
}