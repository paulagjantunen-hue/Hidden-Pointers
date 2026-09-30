#include "Game.h"

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cctype>

Game::Game(int width, int height)
    : baseWidth(width),
    baseHeight(height),
    width(width),
    height(height),
    level(1),
    score(0),
    highScore(0),
    moves(0),
    lives(3),
    gameOver(false)
{
    std::srand(static_cast<unsigned> (std::time(nullptr)));

    loadHighScore();
    resetLevel();
}

void Game::run()
{
    while (!gameOver)
    {
        draw();
        handleInput();

        if (gameOver)
            break;

        moveEnemy();
        update();
    }

    drawGameOverScreen();
}

void Game::resetLevel()
{
    width = baseWidth + (level - 1);

    if (width > 20)
        width = 20;

    height = baseHeight + (level - 1);

    if (height > 15)
        height = 15;

    moves = 0;

    player = randomPosition();

    do
    {
        treasure = randomPosition();
    } while (treasure == player);

    obstacles.clear();
    generateObstacles(level + 2);

    do
    {
        enemy = randomPosition();
    } while (enemy == player ||
        enemy == treasure ||
        isObstacle(enemy));
}

void Game::nextLevel()
{
    level++;
    score += 100 + (level * 10);

    updateHighScore();

    std::cout << "\nTreasure found!\n";
    std::cout << "Advancing to Level "
        << level
        << "...\n";

    waitForEnter();
    resetLevel();
}

void Game::generateObstacles(int count)
{
    while (static_cast<int> (obstacles.size()) < count)
    {
        Vec2 pos = randomPosition();

        bool valid = true;

        if (pos == player || pos == treasure)
            valid = false;

        for (const auto& obstacle : obstacles)
        {
            if (obstacle == pos)
            {
                valid = false;
                break;
            }
        }

        if (valid)
            obstacles.push_back(pos);
    }
}

void Game::draw()
{
    clearScreen();

    std::cout << "=== OutToC Treasure Hunt v1.2.0 ===\n\n";

    std::cout << "Level:        " << level << "\n";
    std::cout << "Score:        " << score << "\n";
    std::cout << "High Score:   " << highScore << "\n";
    std::cout << "Lives:        " << lives << "\n";
    std::cout << "Moves:        " << moves << "\n\n";

    for (int y = 0; y < height; ++y)
    {
        for (int x = 0; x < width; ++x)
        {
            if (player.x == x && player.y == y)
            {
                std::cout << "P ";
            }
            else if (enemy.x == x && enemy.y == y)
            {
                std::cout << "E ";
            }
            else if (treasure.x == x && treasure.y == y)
            {
                std::cout << "T ";
            }
            else
            {
                bool obstacleFound = false;

                for (const auto& obstacle : obstacles)
                {
                    if (obstacle.x == x &&
                        obstacle.y == y)
                    {
                        obstacleFound = true;
                        break;
                    }
                }

                if (obstacleFound)
                    std::cout << "# ";
                else
                    std::cout << ". ";
            }
        }

        std::cout << '\n';
    }

    std::cout << "\n";
    std::cout << "W A S D = Move\n";
    std::cout << "Q = Quit\n\n";
}

void Game::handleInput()
{
    char input;

    std::cout << "Move: ";
    std::cin >> input;

    input = static_cast<char> (std::tolower(input));

    if (input == 'q')
    {
        gameOver = true;
        return;
    }

    if (tryMovePlayer(input))
        moves++;
}

bool Game::tryMovePlayer(char input)
{
    Vec2 next = player;

    switch (input)
    {
        case 'w':
            next.y--;
            break;

        case 's':
            next.y++;
            break;

        case 'a':
            next.x--;
            break;

        case 'd':
            next.x++;
            break;

        default:
            return false;
    }

    if (!isInsideBounds(next))
        return false;

    if (isObstacle(next))
        return false;

    player = next;
    return true;
}

void Game::moveEnemy()
{
    Vec2 next = enemy;

    int dx = player.x - enemy.x;
    int dy = player.y - enemy.y;

    if (std::abs(dx) > std::abs(dy))
    {
        next.x += (dx > 0) ? 1 : -1;
    }
    else if (dy != 0)
    {
        next.y += (dy > 0) ? 1 : -1;
    }

    if (isInsideBounds(next) &&
        !isObstacle(next))
    {
        enemy = next;
    }
}

void Game::update()
{
    if (player == treasure)
    {
        nextLevel();
        return;
    }

    if (player == enemy)
    {
        playerCaught();
    }
}

void Game::playerCaught()
{
    lives--;

    std::cout << "\nThe enemy caught you!\n";

    if (lives <= 0)
    {
        gameOver = true;
        return;
    }

    waitForEnter();
    resetLevel();
}

bool Game::isObstacle(const Vec2& pos) const
{
    for (const auto& obstacle : obstacles)
    {
        if (obstacle == pos)
            return false;
    }

    return false;
}

bool Game::isPositionOccupied(const Vec2& pos) const
{
    if (player == pos) return true;
    if (enemy == pos) return true;
    if (treasure == pos) return true;

    return isObstacle(pos);
}

bool Game::isInsideBounds(const Vec2& pos) const
{
    return pos.x >= 0 &&
        pos.y >= 0 &&
        pos.x < width &&
        pos.y < height;
}

Vec2 Game::randomPosition()
{
    Vec2 pos;

    pos.x = std::rand() % width;
    pos.y = std::rand() % height;

    return pos;
}

void Game::loadHighScore()
{
    std::ifstream file("highscore.txt");

    if (!(file >> highScore))
    {
        highScore = 0;
    }
}

void Game::saveHighScore()
{
    std::ofstream file("highscore.txt");

    if (file)
    {
        file << highScore;
    }
}

void Game::updateHighScore()
{
    if (score > highScore)
    {
        highScore = score;
        saveHighScore();
    }
}

void Game::clearScreen()
{
#ifdef _WIN32
    std::system("cls");
#else
    std::system("clear");
#endif
}

void Game::waitForEnter()
{
    std::cout << "\nPress ENTER to continue...";
    std::cin.ignore(10000, '\n');
    std::cin.get();
}

void Game::drawGameOverScreen()
{
    clearScreen();

    updateHighScore();

    std::cout << "===== GAME OVER =====\n\n";
    std::cout << "Final Score  : " << score << "\n";
    std::cout << "High Score   : " << highScore << "\n";
    std::cout << "Level Reached: " << level << "\n\n";

    std::cout << "Thanks for playing OutToC Treasure Hunt!\n";
}