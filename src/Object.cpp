#include "Object.h"

// Object::Object(const int _x, const int _y)
Object::Object(SDL_Rect _rect)
: m_rect(_rect)
{
}

Object::~Object()
{
	//TODO test if the texture is really eventually destroyed
	//the texure is destroyed by the smart pointer and custom destructor
}

int Object::assign_texture(shared_ptr<SDL_Texture> _tex)
{
	if(_tex == NULL) {
		throw "ERROR: Null texture assignment.";
	}
	
	m_texture = _tex;
	
	int tex_w, tex_h;
	SDL_QueryTexture(m_texture.get(), NULL, NULL, &tex_w, &tex_h);
	
	m_rect.w = tex_w;
	m_rect.h = tex_h;
	
	return 0;
}

void Object::render(SDL_Renderer* _ren)
{
	SDL_RenderCopy(_ren, m_texture.get(), NULL, &m_rect);
}

SDL_Rect* Object::get_rect()
{
	return &m_rect;
}
