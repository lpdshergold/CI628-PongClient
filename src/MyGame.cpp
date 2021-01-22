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
GameFont::GameFont(const char* fontPath, int fontSize, SDL_Color color, int x, int y, std::string manualText) {
    this->textPath = fontPath;
    this->textSize = fontSize;
    this->textColor = color;
    this->xPos = x;
    this->yPos = y;
    this->selectPlayerText = manualText;
}

// set the score of the player
void GameFont::setScore(int score) {
    playerScore = score;
    playerScoreText = std::to_string(playerScore);
}

void GameFont::setString() {
    selectPlayerText = "";
}

// function to get the font surface
SDL_Surface* GameFont::getScoreSurface() {
    TTF_Font* textFont = TTF_OpenFont(textPath, textSize);

    if (&textFont == nullptr) {
        printf("TTF_Font: %s\n", TTF_GetError());
    }

    std::string playerScoreText = std::to_string(playerScore);
    const char* formattedScore = playerScoreText.c_str();

    SDL_Color fontColor = textColor;

    SDL_Surface* tempSurface = TTF_RenderText_Blended(textFont, formattedScore, fontColor);

    // clear font from memory
    TTF_CloseFont(textFont);
    textFont = nullptr;

    // return a surface to where this function is called
    return tempSurface;
}

SDL_Surface* GameFont::getTextSurface() {
    TTF_Font* textFont = TTF_OpenFont(textPath, textSize);

    if (&textFont == nullptr) {
        printf("TTF_Font: %s\n", TTF_GetError());
    }

    std::string playerSelectText = selectPlayerText;
    const char* formattedText = playerSelectText.c_str();

    SDL_Color fontColor = textColor;

    SDL_Surface* tempSurface = TTF_RenderText_Blended(textFont, formattedText, fontColor);

    // clear font from memory
    TTF_CloseFont(textFont);
    textFont = nullptr;

    // return a surface to where this function is called
    return tempSurface;
}

