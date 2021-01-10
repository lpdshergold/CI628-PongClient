#include <string>
#include "MyGame.h"

using namespace std;



// Player constructor
Player::Player(int x, int y, int width, int height, const char* path) {
    this->X = x;
    this->Y = y;
    this->Width = width;
    this->Height = height;
    this->pPath = path;
    
    this->rectDestination = { X,Y,Width,Height };
}

// render the player to the screen
void Player::render(SDL_Renderer* renderer) {
    Image playerImage{ X, Y, Width, Height, pPath };
    playerImage.render(renderer);
}

// Set the y position of the player
void Player::setY(int yPos) {
    Y = yPos;
}

// Get the bat position
void Player::getBat() {
    rectDestination = { X,Y,Width,Height };
}

// call to update yPos and get the bat x, y, height and width
void Player::updateBat(int yPos) {
    setY(yPos);
    getBat();
}



// ball constructor
Ball::Ball(int x, int y, int width, int height, const char* path) {
    this->xPos = x;
    this->yPos = y;
    this->bWidth = width;
    this->bHeight = height;
    this->pPath = path;

    this->rectDestination = { xPos, yPos, bWidth, bHeight };
}

void Ball::render(SDL_Renderer* renderer) {
    Image ball{ xPos, yPos, bWidth, bHeight, pPath };
    ball.render(renderer);
}

// set the balls yPos
void Ball::setY(int y) {
    yPos = y;
}

// set the balls xPos
void Ball::setX(int x) {
    xPos = x;
}

// get the balls x, y, width and height
void Ball::getBall() {
    rectDestination = { xPos, yPos, bWidth, bHeight };
}

// used to update pos and call setters
void Ball::updateBall(int xPos, int yPos) {
    setX(xPos);
    setY(yPos);
    getBall();
}



// GameFont constructor
GameFont::GameFont(const char* fontPath, int fontSize, SDL_Color color, int x, int y) {
    this->textPath = fontPath;
    this->textSize = fontSize;
    this->textColor = color;
    this->xPos = x;
    this->yPos = y;
}

// set the score of the player
void GameFont::setScore(int score) {
    playerScore = score;
    playerScoreText = std::to_string(playerScore);
}

// function to get the font surface
SDL_Surface* GameFont::getFontSurface() {
    TTF_Font* textFont = TTF_OpenFont(textPath, textSize);

    if (textFont == nullptr) {
        printf("TTF_Font: %s\n", TTF_GetError());
    }

    std::string playerScoreText = std::to_string(playerScore);

    SDL_Color fontColor = textColor;

    SDL_Surface* tempSurface = TTF_RenderText_Blended(textFont, playerScoreText.c_str(), fontColor);

    // clear font from memory
    TTF_CloseFont(textFont);
    textFont = nullptr;

    // return a surface to where this function is called
    return tempSurface;
}

// render function to show font on screen
void GameFont::renderText(SDL_Renderer* render) {
    // retrieve the font surface
    SDL_Surface* textSurface = getFontSurface();

    if (textSurface != nullptr) {
        // create texture from existing textSurface
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(render, textSurface);

        // free surface from memory
        SDL_FreeSurface(textSurface);
        textSurface = nullptr;

        // check if nullptr
        if (textTexture != nullptr) {
            int width, height;
            // querying texture for the width and height for font
            SDL_QueryTexture(textTexture, NULL, NULL, &width, &height);

            SDL_Rect fontPos = { xPos, yPos, width, height };

            // render texture onto screen
            SDL_RenderCopy(render, textTexture, NULL, &fontPos);

            // destroy the texture in memory and give textTexture a nullptr
            SDL_DestroyTexture(textTexture);
            textTexture = nullptr;
        } else {
            printf("SDL_Texture: renderText error");
        }
    } else {
        printf("SDL_Surface: renderText error");
    }
}



// Image Constructor
Image::Image(int x, int y, int width, int height, const char* path = nullptr) {
    this->sWidth = width;
    this->sHeight = height;
    this->pPath = path;
    this->rectDestination = {x, y, width, height};
}

// render image
void Image::render(SDL_Renderer* renderer) {
    this->sprite = IMG_Load(pPath);
    if (sprite == nullptr) {
        printf("IMG_Load: %s\n", IMG_GetError());
    }

    if (sprite != nullptr) {
        sTexture = SDL_CreateTextureFromSurface(renderer, sprite);

        SDL_FreeSurface(sprite);
        sprite = nullptr;
        if (sTexture != nullptr) {
            SDL_RenderCopy(renderer, sTexture, NULL, &rectDestination);

            SDL_DestroyTexture(sTexture);
            sTexture = nullptr;
        }
    }
}


Particle::Particle(double xPos, double yPos, double velXPos, double velYPos, int pSize, double pLife, SDL_Color pColor) {
    this->x = xPos;
    this->y = yPos;
    this->vel_x = velXPos;
    this->vel_y = velYPos;
    this->size = pSize;
    this->color = pColor;
    this->life = pLife;
}


// sound effect function (if music is needed, make playMusic())
Mix_Chunk MyGame::playSound(const char* path = "") {
    Mix_Chunk* sound = nullptr;
    sound = Mix_LoadWAV(path);

    // check sound is not a nullptr
    if (sound == nullptr) {
        printf("SDL_Mixer: %s\n", Mix_GetError());
        exit(12);
    }

    // Play sound
    Mix_PlayChannel(-1, sound, 0);
}


