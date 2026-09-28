// Simple console treasure hunt game in C++
// Build: g++ OutToC_Treasure.cpp -o OutToC_Treasure

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <cctype>

struct Vec2 {
    int x;
    int y;
};

class Game {
public:
    Game(int w, int h)
        : baseWidth(w), baseHeight(h),
          width(w), haight(h),
          moves(0), score(0),
          level(1), gameOver(false) {
        std::srand(static_cast<unsigned>(std::time(nullptr)));
        reset();
    }

    void run() {
        while (!gameOver) {
            draw();
            handleInput();
            update();
        }
        drawGameOver();
    }

private:
    int baseWidth;
    int baseHeight;
    int width;
    int height;
    int moves;
    int score;
    int level;
    bool gameOver;

    Vec2 player;
    Vec2 treasure;
    std::vector<Vec2> obstacles;

    void reset() {
        moves = 0;
        
        // Increase grid size slightly with level (capped)
        width  = baseWidth  + (level - 1);
        height = baseHeight + (level - 1);
        if (width > 20) width = 20;
        if (height > 15) height = 15;

        player   = randomPos();
        treasure = randomPos();

        // Ensure treasure not on player
        while (treasure.x == player.x && treasure.y == player.y) {
            treasure = randomPos();
        }

        // Generate obstacles
        obstacles.clear();
        int obstacleCount = 2 + level; // more obstacles each level
        for (int i = 0; i < obstacleCount; ++i) {
            Vec2 o = randomPos();
            // Avoid player, treasure, and duplicates
            while (isBlocked(o) || (o.x == player.x && o.y == player.y) ||
                    (o.x == treasure.x && o.y == treasure.y)) {
                o = randomPos();
            }
            obstacles.push_back(o);
        }
    }

    Vec2 randomPos() {
        Vec2 v;
        v.x = std::rand() % width;
        v.y = std::rand() % height;
        return v;
    }

    bool isObstacle(int x, int y) const {
        for (const auto &o : obstacles) {
            if (o.x == x && o.y == y) return true;
        }
        return false;
    }

    bool isBlocked(const Vec2 &pos) const {
        return isObstacle(pos.x, pos.y);
    }

    void draw() {
        std::system("clear"); // use "cls" on Windows
        std::cout << "=== Out to C: Treasure Hunt (Level " << level << ") ===\n";
        std::cout << "Use WASD to move, Q to quit. \n";
        std::cout << "Grid: " << width << " x " << height << "\n";
        std::cout << "Moves: " << moves << " Score: " << score << "\n";
        std::cout << "Obstacles: " << obstacles.size() << "\n";

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                if (x == player.x && y == player.y) {
                    std::cout << "P ";
                } else if (x == treasure.x && y == treasure.y) {
                    std::cout << "T ";
                } else if (isObstacle(x, y)) {
                    std::cout << "# ";
                } else {
                    std::cout << ". ";
                }
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }

    void handleInput() {
        char c;
        std::cout << "Move (W/A/S/D) or Q to quit: ";
        std::cin >> c;
        c = static_cast<char>(std::tolower(c));

        Vec2 next = player;

        switch (c) {
        case 'w':
            if (player.y > 0) next.y--;
            break;
        case 's':
            if (player.y < height - 1) next.y++;
            break;
        case 'a':
            if (player.x > 0) next.x--;
            break;
        case 'd':
            if (player.x < width - 1) player.x++;
            break;
        case 'q':
            gameOver = true;
            return;
        default:
            std::cout << "Invalid input.\n";
            return;
        }

        // Check obstacles collision
        if (!isBlocked(next)) {
            player = next;
            moves++;
        } else {
            std::cout << "You bumped into an obstacle!\n";
        }
    }

    void update() {
        if (player.x == treasure.x && player.y == treasure.y) {
            int gained = 100 - moves + (level * 10);
            if (gained < 0) gained = 0;
            score += gained;

            std::cout << "\nYou found the treasure on level " << level << "!\n";
            std::cout << "Points gained: " << gained << "\n";

            level++;
            std::cout << "Advancing to level " << level << "...\n";
            std::cout << "Press ENTER to continue...";
            std::cin.ignore();
            std::cin.get();

            reset();
        }
    }

    void drawGameOver() {
        std::system("clear"); // use "cls" on Windows
        std::cout << "=== Game Over ===\n";
        std::cout << "Final level reached: " << level << "\n";
        std::cout << "Total score: " << score << "\n";
        std::cout << "Thanks for hunting treasure with obstacles!\n";
    }
};

int main() {
    Game game(8, 6); //base width, base height
    game.run();
    return 0;
}