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
    int duration;

    PowerUp()
        : active(false),
          duration(0)
    {
        position = {0, 0};
    }
};

class Game
{
public:
    Game(int width, int height);
    void run();

private:
    // Map
    int baseWidth;
    int baseHeight;
    int width;
    int height;

    // Progression
    int level;
    int score;
    int highScore;
    int moves;

    // Player state
    int lives;   
    bool gameOver;

    // Statistics
    int treasuresCollected;
    int treasuresRequired;
    int enemiesAvoided;

    // Timer
    int timeRemaining;

    // Power-ups
    bool speedBoost;
    int speedBoostTurns;
    PowerUp powerUp;

    // Entities
    Vec2 player;
    Vec2 enemy;

    // Multiple treasures (v1.4.0)
    std::vector<Vec2> obstacles;

private:
    //Main loop
    void draw();
    void update();
    void handleInput();

    // Level control
    void resetLevel();
    void nextLevel();
    void generateMaze();
    void generateObstacles(int count);

    // Treasure system
    void spawnTreasures();
    void collectTreasure();
    bool allTreasuresCollected() const;

    // Power-ups
    void spawnPowerUp();
    void collectPowerUp();
    void updatePowerUps();

    // Enemy AI
    void moveEnemy();
    void playerCaught();

    // Timer
    void updateTimer();

    // Movement
    bool tryMovePlayer(char input);
    bool tryMovePlayerDirection(int dx, int dy);

    // Collision checks
    bool isObstacle(const Vec2& pos) const;
    bool isTreasure(const Vec2& pos) const;
    bool isPositionOccupied(const Vec2& pos) const;
    bool isInsideBounds(const Vec2& pos) const;

    // Utility
    Vec2 randomPosition();
    void clearScreen();
    void waitForEnter();

    // Score management
    void loadHighScore();
    void saveHighScore();
    void updateHighScore();

    // Statistics
    void resetStatistics();

    // UI
    void drawHUD();
    void drawGameOverScreen();
};

#endif // GAME_H