#ifndef __MY_GAME_H__
#define __MY_GAME_H__

#include <iostream>
#include <vector>
#include <string>

#include "SDL.h"
#include "SDL_mixer.h"
#include "SDL_ttf.h"
#include "SDL_image.h"

static struct GameData {
    int player1Y = 0;
    int player2Y = 0;

    int ballX = 0;
    int ballY = 0;

    int playerOneScore = 0;
    int playerTwoScore = 0;
} game_data;

// class for the players
class Player {
    private:
        int X = 0, Y = 0, Width = 0, Height = 0;
        //SDL_Rect bat = {};


        // TEMP 
        SDL_Surface* sprite = nullptr;
        SDL_Rect rectDestination;
        SDL_Texture* sTexture = nullptr;

        // set y position of the bat instance
        void setY(int yPos);
        // get the bat position
        void getBat();

    public:
        // player constructor
        Player(int x, int y, int width, int height, const char* path);
        // update the bat y position and get the bats position
        void updateBat(int yPos);


        // TEMP 
        void render(SDL_Renderer* renderer);
};

// class for the ball
class Ball {
    private:
        int xPos = 0, yPos = 0, bWidth = 0, bHeight = 0; 
        SDL_Rect ball = {};

        // TEMP 
        SDL_Surface* sprite = nullptr;
        SDL_Rect rectDestination;
        SDL_Texture* sTexture = nullptr;

        void setY(int yPos);
        void setX(int xPos);
        void getBall();
    public:
        Ball(int x, int y, int width, int height, const char* path);
        void updateBall(int y, int x);

        // TEMP 
        void render(SDL_Renderer* renderer);
};

// class for audio
class Audio {
    private:
    public:
};

// class for fonts
class GameFont {
    private:
        int playerScore = 0;
        string playerScoreText;
    public:
        GameFont(const char* fontPath, int score, SDL_Color color, int fontSize);
        int setScore(int score);
        void getScore();
        void renderText(SDL_Renderer render);
};

// class for images
class Image {
    private:
        SDL_Surface* sprite = nullptr;
        SDL_Rect rectDestination;
        SDL_Texture* sTexture = nullptr;

        // int xPos, yPos, sWidth, sHeight;
        // const char* sPath;
    public:
        Image(int x, int y, int width, int height, const char* path);
        void render(SDL_Renderer* renderer);
};

class MyGame {
    private:
        // sound effect paths
        const char* BAT_HIT_PATH = "res/sounds/bat_hit.wav";
        const char* GOAL_PATH = "res/sounds/goal.wav";

        // image paths
        const char* FOOTBALL_FIELD_PATH = "res/images/football_field.png";
        const char* FOOTBALL_PATH = "res/images/football.png";
        const char* PLAYER_ONE_PATH = "res/images/playerOne.png";
        const char* PLAYER_TWO_PATH = "res/images/playerTwo.png";

        // font path
        const char* ARIAL_FONT_PATH = "res/fonts/arial.ttf";

        // player 1 and 2
        Player playerOne{ 200, game_data.player1Y, 25, 75, PLAYER_ONE_PATH };
        Player playerTwo{ 600, game_data.player2Y, 25, 75, PLAYER_TWO_PATH };

        // ball instance
        Ball ball{ game_data.ballX, game_data.ballY, 30, 30, FOOTBALL_PATH };

        // player scores
        GameFont firstPlayerScore;
        GameFont secondPlayerScore;

        // background image
        Image background{ 0, 0, 800, 600, FOOTBALL_FIELD_PATH };

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