MyGame::MyGame() {}

// recieve data from server
void MyGame::on_receive(string cmd, vector<string>& args) {
    if (cmd == "GAME_DATA") {
        // we should have exactly 4 arguments
        if (args.size() == 4) {
            game_data.player1Y = stoi(args.at(0));
            game_data.player2Y = stoi(args.at(1));
            game_data.ballX = stoi(args.at(2));
            game_data.ballY = stoi(args.at(3));
        }
    } else {
        cout << "Received: " << cmd << endl;
    }

    // check for SCORES from the server
    if (cmd == "SCORES") {
        playSound(GOAL_PATH); // play goal sound

        // should get two arguments
        if (args.size() == 2) {
            // store server score
            game_data.playerOneScore = stoi(args.at(0));
            game_data.playerTwoScore = stoi(args.at(1));
        }
    }

    if (cmd == "HIT_WALL_LEFTGAME_DATA") {
        particleCelebrationAfterGoal(game_data.ballX, game_data.ballY, true);
    } else if (cmd == "HIT_WALL_RIGHTGAME_DATA") {
        particleCelebrationAfterGoal(game_data.ballX, game_data.ballY, false);
    }

    // check to see if the ball hit one of the bats
    if (cmd == "BALL_HIT_BAT1") {
        playSound(BAT_HIT_PATH); // play hitting bat sound
    } else if (cmd == "BALL_HIT_BAT2") {
        playSound(BAT_HIT_PATH); // play hitting bat sound
    }
}

void MyGame::send(string message) {
    messages.push_back(message);
}

void MyGame::input(SDL_Event& event) {
    switch (event.key.keysym.sym) {
        case SDLK_w:
            send(event.type == SDL_KEYDOWN ? "W_DOWN" : "W_UP");
            break;
        case SDLK_s:
            send(event.type == SDL_KEYDOWN ? "S_DOWN" : "S_UP");
            break;
        case SDLK_i:
            send(event.type == SDL_KEYDOWN ? "I_DOWN" : "I_UP");
            break;
        case SDLK_k:
            send(event.type == SDL_KEYDOWN ? "K_DOWN" : "K_UP");
            break;
        default:
            cout << "Not aware of that key" << endl;
    }
}

double MyGame::getRandomNumber() {
    double random = rand() * 1.0 / RAND_MAX;
    return random;
}

void MyGame::checkAllParticles(SDL_Renderer* renderer) {
    for (auto particle : particles) {
        SDL_Rect rect = { (int)particle->x, (int)particle->y, particle->size * 2, particle->size * 2 };

        SDL_SetRenderDrawColor(renderer, particle->color.r, particle->color.g, particle->color.b, particle->color.a);

        SDL_RenderFillRect(renderer, &rect);
    }
}

void MyGame::particleVelAndLife(float reduceParticleLife) {
    for (auto particle : particles) {
        particle->x += particle->vel_x;
        particle->y += particle->vel_y;
        particle->life -= reduceParticleLife;

        if (particle->life <= 0.0) {
            particle->color.a = 0;
        } else {
            particle->color.a = (Uint8)((particle->life / 5.0) * 255);
        }
    }
}

void MyGame::particlesFollowBall(int ballXPos, int ballYPos) {
    for (int numOfParticles = 0; numOfParticles < 5; numOfParticles++) {
        double velX = -1 * getRandomNumber();
        double velY = getRandomNumber() - 0.5;

        SDL_Color color = { 255, 0, 0, 255 };

        particles.push_back(new Particle(ballXPos + 10, ballYPos + 12, velX * 2, velY * 2, 4, 1.0, color));
    }
    particleVelAndLife(0.075);
}

void MyGame::particleCelebrationAfterGoal(int ballXPos, int ballYPos, bool leftGoal) {
    for (int numOfParticles = 0; numOfParticles < 20; numOfParticles++) {
        double velX, velY;
        if (leftGoal) {
            velX = 2.5 * getRandomNumber();
            velY = 2.5 * getRandomNumber();
        } else {
            velX = -2.5 * getRandomNumber();
            velY = -2.5 * getRandomNumber();
        }

        SDL_Color color = { 100, 100, 255, 255 };

        particles.push_back(new Particle(ballXPos, ballYPos, velX * 2, velY * 2, 4, (1.0 * 2), color));
    }
    particleVelAndLife(0.050);
}

void MyGame::update() {
    // update player positions
    playerOne->updateBat(game_data.player1Y);
    playerTwo->updateBat(game_data.player2Y);

    // update ball position
    ball->updateBall(game_data.ballX, game_data.ballY);

    // update player scores
    firstPlayerScore->setScore(game_data.playerOneScore);
    secondPlayerScore->setScore(game_data.playerTwoScore);

    // update particle position to follow ball
    particlesFollowBall(game_data.ballX, game_data.ballY);
}


void MyGame::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_ADD);
    
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // render the background image first
    background->render(renderer);

    // render player one score to screen
    firstPlayerScore->renderText(renderer);

    // render player two score to screen
    secondPlayerScore->renderText(renderer);

    // render player1 onscreen
    playerOne->render(renderer);

    // render player2 onscreen
    playerTwo->render(renderer);

    // render out all particles
    checkAllParticles(renderer);

    // render the ball onscreen
    ball->render(renderer);
}