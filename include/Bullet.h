#ifndef BULLET_H
#define BULLET_H

#include <cmath>
using std::sin;
using std::cos;

#include "utils.h"
using utils::Vec2;

class Bullet {
public:
    Bullet(Vec2 _pos, Vec2 _parent_trans, float _spd, float _dir);
    ~Bullet() {}; 

    void render(SDL_Renderer* _ren);
    void update(float _t);
    bool has_expired();
private:
    Vec2 m_pos;
    Vec2 m_trans;
    float m_spd;
    unsigned m_age;
    unsigned m_max_age;
};

#endif //BULLET_H
