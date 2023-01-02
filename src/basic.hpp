#ifndef BASIC_HPP
#define BASIC_HPP

#include "game.hpp"

class Basic : public Game {

public:
    Basic(int, int);
    ~Basic();
    bool run() override;

private:
    void _pad_state();
    void _combine_cells(uint8_t*, uint8_t*);
    uint8_t** _get_neighbours(uint8_t*);

	uint8_t* _state;
	uint8_t* _next;

    int _width;
    int _height;
    int _bm_len;
};

#endif
