#include "Bullet.h"
Bullet::Bullet(Vec2 _pos, float _spd, float _dir)
: m_pos(_pos)
, m_spd(_spd)
, m_trans(Vec2{_spd * cos(_dir), _spd * sin(_dir)})
, m_age(0)
, m_max_age(300)
{}

void Bullet::render(SDL_Renderer* _ren)
{
    SDL_SetRenderDrawColor(_ren, 0x00, 0x00, 0x00, 0xFF);
    SDL_RenderDrawPoint(_ren, m_pos.x, m_pos.y);
}

void Bullet::update()
{
    ++m_age;

    m_pos += m_trans;
}

bool Bullet::has_expired() { return (m_age > m_max_age); }
