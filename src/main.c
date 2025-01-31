#include "main.h"

#include <SDL.h>
#include <SDL_error.h>
#include <SDL_events.h>
#include <SDL_keyboard.h>
#include <SDL_render.h>
#include <SDL_video.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "errors.h"
#include "game/game.h"
// #include "window/audio.h"
#include "window/renderer.h"

#ifdef __EMSCRIPTEN__ // for web builds
#include <emscripten.h>
#endif


bool playing = true;

SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;

// handles game application initialisation
static void init(void) {
    // initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) < 0) {
        error(ERROR_SDL_INIT, "SDL could not initialize! SDL Error: %s", SDL_GetError());
        return;
    }

    // initialize the renderer
    if (renderer_init(&window, &renderer) < 0) {
        error(ERROR_SDL_RENDERER_INIT, SDL_GetError());
        return;
    }

    // initialize audio
    // AudioDevice* audio_device = audio_device_init(32000, AUDIO_S16, 1, 255);
    // AudioData audio1 = audio_load_wav(audio_device, "FILE NAME");
}

// handles game application updating
static void update(void) {
    // update the input
    {
        SDL_Event e;
        while (SDL_PollEvent(&e)) {
            switch (e.type) {
            case SDL_QUIT:
                exit(SUCCESS);
                break;
            }
        }
    }

    // updates the game
    game_update((GameData){NULL}, SDL_GetKeyboardState(NULL));

    // updates the render
    RenderData render_data = {window, renderer};
    renderer_update(&render_data);
}

// handles game application quitting
void stop(void) {
    playing = false;
}

// entry point of the application
int32_t main(int32_t argc, char** argv) {
    (void)argc;
    (void)argv;
    init();

    while (playing)
        update();

    // cleanup of resources
    SDL_Quit();

    return 0;
}
