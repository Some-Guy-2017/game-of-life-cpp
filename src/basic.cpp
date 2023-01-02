#include "init.hpp"
#include "basic.hpp"
#include <time.h>

#include <iostream>
#include <bitset>
using std::cout;
using std::endl;

Basic::Basic(int width, int height) : Game(width, height, "Game of life") {

    //Game(width, height);

	_width = width;
	_height = height;

    _bm_len = _width * _height;

	_state = new uint8_t[_bm_len];
    _next = new uint8_t[_bm_len];

    // cell:
    //   bit 0   = cell state
    //   bit 1-4 = neighbour count
    //   bit 5-7 = unused

    for (int i=0;i<_bm_len;i++) {
        _state[i] = rand() & 1;
        if (
            i / _width == 0 || i / _width == _height-1 ||
            i % _width == 0 || i % _width == _width-1
        ) continue;
        if (_state[i]) _pixel_data[i] = 0xffffffff;
        else _pixel_data[i] = 0x000000ff;
    }

    _pad_state();

    // initialize the amount of neighbours at each cell
    for (uint8_t* i=_state+_width+1;i<_state+_bm_len-_width-1;i++) {
        int index = (int)(i - _state);
        if (index % _width == 0 || index % _width == _width-1) continue;
        uint8_t n = 0;
        uint8_t** neighbours = _get_neighbours(i);
        for (int i=0;i<8;i++) {
            uint8_t* neighbour = neighbours[i];
            if (*neighbour & 1) {
                n++;
            }
        }
        *i |= (n & 0b1111) << 1;
    }

    for (int i=0;i<_bm_len;i++) _next[i] = 0;

}

bool Basic::run() {

    _pad_state();

    /*
    if (true) {
        for (int i=0;i<_width+2;i++) cout<<"--";
        cout<<'\n';
        for (int y=0;y<_height;y++) {
        cout<<" |";
        for (int x=0;x<_width;x++) {
            int i = x + y * _width;
            if (_state[i]&1) cout<<'@';
            else cout<<' ';
            cout<<((_state[i]>>1)&0b1111);
        }
        cout<<"| \n";
        }
        for (int i=0;i<_width+2;i++) cout<<"--";
        cout<<endl;
    }

    std::cin.get();
    */

    uint8_t* next_pad = _state + _width + _width - 1;
    uint8_t* now = _state + _width;
    uint8_t* next = _next + _width;
    uint8_t* end = _state + _bm_len - _width;
    int* pixel = _pixel_data + _width;

    while (now < end) {
        now++;
        next++;
        pixel++;
        if (now == next_pad) {
            next_pad += _width;
            now++;
            next++;
            pixel++;
            continue;
        }
        uint8_t state = *now & 1;
        uint8_t nbr_n = (*now >> 1) & 0b1111;

        if (state) {

            if (nbr_n == 2 || nbr_n == 3) {
                _combine_cells(next, now);
            }

            else {
                *pixel = 0x000000ff;
                *now &= 0b11110;
                _combine_cells(next, now);
                uint8_t** nbr_is = _get_neighbours(next);

                for (int i=0;i<8;i++) {
                    uint8_t* nbr_i = *(nbr_is++);
                    uint8_t n = (*nbr_i >> 1) & 0b1111;
                    n--;
                    *nbr_i &= 1;
                    *nbr_i |= (n & 0b1111) << 1;
                }

            }

        }

        else {

            if (nbr_n != 3) {
                _combine_cells(next, now);
            }

            else {
                *pixel = 0xffffffff;
                *now &= 0b11110;
                *now |= 1;
                _combine_cells(next, now);

                uint8_t** nbr_is = _get_neighbours(next);
                for (int i=0;i<8;i++) {
                    uint8_t* nbr_i = *(nbr_is++);
                    uint8_t n = (*nbr_i >> 1) & 0b1111;
                    n++;
                    *nbr_i &= 1;
                    *nbr_i |= (n & 0b1111) << 1;
                }
    
            }

        }
    }

    // swap next and now
    uint8_t* tmp = _state;
    _state = _next;
    _next = tmp;

    next = _next;
    end = _next + _bm_len;
    while (next < end)
        *(next++) = 0;

    return _update_window();
}

void Basic::_pad_state() {
    uint8_t* pad;
    uint8_t* data;
    uint8_t* end;

    pad = _state+1;
    end = _state + _width-1;
    data = _state + _bm_len - _width - _width + 1;
    while (pad < end) {
        _combine_cells(pad, data);
        *pad &= 1;
        pad++;
        data++;
    }

    pad = _state + _bm_len-_width+1;
    end = _state + _bm_len-1;
    data = _state + _width + 1;
    while (pad < end) {
        _combine_cells(pad, data);
        *pad &= 1;
        pad++;
        data++;
    }

    pad = _state + _width;
    end = _state + _bm_len - _width;
    data = _state + _width + _width - 2;
    while (pad < end) {
        _combine_cells(pad, data);
        *pad &= 1;
        pad += _width;
        data += _width;
    }

    pad = _state + _width + _width - 1;
    end = _state + _bm_len - 1;
    data = _state + _width + 1;
    while (pad < end) {
        _combine_cells(pad, data);
        *pad &= 1;
        pad += _width;
        data += _width;
    }

    _combine_cells(_state,_state+_bm_len-_width-2);
    *_state &= 1;
    _combine_cells(_state+_bm_len-1,_state+_width+1);
    *(_state+_bm_len-1) &= 1;
    _combine_cells(_state+_width-1,_state+_bm_len-_width-_width+1);
    *(_state+_width-1) &= 1;
    _combine_cells(_state+_bm_len-_width,_state+_width*2-2);
    *(_state+_bm_len-_width) &= 1;
}

void Basic::_combine_cells(uint8_t* dst, uint8_t* src) {
    uint8_t n = (*dst >> 1) & 0b1111;
    n += (*src >> 1) & 0b1111;
    *src &= 1;
    *src |= (n & 0b1111) << 1;
    *dst = *src;
}

uint8_t** Basic::_get_neighbours(uint8_t* index) {
    static uint8_t* neighbours[8];
    uint8_t* oi = index - _width - 1;

    int i = 0;
    for (int oy=0;oy<=2;oy++) {
        for (int ox=0;ox<=2;ox++) {
            if (ox != 1 || oy != 1) {
                neighbours[i++] = oi;
            }
            oi++;
        }
        oi += _width;
        oi -= 3;
    }

    return neighbours;
}

Basic::~Basic() {
    delete[] _state;
    delete[] _next;
}