// render function to show font on screen
void GameFont::renderFont(SDL_Renderer* render) {
    // retrieve the font surface
    SDL_Surface* textSurface;
    if (selectPlayerText != "") {
        textSurface = getTextSurface();
    } else {
        textSurface = getScoreSurface();
    }

    if (&textSurface != nullptr) {
        // create texture from existing textSurface
        SDL_Texture* textTexture = SDL_CreateTextureFromSurface(render, textSurface);

        // free surface from memory
        SDL_FreeSurface(textSurface);
        textSurface = nullptr;

        // check if nullptr
        if (&textTexture != nullptr) {
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

    if (sprite != nullptr) {
        sTexture = SDL_CreateTextureFromSurface(renderer, sprite);

        SDL_FreeSurface(sprite);
        sprite = nullptr;
        if (sTexture != nullptr) {
            SDL_RenderCopy(renderer, sTexture, NULL, &rectDestination);

            SDL_DestroyTexture(sTexture);
            sTexture = nullptr;
        }
    } else {
        printf("IMG_Load: %s\n", IMG_GetError());
    }
}


Particle::Particle(double xPos, double yPos, double velXPos, double velYPos, double pLife, SDL_Color pColor) {
    this->pX = xPos;
    this->pY = yPos;
    this->vel_x = velXPos;
    this->vel_y = velYPos;
    this->color = pColor;
    this->life = pLife;
    this->size = 4;
}

int Particle::getParticleSize() {
    return this->size;
}

int Particle::getParticleSizeMultiply(int timesSize = 0) {
    return this->size * timesSize;
}

SDL_Color Particle::getParticleColor() {
    return this->color;
}

void Particle::setParticleColorAlpha(float alphaVal) {
    this->color = { color.r, color.g, color.b, (Uint8)alphaVal };
}

void Particle::reduceParticleLife(float reduceLife) {
    this->life -= reduceLife;
}

double Particle::getParticleLife() {
    return this->life;
}

double Particle::getParticleX() {
    return this->pX;
}

double Particle::getParticleY() {
    return this->pY;
}

void Particle::updateParticleVel() {
    this->pX += this->vel_x;
    this->pY += this->vel_y;
}

SDL_Rect Particle::setParticleRect(int timesSize) {
    SDL_Rect tempRect = { this->getParticleX(), this->getParticleY(), this->getParticleSizeMultiply(timesSize), this->getParticleSizeMultiply(timesSize) };
    return tempRect;
}


// sound effect function (if music is needed, make playMusic())
void MyGame::playSound(const char* path = "") {
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
    } else if (cmd == "SCORES") {
        playSound(GOAL_PATH); // play goal sound

        // should get two arguments
        if (args.size() == 2) {
            // store server score
            game_data.playerOneScore = stoi(args.at(0));
            game_data.playerTwoScore = stoi(args.at(1));
        }
    } else if (cmd == "HIT_WALL_LEFTGAME_DATA") {
        particleCelebrationAfterGoal(game_data.ballX, game_data.ballY, true);
    } else if (cmd == "HIT_WALL_RIGHTGAME_DATA") {
        particleCelebrationAfterGoal(game_data.ballX, game_data.ballY, false);
    } else if (cmd == "BALL_HIT_BAT1") {
        playSound(BAT_HIT_PATH); // play hitting bat sound
    } else if (cmd == "BALL_HIT_BAT2") {
        playSound(BAT_HIT_PATH); // play hitting bat sound
    } else {
        cout << "Received: " << cmd << endl;
    }
}

void MyGame::send(string message) {
    messages.push_back(message);
}

void MyGame::input(SDL_Event& event) {
    if (game_data.selectPlayer == 0) {
        switch (event.key.keysym.sym) {
            case SDLK_1:
                game_data.selectPlayer = 1;
                selectPlayerOneText->setString();
                selectPlayerTwoText->setString();
                break;
            case SDLK_2:
                game_data.selectPlayer = 2;
                selectPlayerOneText->setString();
                selectPlayerTwoText->setString();
                break;
            default:
                break;
        }
    }

    if (game_data.selectPlayer == 1) {
        switch (event.key.keysym.sym) {
            case SDLK_w:
                send(event.type == SDL_KEYDOWN ? "W_DOWN" : "W_UP");
                break;
            case SDLK_s:
                send(event.type == SDL_KEYDOWN ? "S_DOWN" : "S_UP");
                break;
        }
    } else if (game_data.selectPlayer == 2) {
        switch (event.key.keysym.sym) {
            case SDLK_i:
                send(event.type == SDL_KEYDOWN ? "I_DOWN" : "I_UP");
                break;
            case SDLK_k:
                send(event.type == SDL_KEYDOWN ? "K_DOWN" : "K_UP");
                break;
            default:
                break;
        }
    }
}

double MyGame::getRandomVel(double velTimesAmount) {
    double random = rand() * 1.0 / RAND_MAX;

    return velTimesAmount * random;
}

// get random color values
SDL_Color MyGame::randomColor() {
    int random[3];

    for (int &num : random) {
        num = rand() * 255 + 1;
    }

    SDL_Color color = { (Uint8)random[0], (Uint8)random[1], (Uint8)random[2], (Uint8)random[3] };
    return color;
}

// erase particles when not needed
void MyGame::eraseParticle() {
    // loop through all the particles and remove any that is no longer needed
    for (int particle = 0; particle <= allParticles.size() - 1; particle++) {
        if (allParticles[particle]->getParticleLife() <= 0.0) {
            allParticles.erase(allParticles.begin() + particle);
        }
    }
}

// render all particles
void MyGame::renderAllParticles(SDL_Renderer* renderer) {
    for (Particle* particle : allParticles) {
        SDL_Rect rect = particle->setParticleRect(1);

        SDL_SetRenderDrawColor(renderer, particle->getParticleColor().r, particle->getParticleColor().g, particle->getParticleColor().b, particle->getParticleColor().a);

        SDL_RenderFillRect(renderer, &rect);
    }
}

// update the particle velocity
void MyGame::particleVel() {
    for (Particle* particle : allParticles) {
        particle->updateParticleVel();
    }
}

// reduce particle life time
void MyGame::particleLife(float reduceParticleLife) {
    for (Particle* particle : allParticles) {
        particle->reduceParticleLife(reduceParticleLife);
    }
}

// create particles to follow the ball
void MyGame::particlesFollowBall(int ballXPos, int ballYPos) {
    SDL_Color color = { 244, 138, 0, 255 };
    int maxParticles[5];

    for (int &particle : maxParticles) {
        int ballX = ballXPos + 10;
        int ballY = ballYPos + 12;

        allParticles.push_back(new Particle(ballX, ballY, (getRandomVel(-1) * 2), (getRandomVel(-1) * 2), 1.0, color));
    }
}

// create particle effect when scoring a goal
void MyGame::particleCelebrationAfterGoal(int ballXPos, int ballYPos, bool leftGoal) {
    int maxParticles[40];

    for (int &particles : maxParticles) {
        SDL_Color color = randomColor();

        if (leftGoal) {
            allParticles.push_back(new Particle(ballXPos, ballYPos, (getRandomVel(2.5) * 2), (getRandomVel(2.5) * 2), (1.0 * 2), color));
        } else {
            allParticles.push_back(new Particle(ballXPos, ballYPos, (getRandomVel(-2.5) * 2), (getRandomVel(-2.5) * 2), (1.0 * 2), color));
        }
    }
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
    // update each particles life
    particleLife(0.050);
    // update each particles vel
    particleVel();
    // erase particles that aren't needed anymore
    eraseParticle();
}


void MyGame::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // render the background image
    background->render(renderer);

    // render player one score to screen
    firstPlayerScore->renderFont(renderer);

    // render player two score to screen
    secondPlayerScore->renderFont(renderer);

    if (game_data.selectPlayer == 0) {
        selectPlayerOneText->renderFont(renderer);
        selectPlayerTwoText->renderFont(renderer);
    }

    // render player1 onscreen
    playerOne->render(renderer);

    // render player2 onscreen
    playerTwo->render(renderer);

    // render out all particles
    renderAllParticles(renderer);

    // render the ball onscreen
    ball->render(renderer);
}