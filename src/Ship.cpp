#include "Ship.h"

Ship::Ship(Vec2 _pos)
: m_pos(_pos)
, m_spd(Vec2{0.0f, 0.0f})
, m_dir(Vec2{0.0f, 0.0f})
, m_rot(0)
, m_turn_spd(0.1f)
, m_accel(0.1f)
{
    m_shape.push_back(Vec2{0.0f, 15.0f});
    m_shape.push_back(Vec2{-10.0f, -10.0f});
    m_shape.push_back(Vec2{10.0f, -10.0f});
}

Ship::~Ship(){}

void Ship::render(SDL_Renderer* _ren)
{
    SDL_SetRenderDrawColor(_ren, 0x00, 0x00, 0x00, 0xFF);
    for(unsigned i = 1; i < m_shape.size(); ++i) {
        SDL_RenderDrawLine(_ren
                          , m_shape[i-1].x + m_pos.x
                          , m_shape[i-1].y + m_pos.y
                          , m_shape[i].x + m_pos.x
                          , m_shape[i].y + m_pos.y);
    }
    SDL_RenderDrawLine(_ren
                      , m_shape.back().x + m_pos.x
                      , m_shape.back().y + m_pos.y
                      , m_shape.front().x + m_pos.x
                      , m_shape.front().y + m_pos.y);
}

void Ship::accel(float _perc) //_perc defaults to 1
{
    m_spd.x += m_accel * sin(m_rot) * _perc;
    m_spd.y += m_accel * cos(m_rot) * _perc;
}

void Ship::rotate(float _perc) //_perc defaults to 1
{
    float turn_val = m_turn_spd * _perc;
    m_rot -= turn_val; //clockwise is negative, as per standard

    for(unsigned i = 0; i < m_shape.size(); ++i) {
        float new_x = (m_shape[i].x * cos(turn_val))
                - (m_shape[i].y * sin(turn_val));
        float new_y = (m_shape[i].x * sin(turn_val))
                + (m_shape[i].y * cos(turn_val));

        m_shape[i].x = new_x;
        m_shape[i].y = new_y;
    }
}

void Ship::update(const SDL_Rect& _borders)
{
    m_pos.x += m_spd.x;
    m_pos.y += m_spd.y;

    if(m_pos.x < 0) {m_pos.x += _borders.w;}
    else if(m_pos.x > _borders.w) {m_pos.x -= _borders.w;}
    if(m_pos.y < 0) {m_pos.y += _borders.h;}
    else if(m_pos.y > _borders.h) {m_pos.y -= _borders.h;}
}
