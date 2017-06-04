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
#include "Bullet.h"

const int win_w = 800;
const int win_h = 640;
const string win_title = "polygon dogfight";

int init(); //initialise SDL
int create_main_win(SDL_Window*& _win,
                    const string& _title, const int _w, const int _h);
int create_win_renderer(SDL_Window* _win, SDL_Renderer*& _ren);
void close(SDL_Window*& _win, SDL_Renderer*& _ren);
void run_game(SDL_Renderer* _ren, const int _win_w, const int _win_h);
void draw_circle(utils::Circle* _cir, SDL_Renderer* _ren);
void draw_poly(utils::Circle* _cir, unsigned _fcs, SDL_Renderer* _ren);
void smart_del_vector(vector<auto*>& _v, unsigned _pos);
bool detect_coll_pt_cir(Vec2 _pos1, Vec2 _pos2, unsigned _r2);

int main(int argc, char* args[])
{
    if(init() != 0) {
        cerr << "the application will now exit.\n";
        return 1;
    }
    
    SDL_Window* win_main = NULL;
    SDL_Renderer* ren_main = NULL;
    
    if(create_main_win(win_main, win_title, win_w, win_h) != 0) {
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

int create_main_win(SDL_Window*& _win,
        const string& _title, const int _w, const int _h)
{
    _win = SDL_CreateWindow(_title.c_str(),
                            SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                            _w, _h,
                             SDL_WINDOW_SHOWN);
    if(_win == NULL) {
        cerr << "ERROR: FATAL - SDL window could not be created!\n";
        cerr << "SDL error = " << SDL_GetError() << endl;
        return 1;
    }
    
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
    SDL_Rect scene_rect = SDL_Rect{0, 0, _win_w, _win_h};

    Text_Object fps_txt("FPS: ",
                        TTF_OpenFont("assets/fonts/DejaVuSansMono.ttf", 20),
                        SDL_Colour{0x30, 0x80, 0xf0, 0x00},
                        _ren,
                        SDL_Rect{5, 5});
    Text_Object game_over("GAME OVER",
                          TTF_OpenFont("assets/fonts/DejaVuSansMono.ttf", 20),
                          SDL_Colour{0x30, 0x80, 0xf0, 0x00},
                          _ren,
                          SDL_Rect{0, 0});

    vector<Ship*> ships;
    ships.push_back(new Ship(Vec2{500.0f, 400.0f}));
    ships.push_back(new Ship(Vec2{100.0f, 400.0f}));
    //TODO clear bullets array from expired (NULL) objects
    vector<Bullet*> bullets;
    utils::Circle cir = utils::Circle {400, 300, 100};
    utils::Circle* p_cir = &cir;

    //main loop
    bool pause = false;
    bool flag_quit = false;
    while(flag_quit == false) {
        SDL_SetRenderDrawColor(_ren, 0xFF, 0xF0, 0xF0, 0x00);
        loop_timer.set_start(SDL_GetTicks());
        float dt = float(frame_len) / 1000.0f;
        
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
                //player0
                if(key_states[SDL_SCANCODE_UP]) {
                    bullets.push_back(ships[0]->fire1());
                }
                //player1
                if(key_states[SDL_SCANCODE_W]) {
                    bullets.push_back(ships[1]->fire1());
                }
            }
            else if(event.type == SDL_QUIT) {flag_quit = true;}
        }
        //continuous response key check
                //player0
        if(key_states[SDL_SCANCODE_DOWN]) {ships[0]->accel(dt, 1.0f);}
        if(key_states[SDL_SCANCODE_LEFT]) {ships[0]->rotate(dt, -1.0f);}
        if(key_states[SDL_SCANCODE_RIGHT]) {ships[0]->rotate(dt, 1.0f);}
                //player1
        if(key_states[SDL_SCANCODE_S]) {ships[1]->accel(dt, 1.0f);}
        if(key_states[SDL_SCANCODE_A]) {ships[1]->rotate(dt, -1.0f);}
        if(key_states[SDL_SCANCODE_D]) {ships[1]->rotate(dt, 1.0f);}

        if(pause) {SDL_Delay(tgt_frame_len); continue;}

        for(unsigned i = 0; i < ships.size(); ++i) {
            if(ships[i]->is_dead()) {
                smart_del_vector(ships, i);
                --i;
                continue;
            }
            ships[i]->update(dt, scene_rect);
        }

        if(ships.size() == 0) { pause = true; continue;}

        for(unsigned i = 0; i < bullets.size(); ++i) {
            if(bullets[i]->has_expired()) {
                smart_del_vector(bullets, i);
                --i;
                continue;
            }

            bullets[i]->update(dt);

            for(unsigned j = 0; j < ships.size(); ++j) {
                if(detect_coll_pt_cir(bullets[i]->get_pos(),
                        ships[j]->get_pos(), ships[j]->get_coll_rad())
                ) {
                    ships[j]->take_dmg();
                }
            }
        }

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

        if(ships.size() == 1) {
            game_over.render_stretched();
        }

        if(flag_quit == true) {
            break;
        }

        for(unsigned i = 0; i < ships.size(); ++i) {
            ships[i]->render(_ren);
        }

        for(unsigned i = 0; i < bullets.size(); ++i) {
            bullets[i]->render(_ren);
        }

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

void smart_del_vector(vector<auto*>& _v, unsigned _pos)
{
    delete _v[_pos];
    if(_pos < _v.size() - 1) {_v[_pos] = _v.back();}
    _v.pop_back();
}

bool detect_coll_pt_cir(Vec2 _pos1, Vec2 _pos2, unsigned _r2)
{
    //pythagorean theorem
    Vec2 dist_v = Vec2{fabs(_pos1.x - _pos2.x), fabs(_pos1.y - _pos2.y)};
    float dist = sqrtf(powf(dist_v.x, 2) + powf(dist_v.y, 2));

    if(dist <= _r2) {
        return true;
    }

    return false;
}
