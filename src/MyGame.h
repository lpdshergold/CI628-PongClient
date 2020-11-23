#ifndef __MY_GAME_H__
#define __MY_GAME_H__

#include <iostream>
#include <vector>
#include <string>

#include "SDL.h"

static struct GameData {
    int player1Y = 0;
    int player2Y = 0;
    int ballX = 0;
    int ballY = 0;
    int playerOneScore = 0;
    int playerTwoScore = 0;
} game_data;

class Player {
    private:
        int X = 0, Y = 0, Width = 0, Height = 0;

    public:
        SDL_Rect bat = {};
        Player(int x, int y, int width, int height);
        void setY(int yPos);
        void updateBat();
};

class MyGame {

    private:
    // player 1 and 2
        Player playerOne{ 200, 0, 25, 75 };
        Player playerTwo{ 570, 0, 25, 75 };
        SDL_Rect ball = { 400, 0 , 30, 30 };

    public:
        std::vector<std::string> messages;

        void on_receive(std::string message, std::vector<std::string>& args);
        void send(std::string message);
        void input(SDL_Event& event);
        void update();
        void render(SDL_Renderer* renderer);
};
#endif