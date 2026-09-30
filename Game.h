#ifndef GAME_H
#define GAME_H

#include <vector>

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

    // Status
    int lives;
    int timeRemaining;   
    bool gameOver;

    // Statistics
    int treasuresCollected;
    int treasuresRequired;
    int enemiesAvoided;

    // Player effects
    bool speedBoost;
    int speedBoostTurns;

    // Entities
    Vec2 player;
    Vec2 enemy;

    std::vector<Vec2> treasures;
    std::vector<Vec2> obstacles;

    PowerUp powerUp;

private:
    //Main loop
    void draw();
    void drawHUD();
    void update();
    void handleInput();

    // Level
    void resetLevel();
    void nextLevel();

    // Generation
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

    // Enemy
    void moveEnemy();
    void playerCaught();

    // Timer
    void updateTimer();

    // Movement
    bool tryMovePlayer(char input);
    bool tryMovePlayerDirection(int dx, int dy);

    // Collision
    bool isObstacle(const Vec2& pos) const;
    bool isTreasure(const Vec2& pos) const;
    bool isPositionOccupied(const Vec2& pos) const;
    bool isInsideBounds(const Vec2& pos) const;

    // Utilities
    Vec2 randomPosition();
    void clearScreen();
    void waitForEnter();

    // Score
    void loadHighScore();
    void saveHighScore();
    void updateHighScore();

    // Stats
    void resetStatistics();

    // End game
    void drawGameOverScreen();
};

#endif