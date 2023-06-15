#include <SDL2/SDL.h>

#include "sdlutil.h"

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_STANDARD_VARARGS
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT
#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#define NK_SDL_RENDERER_IMPLEMENTATION

#include "nuklear.h"
#include "nuklear_sdl_renderer.h"

#define KEYCODE_MOD 1073741625 
#define KEYCODE_MAX 512

#define FONT_SCALE 1.0f

sdl_ctx_t* 
create_ctx(const char* title, int width, int height) 
{
    // Initialize SDL stuff
    sdl_ctx_t* ctx = (sdl_ctx_t*) malloc(sizeof(sdl_ctx_t));

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

    // Set up Nuklear

    ctx->nk = nk_sdl_init(ctx->win, ctx->ren);
    /* Load Fonts: if none of these are loaded a default font will be used  */
    /* Load Cursor: if you uncomment cursor loading please hide the cursor */
    {
        struct nk_font_atlas *atlas;
        struct nk_font_config config = nk_font_config(0);
        struct nk_font *font;

        /* set up the font atlas and add desired font; note that font sizes are
         * multiplied by font_scale to produce better results at higher DPIs */
        nk_sdl_font_stash_begin(&atlas);
        font = nk_font_atlas_add_default(atlas, 13 * FONT_SCALE, &config);
        nk_sdl_font_stash_end();

        /* this hack makes the font appear to be scaled down to the desired
         * size and is only necessary when font_scale > 1 */
        font->handle.height /= FONT_SCALE;
        /*nk_style_load_all_cursors(ctx, atlas->cursors);*/
        nk_style_set_font(ctx, &font->handle);
    }

    return ctx;
}

void 
destroy_ctx(sdl_ctx_t* ctx) 
{
    SDL_DestroyRenderer(ctx->ren);
    SDL_DestroyWindow(ctx->win);

    SDL_Quit();

    free(ctx);
}

void 
poll_events(sdl_ctx_t* ctx) 
{
    int i;
    SDL_Event e;

    // Nuklear event handling
    nk_input_begin(ctx->nk);

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

        // Nuklear event handling
        nk_sdl_handle_event(&e);
        nk_input_end(ctx->nk);
    }
}