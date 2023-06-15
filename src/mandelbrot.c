#include <complex.h>
#include <stdio.h>

#include <SDL2/SDL.h>

#include "sdlutil.h"
#include "nuklear.h"

#define WINDOW_TITLE "Mandelbrot Set Visualizer"
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

#define ITERATION_COUNT 100
#define ERROR_MARGIN 10.0            
#define ZOOM_FACTOR 300.0

#define X_OFFSET 100
#define Y_OFFSET 0

/*
 * A utility function that prints a complex number to the standard output.
 */
void 
print_complex(double complex z) {
    printf("%.1f + %.1fi\n", creal(z), cimag(z));
}

/*
 * The iterative function that generates the mandlebrot set:
 * f_c(z) = z^2 + c
 */
double complex
iterate(double complex z, double complex c) {
    return (z * z) + c;
}

int 
main() 
{
    // Set up SDL
    sdl_ctx_t* ctx = create_ctx(WINDOW_TITLE, WINDOW_WIDTH, WINDOW_HEIGHT);
    Uint32** pixels = malloc(sizeof(Uint32) * WINDOW_WIDTH * WINDOW_HEIGHT);
    int* pitch = malloc(sizeof(int));

    if (ctx == NULL) {
        return 1;
    }

    /*
     * This next bit of code unlocks the texture, writes the pixels for the 
     * visualization, then locks the texture so that it can be copied to the 
     * SDL renderer.
     */
    SDL_LockTexture(ctx->tex, NULL, (void**) pixels, pitch);

    for (int x = 0; x < WINDOW_WIDTH; x++) {
        for (int y = 0; y < WINDOW_HEIGHT; y++) {
            double complex z = 0.0;
            double complex c = (double) (x - X_OFFSET - WINDOW_WIDTH/2) / ZOOM_FACTOR + ((double) (y - Y_OFFSET - WINDOW_HEIGHT/2) / ZOOM_FACTOR) * I;

            for (int i = 0; i < ITERATION_COUNT; i++) {
                z = iterate(z, c);
            }

            if (cabs(z - c) < ERROR_MARGIN) {
                (*pixels)[(y * WINDOW_WIDTH) + x] = 0xFFFFFFFF;
            } else {
                (*pixels)[(y * WINDOW_WIDTH) + x] = 0x000000FF;
            }
        }
    }

    SDL_UnlockTexture(ctx->tex);

    // The main loop
    while (!(ctx->window_should_close)) {
        poll_events(ctx);

        /* GUI */
        if (nk_begin(ctx->nk, "Demo", nk_rect(50, 50, 230, 250),
            NK_WINDOW_BORDER|NK_WINDOW_MOVABLE|NK_WINDOW_SCALABLE|
            NK_WINDOW_MINIMIZABLE|NK_WINDOW_TITLE))
        {
            enum {EASY, HARD};
            static int op = EASY;
            static int property = 20;

            nk_layout_row_static(ctx->nk, 30, 80, 1);
            if (nk_button_label(ctx->nk, "button"))
                fprintf(stdout, "button pressed\n");
            nk_layout_row_dynamic(ctx->nk, 30, 2);
            if (nk_option_label(ctx->nk, "easy", op == EASY)) op = EASY;
            if (nk_option_label(ctx->nk, "hard", op == HARD)) op = HARD;
            nk_layout_row_dynamic(ctx->nk, 25, 1);
            nk_property_int(ctx->nk, "Compression:", 0, &property, 100, 10, 1);

            nk_layout_row_dynamic(ctx->nk, 20, 1);
            nk_label(ctx->nk, "background:", NK_TEXT_LEFT);
            nk_layout_row_dynamic(ctx->nk, 25, 1);
            if (nk_combo_begin_color(ctx->nk, nk_rgb_cf(ctx->bg), nk_vec2(nk_widget_width(ctx->nk),400))) {
                nk_layout_row_dynamic(ctx->nk, 120, 1);
                ctx->bg = nk_color_picker(ctx->nk, ctx->bg, NK_RGBA);
                nk_layout_row_dynamic(ctx->nk, 25, 1);
                ctx->bg.r = nk_propertyf(ctx->nk, "#R:", 0, ctx->bg.r, 1.0f, 0.01f,0.005f);
                ctx->bg.g = nk_propertyf(ctx->nk, "#G:", 0, ctx->bg.g, 1.0f, 0.01f,0.005f);
                ctx->bg.b = nk_propertyf(ctx->nk, "#B:", 0, ctx->bg.b, 1.0f, 0.01f,0.005f);
                ctx->bg.a = nk_propertyf(ctx->nk, "#A:", 0, ctx->bg.a, 1.0f, 0.01f,0.005f);
                nk_combo_end(ctx->nk);
            }
        }
        nk_end(ctx->nk);

        SDL_RenderCopy(ctx->ren, ctx->tex, NULL, NULL);
        SDL_RenderPresent(ctx->ren);

        SDL_Delay(16);
    }

    // Take down SDL
    free(pitch);
    free(pixels);
    destroy_ctx(ctx);

    return 0;
}