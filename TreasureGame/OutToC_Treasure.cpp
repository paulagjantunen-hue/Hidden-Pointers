// Simple console treasure hunt game in C++
// Build: g++ OutToC_Treasure.cpp -o OutToC_Treasure

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

struct Vec2 {
    int x;
    int y;
};

class Game {
public:
    Game(int w, int h)
        : width(w), height(h), moves(0), score(0), gameOver(false) {
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
    int width;
    int height;
    int moves;
    int score;
    bool gameOver;

    Vec2 player;
    Vec2 treasure;

    void reset() {
        moves = 0;
        gameOver = false;
        player = randomPos();
        treasure = randomPos();
        // ensure treasure not on player
        while (treasure.x == player.x && treasure.y == player.y) {
            treasure = randomPos();
        }
    }

    Vec2 randomPos() {
        Vec2 v;
        v.x = std::rand() % width;
        v.y = std::rand() % height;
        return v;
    }

    void draw() {
        std::system("clear"); // use "cls" on Windows
        std::cout << "=== Out to C: Treasure Hunt ===\n";
        std::cout << "Use WASD to move, Q to quit. \n";
        std::cout << "Grid: " << width << " x " << height << "\n";
        std::cout << "Moves: " << moves << " Score: " << score << "\n\n";

        for (int y = 0; y < height; ++y) {
            for (int x = 0; x < width; ++x) {
                if (x == player.x && y == player.y) {
                    std::cout << "P ";
                } else if (x == treasure.x && y == treasure.y) {
                    std::cout << "T ";
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

        switch (std::tolower(c)) {
        case 'w':
            if (player.y > 0) player.y--;
            moves++;
            break;
        case 's':
            if (player.y < height - 1) player.y++;
            moves++;
            break;
        case 'a':
            if (player.x > 0) player.x--;
            moves++;
            break;
        case 'd':
            if (player.x < width - 1) player.x++;
            moves++;
            break;
        case 'q':
            gameOver = true;
            break;
        default:
            std::cout << "Invalid input.\n";
            break;
        }
    }

    void update() {
        if (player.x == treasure.x && player.y == treasure.y) {
            score += 100 - moves;
            if (score < 0) score = 0;
            std::cout << "\nYou found the treasure! +"
                      << (100 - moves) << " points\n";
            std::cout << "Press ENTER to continue...";
            std::cin.ignore();
            std::cin.get();
            reset();
        }
    }

    void drawGameOver() {
        std::system("clear"); // use "cls" on Windows
        std::cout << "=== Game Over ===\n";
        std::cout << "Total score: " << score << "\n";
        std::cout << "Thanks for coding in C++ and hunting treasure!\n";
    }
};

int main() {
    Game game(8, 6); //width, height
    game.run();
    return 0;
}