#include <string>
#include "MyGame.h"

#include "SDL_image.h"

using namespace std;

// Player Class ====================================================
// Player constructor
Player::Player(int x, int y, int width, int height, const char* path) {
    X = x; 
    Y = y;
    Width = width;
    Height = height;

    rectDestination = { X,Y,Width,Height };

    sprite = IMG_Load(path);
    if (sprite == nullptr) {
        printf("IMG_Load: %s\n", IMG_GetError());
    }
}

void Player::render(SDL_Renderer* renderer) {
    sTexture = SDL_CreateTextureFromSurface(renderer, sprite);

    SDL_RenderCopy(renderer, sTexture, NULL, &rectDestination);

    SDL_DestroyTexture(sTexture);
    sTexture = nullptr;
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
// ==================================================================

// Ball Class =======================================================
// ball constructor
Ball::Ball(int x, int y, int width, int height, const char* path) {
    xPos = x;
    yPos = y;
    bWidth = width;
    bHeight = height;

    rectDestination = { xPos, yPos, bWidth, bHeight };

    sprite = IMG_Load(path);
    if (sprite == nullptr) {
        printf("IMG_Load: %s\n", IMG_GetError());
    }
}

void Ball::render(SDL_Renderer* renderer) {
    sTexture = SDL_CreateTextureFromSurface(renderer, sprite);

    SDL_RenderCopy(renderer, sTexture, NULL, &rectDestination);

    SDL_DestroyTexture(sTexture);
    sTexture = nullptr;
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
// ==================================================================


// Image Class =======================================================
// Image Constructor
Image::Image(int x, int y, int width, int height, const char* path = "") {
    rectDestination = {x, y, width, height};

    sprite = IMG_Load(path);
    if (sprite == nullptr) {
        printf("IMG_Load: %s\n", IMG_GetError());
    }
}

void Image::render(SDL_Renderer* renderer) {
    sTexture = SDL_CreateTextureFromSurface(renderer, sprite);

    SDL_RenderCopy(renderer, sTexture, NULL, &rectDestination);

    SDL_DestroyTexture(sTexture);
    sTexture = nullptr;
}
// ===================================================================

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

void MyGame::update() {
    playerOne.updateBat(game_data.player1Y);
    playerTwo.updateBat(game_data.player2Y);
    ball.updateBall(game_data.ballX, game_data.ballY);
}

void MyGame::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // render the background image first
    background.render(renderer);
    TTF_Font* fontAriel = TTF_OpenFont("res/fonts/arial.ttf", 24);

    if (fontAriel == nullptr) {
        printf("TTF_Font: %s\n", TTF_GetError());
    }

    int playerScore = game_data.playerOneScore;
    std::string score_text = std::to_string(playerScore);

    SDL_Color color = { 0, 0, 0, 255 };
    SDL_Surface* text_surface = TTF_RenderText_Blended(fontAriel, score_text.c_str(), color);

    // clear font memory
    TTF_CloseFont(fontAriel);

    if (text_surface != nullptr) {
        SDL_Texture* text_texture = SDL_CreateTextureFromSurface(renderer, text_surface);

        SDL_FreeSurface(text_surface);
        if (text_texture != nullptr) {
            int w, h;
            SDL_QueryTexture(text_texture, NULL, NULL, &w, &h);
            SDL_Rect dst = {100,100, w, h};

            // NULL to draw entire texture
            SDL_RenderCopy(renderer, text_texture, NULL, &dst);

            SDL_DestroyTexture(text_texture);
        }
    }

    // render player1 onscreen
    playerOne.render(renderer);

    // render player2 onscreen
    playerTwo.render(renderer);
    
    // render the ball onscreen
    ball.render(renderer);
}