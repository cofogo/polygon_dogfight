#include <iostream>
using std::cout;
using std::cin;
using std::cerr;
using std::endl;
#include <vector>
using std::vector;
#include <string>
using std::string;
using std::to_string;
#include <memory>
using std::shared_ptr;
#include <cmath>
using std::sin;
using std::cos;
using std::acos;

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "utils.h"
using utils::Vec2;
#include "Timer.h"
#include "Object.h"
#include "Text_Object.h"
#include "Ship.h"

const int win_w = 800;
const int win_h = 640;

int init(); //initialise SDL
int create_main_win(SDL_Window*& _win, SDL_Surface*& _srf,
                    const int _w, const int _h);
int create_win_renderer(SDL_Window* _win, SDL_Renderer*& _ren);
void close(SDL_Window*& _win, SDL_Renderer*& _ren);
void run_game(SDL_Renderer* _ren, const int _win_w, const int _win_h);
void draw_circle(utils::Circle* _cir, SDL_Renderer* _ren);
void draw_poly(utils::Circle* _cir, unsigned _fcs, SDL_Renderer* _ren);

int main(int argc, char* args[])
{
	if(init() != 0) {
		cerr << "the application will now exit.\n";
		return 1;
	}
	
	SDL_Window* win_main = NULL;
	SDL_Surface* srf_main = NULL;
	SDL_Renderer* ren_main = NULL;
	
	if(create_main_win(win_main, srf_main, win_w, win_h) != 0) {
		close(win_main, ren_main);
		return 1;
	}
	
	if(create_win_renderer(win_main, ren_main) != 0) {
		close(win_main, ren_main);
		return 1;
	}

	run_game(ren_main, win_w, win_h);
	
	close(win_main, ren_main);
	return 0;
}

int init()
{
	if(SDL_Init(SDL_INIT_VIDEO) < 0) {
		cerr << "ERROR: FATAL - could not initialise SDL!\n";
		cerr << "SDL error = " << SDL_GetError() << endl;
		return 1;
	}
	
	int sdl_image_flags = IMG_INIT_PNG;
	if(!(IMG_Init(sdl_image_flags) & sdl_image_flags)) {
		cerr << "ERROR: FATAL - could not initialise SDL_image!\n";
		cerr << "SDL_image error = " << IMG_GetError() << endl;
		return 1;
	}

	if(TTF_Init() == -1) {
		cerr << "ERROR: FATAL - could not initialise SDL_ttf!\n";
		cerr << "SDL_ttf error = " << TTF_GetError() << endl;
		return 1;
	}
	
	return 0;
}

int create_main_win(SDL_Window*& _win, SDL_Surface*& _srf,
										const int _w, const int _h)
{
	_win = SDL_CreateWindow("breakout42",
	                        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
	                        _w, _h,
	                         SDL_WINDOW_SHOWN);
	if(_win == NULL) {
		cerr << "ERROR: FATAL - SDL window could not be created!\n";
		cerr << "SDL error = " << SDL_GetError() << endl;
		return 1;
	}
	
	_srf = SDL_GetWindowSurface(_win);
	
	return 0;
}

int create_win_renderer(SDL_Window* _win, SDL_Renderer*& _ren)
{
	_ren = SDL_CreateRenderer(_win, -1, SDL_RENDERER_ACCELERATED);
	
	if(_ren == NULL) {
		cerr << "ERROR: Could not create renderer!\n";
		cerr << "SDL error = " << SDL_GetError() << endl;
		return 1;
	}
	
	//initialise renderer colour
	SDL_SetRenderDrawColor(_ren, 0xff, 0xff, 0xff, 0xff);
	
	return 0;
}

void close(SDL_Window*& _win, SDL_Renderer*& _ren)
{
	if(_win != NULL) {SDL_DestroyWindow(_win);}
	_win = NULL;
	
	if(_ren != NULL) {SDL_DestroyRenderer(_ren);}
	_ren = NULL;
	
	SDL_Quit();
}

