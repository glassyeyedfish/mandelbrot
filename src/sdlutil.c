#include <SDL2/SDL.h>

#include "./sdlutil.h"

#define KEYCODE_MOD 1073741625 
#define KEYCODE_MAX 512

ctx_t* 
create_ctx(const char* title, int width, int height) 
{
    ctx_t* ctx = (ctx_t*) malloc(sizeof(ctx_t));

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "[error] while initializing SDL.\nSDL Error: %s", SDL_GetError());
        return NULL;
    }

    ctx->win = SDL_CreateWindow(
        title, 
        SDL_WINDOWPOS_CENTERED, 
        SDL_WINDOWPOS_CENTERED,
        width,
        height,
        0
    );

    if (ctx->win == NULL) {
        fprintf(stderr, "[error] while creating SDL window.\nSDL Error: %s", SDL_GetError());
        return NULL;
    }

    ctx->ren = SDL_CreateRenderer(ctx->win, -1, 0);

    if (ctx->ren == NULL) {
        fprintf(stderr, "[error] while creating SDL renderer.\nSDL Error: %s", SDL_GetError());
        return NULL;
    }

    ctx->tex = SDL_CreateTexture(
        ctx->ren, 
        SDL_PIXELFORMAT_RGBA8888, 
        SDL_TEXTUREACCESS_STREAMING,
        width,
        height
    );

    if (ctx->tex == NULL) {
        fprintf(stderr, "[error] while creating SDL texture.\nSDL Error: %s", SDL_GetError());
        return NULL;
    }

    ctx->window_should_close = false;

    return ctx;
}

void 
destroy_ctx(ctx_t* ctx) 
{
    SDL_DestroyRenderer(ctx->ren);
    SDL_DestroyWindow(ctx->win);

    SDL_Quit();

    free(ctx);
}

void 
poll_events(ctx_t* ctx) 
{
    int i;
    SDL_Event e;


    for (i = 0; i < KEYCODE_MAX; i++) {
        ctx->previous_key_state[i] = ctx->current_key_state[i];
    }


    while (SDL_PollEvent(&e)) {
        switch(e.type) {
        case SDL_WINDOWEVENT:
            switch(e.window.event) {
            case SDL_WINDOWEVENT_CLOSE:
                ctx->window_should_close = true;
                break;
            default:
                break;
            }
            break;

        case SDL_KEYDOWN:
            for (i = 0; i < KEYCODE_MAX; i++) {
                if (e.key.keysym.sym % KEYCODE_MOD == i) {
                    ctx->current_key_state[i] = 1;
                }
            }
            break;

        case SDL_KEYUP:
            for (i = 0; i < KEYCODE_MAX; i++) {
                if (e.key.keysym.sym % KEYCODE_MOD == i) {
                    ctx->current_key_state[i] = 0;
                }
            }
            break;

        default:
            break;
        }
    }
}