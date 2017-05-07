#ifndef SHIP_H
#define SHIP_H

#include <vector>
using std::vector;
#include <cmath>
using std::sin;
using std::cos;
using std::pow;
using std::sqrt;

#include "utils.h"
using utils::Vec2;

#include "Bullet.h"

class Ship {
public:
	Ship(Vec2 _pos);
	~Ship();

	void rotate(float _t, float _perc = 1.0f);
	void accel(float _t, float _perc = 1.0f);
	void update(float _t, const SDL_Rect& _borders);
	void render(SDL_Renderer* _ren);
    Bullet* fire1();
    //void fire2();
private:
	vector<Vec2> m_shape;
	Vec2 m_pos;
	Vec2 m_trans;
    float m_rot;
    float m_turn_spd; // rad/s
    float m_accel; // pixels/s
    float m_weapon1_vel;
};
#endif //SHIP_H