void run_game(SDL_Renderer* _ren, const int _win_w, const int _win_h)
{
	//init game
	Timer loop_timer;
	const short tgt_fps = 60;
	Uint32 tgt_frame_len = 1000 / tgt_fps;
	Uint32 frame_len = 0;
	bool show_fps = false;

	Text_Object fps_txt("FPS: ",
	                    TTF_OpenFont("assets/fonts/DejaVuSansMono.ttf", 20),
	                    SDL_Colour{0x30, 0x80, 0xf0, 0x00},
	                    _ren,
	                    SDL_Rect{5, 5});

	Ship ship = Ship(Vec2{400.0f, 400.0f});
	utils::Circle cir = utils::Circle {400, 300, 100};
	utils::Circle* p_cir = &cir;

	//main loop
	bool pause = false;
	bool flag_quit = false;
	while(flag_quit == false) {
		SDL_SetRenderDrawColor(_ren, 0xFF, 0xF0, 0xF0, 0x00);
		loop_timer.set_start(SDL_GetTicks());
		
		//input and update phase
		SDL_Event event;
		const Uint8* key_states = SDL_GetKeyboardState(NULL);
		//key down/up check
		while(SDL_PollEvent(&event) != 0) {
			if(event.type == SDL_KEYDOWN) {
				if(key_states[SDL_SCANCODE_F]) {
					if(show_fps) {show_fps = false;}
					else {show_fps = true;}
				}
				if(key_states[SDL_SCANCODE_P]) {
					if(pause) {pause = false;}
					else {pause = true;}
				}
			}
			else if(event.type == SDL_QUIT) {
				flag_quit = true;
			}
		}

		if(pause) {SDL_Delay(30); continue;}

		if(show_fps) {
			if(frame_len > 0) {
				fps_txt.redraw("FPS: " + to_string(1000 / frame_len));
			}
			else {
				fps_txt.redraw("FPS: " + to_string(tgt_fps));
			}
		}

		//render phase
		SDL_RenderClear(_ren);

		if(show_fps) {fps_txt.render();}

		if(flag_quit == true) {
			break;
		}

		ship.render(_ren);
//		SDL_SetRenderDrawColor(_ren, 0x00, 0x00, 0xFF, 0x00);
//		cir.r = 100;
//		cir.x = 150;
//		draw_poly(p_cir, 52, _ren);
//		cir.x = 400;
//		draw_circle(p_cir, _ren);
//		cir.x = 650;
//		cir.r = 50;
//		draw_poly(p_cir, 6, _ren);

		SDL_RenderPresent(_ren);

		//limit framerate
		loop_timer.set_end(SDL_GetTicks());
		frame_len = loop_timer.get_duration();
		if(frame_len < tgt_frame_len) {
			Uint32 wait_len = tgt_frame_len - frame_len;
			if(wait_len > 0) {
				SDL_Delay(wait_len);
				frame_len += wait_len;
			}
		}
	}
}

void draw_circle(utils::Circle* _cir, SDL_Renderer* _ren) {
	double pi2 = acos(-1) * 2;
	double c_px = _cir->r * pi2; //circumference in pixels, draw this many times
	double itr_amt = (pi2) / c_px;
	unsigned times = 0;
	//pi2 == full circle in radians
	for(double i = 0; i < pi2; i += itr_amt) {
		SDL_RenderDrawPoint(_ren, _cir->x + (sin(i) * _cir->r), _cir->y + (cos(i) * _cir->r));	
		++times;
	}
}

void draw_poly(utils::Circle* _cir, unsigned _fcs, SDL_Renderer* _ren) {
	double pi2 = acos(-1) * 2;
	double itr_amt = (pi2) / _fcs;
	unsigned times = 0;
	for(double i = itr_amt/2; i < pi2; i += itr_amt) {
		SDL_RenderDrawLine(_ren, _cir->x + (sin(i) * _cir->r),
		                   _cir->y + (cos(i) * _cir->r),
		                   _cir->x + (sin(i + itr_amt) * _cir->r),
		                   _cir->y + (cos(i + itr_amt) * _cir->r));
		++times;
	}
}
