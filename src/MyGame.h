#ifndef __MY_GAME_H__
#define __MY_GAME_H__

#include <iostream>
#include <vector>
#include <string>

#include "SDL.h"
#include "SDL_mixer.h"

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
        void getBat();
};

class MyGame {
    private:
        // Sound effect paths
        const char* bat_hit_path = "../res/sounds/bat_hit.wav";

        // player 1 and 2
        Player playerOne{ 200, 0, 25, 75 };
        Player playerTwo{ 570, 0, 25, 75 };
        SDL_Rect ball = { 400, 0 , 30, 30 };

    public:
        std::vector<std::string> messages;

        Mix_Chunk playSound(const char* path);

        void on_receive(std::string message, std::vector<std::string>& args);
        void send(std::string message);
        void input(SDL_Event& event);
        void update();
        void render(SDL_Renderer* renderer);
};
#endif