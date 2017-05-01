#include <vector>
using std::vector;
#include <cmath>
using std::sin;
using std::cos;
using std::pow;
using std::sqrt;

#include "utils.h"
using utils::Vec2;

class Ship {
public:
	Ship(Vec2 _pos);
	~Ship();

	void rotate(float _perc = 1.0f);
	void accel(float _perc = 1.0f);
	void update(const SDL_Rect& _borders);
	void render(SDL_Renderer* _ren);
private:
	vector<Vec2> m_shape;
	Vec2 m_pos;
	Vec2 m_spd;
	Vec2 m_dir;
    float m_rot;
    float m_turn_spd;
    float m_accel;
};
