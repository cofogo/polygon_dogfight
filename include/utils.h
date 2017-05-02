//this is a header file to store various utility functions

#ifndef UTILS_H
#define UTILS_H

#include <iostream>
using std::cout;
using std::cerr;
using std::endl;
#include <string>
using std::string;

#include <SDL2/SDL_ttf.h>

namespace utils {

struct Line2 {
	int a_x, a_y, b_x, b_y;
};

struct Circle {
	int x, y, r;
};

struct Vec2 {
	float x, y;

    Vec2 operator+=(const Vec2& _v)
    {
        this->x += _v.x;
        this->y += _v.y;
        return *this;
    }
};

SDL_Texture* load_txt_texture(string _s,
             TTF_Font* _font, SDL_Colour _col,
             SDL_Renderer* _ren);
}
#endif //UTILS_H
