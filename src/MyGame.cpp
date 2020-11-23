#include <string>
#include "MyGame.h"

using namespace std;

Player::Player(int x, int y, int width, int height) {
    X = x; 
    Y = y;
    Width = width;
    Height = height;

    bat = { X,Y,Width,Height };
}

void Player::setY(int yPos) {
    Y = yPos;
}

void Player::updateBat() {
    bat = { X,Y,Width,Height };
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

        // should get two arguments
        if (args.size() == 2) {

            // store server score
            game_data.playerOneScore = stoi(args.at(0));
            game_data.playerTwoScore = stoi(args.at(1));
        }
    }

    // check to see if the ball hit one of the bats
    if (cmd == "BALL_HIT_BAT1") {
        cout << "BAT1 was hit by the ball!" << endl;
    } else if (cmd == "BALL_HIT_BAT2") {
        cout << "No wait, ball hit BAT2 instead!" << endl;
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
    playerOne.setY(game_data.player1Y);
    playerOne.updateBat();
    playerTwo.setY(game_data.player2Y);
    playerTwo.updateBat();
    ball.y = game_data.ballY;
    ball.x = game_data.ballX;
}

void MyGame::render(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &playerOne.bat);
    SDL_RenderDrawRect(renderer, &playerTwo.bat);
    SDL_RenderDrawRect(renderer, &ball);
}
