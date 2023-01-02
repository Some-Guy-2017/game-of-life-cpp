#include "game.hpp"

#include <iostream>
#include <vector>
using std::cout;
using std::endl;

Game::Game(int width, int height, const char* title) {

    _frame_count = 0;

    _window_width = width;
    _window_height = height;

    srand(time(NULL));
    _pixel_data = new int[width * height * 4];

    SDL_Init(SDL_INIT_VIDEO);
    _window = SDL_CreateWindow(title, 100, 100, width, height, 0);
    _renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1");
    _texture = SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, width, height);

}

bool Game::run() {
    cout<<"game running"<<endl;
    return 0;
}

bool Game::_update_window() {
    SDL_UpdateTexture(_texture, nullptr, _pixel_data, _window_width * 4);
    SDL_RenderCopy(_renderer, _texture, nullptr, nullptr);
    SDL_RenderPresent(_renderer);

    if (SDL_QuitRequested()) return 1;
    return 0;
}

Game::~Game() {
    delete[] _pixel_data;
}
