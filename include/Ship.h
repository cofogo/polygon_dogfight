#include <vector>
using std::vector;

#include "utils.h"
using utils::Vec2;

class Ship {
public:
	Ship(Vec2 _pos);
	~Ship();

//	void rotate();
//	void accel();
//	void update();
	void render(SDL_Renderer* _ren);
private:
	vector<Vec2> m_shape;
	Vec2 m_pos;
	Vec2 m_spd;
	float m_dir;
};
