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
        const char* pPath;

        SDL_Rect rectDestination;

        // set y position of the bat instance
        void setY(int yPos);
        // get the bat position
        void getBat();

    public:
        // player constructor
        Player(int x, int y, int width, int height, const char* path);
        // update the bat y position and get the bats position
        void updateBat(int yPos);

        void render(SDL_Renderer* renderer);
};

// class for the ball
class Ball {
    private:
        int xPos = 0, yPos = 0, bWidth = 0, bHeight = 0; 
        const char* pPath;

        SDL_Rect rectDestination;

        void setY(int yPos);
        void setX(int xPos);
        void getBall();
    public:
        Ball(int x, int y, int width, int height, const char* path);
        void updateBall(int y, int x);

        // TEMP 
        void render(SDL_Renderer* renderer);
};

// class for fonts
class GameFont {
    private:
        int playerScore = 0;
        std::string playerScoreText;

        const char* textPath;
        int textSize, xPos, yPos;
        SDL_Color textColor;

    public:
        GameFont(const char* fontPath, int fontSize, SDL_Color color, int xPos, int yPos);
        void setScore(int score);
        SDL_Surface* getFontSurface();
        void renderText(SDL_Renderer* render);
};

// class for images
class Image {
    private:
        SDL_Surface* sprite = nullptr;
        SDL_Rect rectDestination;
        SDL_Texture* sTexture = nullptr;

        int sWidth, sHeight;
        const char* pPath;
    public:
        Image(int x, int y, int width, int height, const char* path);
        void render(SDL_Renderer* renderer);
};

class Particle {
    private:

    public:
    double x, y;
    int size;
    double life;
    double vel_x, vel_y;

    SDL_Color color;

    Particle(double xPos, double yPos, double velXPos, double velYPos, int pSize, double pLife, SDL_Color pColor);
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
        const char* FFF_FONT_PATH = "res/fonts/FFF_Tusj.ttf";

        // player 1 and 2
        Player* playerOne = new Player( 200, game_data.player1Y, 25, 75, PLAYER_ONE_PATH );
        Player* playerTwo = new Player( 600, game_data.player2Y, 25, 75, PLAYER_TWO_PATH );

        // ball instance
        Ball* ball = new Ball( game_data.ballX, game_data.ballY, 30, 30, FOOTBALL_PATH );

        // player scores
        SDL_Color color = { 0,0,0,255 };
        const int FONT_SIZE = 50;

        GameFont* firstPlayerScore = new GameFont( FFF_FONT_PATH, FONT_SIZE, color, 175, 75 );
        GameFont* secondPlayerScore = new GameFont( FFF_FONT_PATH, FONT_SIZE, color, 575, 75 );

        // background image
        Image* background = new Image( 0, 0, 800, 600, FOOTBALL_FIELD_PATH );

        std::vector<Particle*> particles;

    public:
        MyGame();

        std::vector<std::string> messages;

        Mix_Chunk playSound(const char* path);

        void on_receive(std::string message, std::vector<std::string>& args);
        void send(std::string message);
        void input(SDL_Event& event);
        void update();
        void render(SDL_Renderer* renderer);
        void checkAllParticles(SDL_Renderer* renderer); 
        void particleVelAndLife(float reduceParticleLife);
        void particlesFollowBall(int ballXPos, int ballYPos);
        void particleCelebrationAfterGoal(int ballXPos, int ballYPos, bool leftGoal);
        double getRandomNumber();
};
#endif