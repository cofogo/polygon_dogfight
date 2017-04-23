#include "Ship.h"

Ship::Ship(Vec2 _pos)
: m_pos(_pos)
, m_spd(Vec2{0.0f, 0.0f})
, m_dir(Vec2{0.0f, 0.0f})
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

void Ship::accel(const Vec2& _v)
{
	m_spd.x += _v.x;
	m_spd.y += _v.y;
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
