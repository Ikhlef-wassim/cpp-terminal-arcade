#include <iostream>
#include <vector>
#include <string>
#include <termios.h>
#include <unistd.h>

const int MAP_W = 16;
const int MAP_H = 10;

std::string map[MAP_H] = {
    "################",
    "#    #     .   #",
    "#    #  ###### #",
    "## ###  #    # #",
    "#    #  # .  # #",
    "# .  #  #  ### #",
    "#### #  #  #   #",
    "#    #     # . #",
    "#    #######   #",
    "################"
};

int playerX = 2;
int playerY = 1;
int score = 0;
int totalGems = 4;

char getKeyPress() {
    char buf = 0;
    struct termios old = {0};
    if (tcgetattr(0, &old) < 0) perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcgetattr(0, &old) < 0) perror("tcsetattr()");
    if (read(0, &buf, 1) < 0) perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSANOW, &old) < 0) perror("tcsetattr()");
    return buf;
}

void render() {
    std::cout << "\x1B[2J\x1B[H"; 
    std::cout << "=== PIXEL ARCADE: TERMINAL CRAWLER ===\n";
    std::cout << "Controls: W (Up) | A (Left) | S (Down) | D (Right) | Q (Quit)\n";
    std::cout << "Gems Remaining: " << totalGems << " | Score: " << score << "\n\n";

    for (int y = 0; y < MAP_H; ++y) {
        for (int x = 0; x < MAP_W; ++x) {
            if (x == playerX && y == playerY) {
                std::cout << "P"; 
            } else {
                std::cout << map[y][x];
            }
        }
        std::cout << "\n";
    }
}

int main() {
    while (true) {
        render();

        if (totalGems == 0) {
            std::cout << "\n🎉 VICTORY! You collected all the gems!\n";
            break;
        }

        char input = getKeyPress();
        if (input == 'q' || input == 'Q') {
            std::cout << "\nGame Exited. Thanks for playing!\n";
            break;
        }

        int targetX = playerX;
        int targetY = playerY;

        if (input == 'w' || input == 'W') targetY--;
        if (input == 's' || input == 'S') targetY++;
        if (input == 'a' || input == 'A') targetX--;
        if (input == 'd' || input == 'D') targetX++;

        if (map[targetY][targetX] != '#') {
            playerX = targetX;
            playerY = targetY;

            if (map[playerY][playerX] == '.') {
                map[playerY][playerX] = ' ';
                score += 100;
                totalGems--;
            }
        }
    }
    return 0;
}
