/*
 * mandelbrot.c
 *
 * Uses an algorithm adapted from https://en.wikipedia.org/wiki/Plotting_algorithms_for_the_Mandelbrot_set
 */

#include <raylib.h>

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>
#endif

// A smooth gradient of blue to purple and back
#define COLOR_00 (Color) {   4,   3,  48, 255 }
#define COLOR_01 (Color) {  28,   8, 145, 255 }
#define COLOR_02 (Color) {  74,  13, 242, 255 }
#define COLOR_03 (Color) { 162, 110, 247, 255 }
#define COLOR_04 (Color) { 237, 207, 252, 255 }
#define COLOR_05 (Color) { 218, 110, 247, 255 }
#define COLOR_06 (Color) { 219,  13, 242, 255 }
#define COLOR_07 (Color) { 145,   8, 138, 255 }
#define COLOR_08 (Color) {  44,   3,  48, 255 }
#define COLOR_09 (Color) { 116,   8, 145, 255 }
#define COLOR_10 (Color) { 166, 013, 242, 255 }
#define COLOR_11 (Color) { 162, 110, 247, 255 }
#define COLOR_12 (Color) { 219, 207, 252, 255 }
#define COLOR_13 (Color) { 130, 110, 247, 255 }
#define COLOR_14 (Color) {  20,  13, 242, 255 }
#define COLOR_15 (Color) {   8,  19, 145, 255 }

int screen_width = 1920;
int screen_height = 1080;

int max_iterations = 1000;
float zoom_factor = 450.0;
int x_offset = 250;

Color colors[] = {
    COLOR_00,
    COLOR_01,
    COLOR_02,
    COLOR_03,
    COLOR_04,
    COLOR_05,
    COLOR_06,
    COLOR_07,
    COLOR_08,
    COLOR_09,
    COLOR_10,
    COLOR_11,
    COLOR_12,
    COLOR_13,
    COLOR_14,
    COLOR_15
};

Image buffer;
Texture tex;

/*
 * The main loop simply draws the texture containing the visualization
 */
void 
main_loop(void)
{
    BeginDrawing();
    DrawTexture(tex, 0, 0, WHITE);
    EndDrawing();
}

int 
main()
{
    // Setup
    InitWindow(screen_width, screen_height, "Mandelbrot Set Visualizer");

    buffer = GenImageColor(screen_width, screen_height, BLACK);

    // The algorithm to generation the mandelbrot set visualization
    for (int x = 0; x < screen_width; x++) {
        for (int y = 0; y < screen_height; y++) {
            float x0 = (float) (x - (screen_width / 2) - x_offset) / zoom_factor;
            float y0 = (float) (y - (screen_height / 2)) / zoom_factor;
            float x1 = 0.0f;
            float y1 = 0.0f;
            int i = 0;
            float x2 = 0.0f;
            float y2 = 0.0f;

            while (x2 + y2 <= 4 && i < max_iterations) {
                y1 = 2 * x1 * y1 + y0;
                x1 = x2 - y2 + x0;
                x2 = x1 * x1;
                y2 = y1 * y1;
                ++i;
            }

            if (i == max_iterations) {
                ImageDrawPixel(&buffer, x, y, BLACK);
            } else {
                ImageDrawPixel(&buffer, x, y, colors[i % 16]);
            }
        }
    }

    tex = LoadTextureFromImage(buffer);

    // Two ways to call the main loop depending on the platform.
#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(main_loop, 0, 1);
#else
    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        main_loop();
    }
#endif

    // Teardown
    CloseWindow();
    return 0;
}
