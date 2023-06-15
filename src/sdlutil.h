#ifndef SDLUTIL_H
#define SDLUTIL_H

#include <stdbool.h>

#include <SDL2/SDL.h>

#include "nuklear.h"

typedef struct {
    SDL_Window* win;
    SDL_Renderer* ren;
    SDL_Texture* tex;

    bool window_should_close;
    SDL_KeyCode current_key_state[512];
    SDL_KeyCode previous_key_state[512];

    struct nk_context *nk;
    struct nk_colorf bg;
} sdl_ctx_t;

typedef enum {
    KEY_BKSP    = 8,
    KEY_TAB     = 9,
    KEY_RETURN  = 13,
    KEY_ESC     = 27,
    KEY_SPACE   = 32,
    KEY_0       = 48,
    KEY_1, KEY_2, KEY_3,
    KEY_4, KEY_5, KEY_6,
    KEY_7, KEY_8, KEY_9,
    KEY_A       = 97,
    KEY_B, KEY_C, KEY_D, KEY_E, KEY_F,
    KEY_G, KEY_H, KEY_I, KEY_J, KEY_K,
    KEY_L, KEY_M, KEY_N, KEY_O, KEY_P,
    KEY_Q, KEY_R, KEY_S, KEY_T, KEY_U,
    KEY_V, KEY_W, KEY_X, KEY_Y, KEY_Z,
    KEY_RIGHT   = 278,
    KEY_LEFT, KEY_DOWN, KEY_UP
} key_t;

sdl_ctx_t* create_ctx(const char* title, int width, int height);
void destroy_ctx(sdl_ctx_t* ctx);
void poll_events(sdl_ctx_t* ctx);

#endif