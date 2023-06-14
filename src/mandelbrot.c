#include <complex.h>
#include <stdio.h>

#include <SDL2/SDL.h>

#include "sdlutil.h"

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