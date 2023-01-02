#ifndef GAME_HPP
#define GAME_HPP

#include <cstdint>
#include <string>
#include <SDL2/SDL.h>

class Game {

public:

    Game(int, int, const char*);
    ~Game();
    bool _update_window();

    virtual bool run();

    SDL_Window* _window;
    SDL_Event _event;
    SDL_Renderer* _renderer;
    SDL_Texture* _texture;
    int* _pixel_data;

    int _window_width;
    int _window_height;

    int _frame_count;

};

#endif
