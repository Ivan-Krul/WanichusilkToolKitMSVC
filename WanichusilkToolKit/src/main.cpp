#include <iostream>
#include <chrono>

#include "SDL.h"

#undef main

struct TimeStop {
    std::chrono::high_resolution_clock::time_point beg, end;

    TimeStop() {
        beg = end = std::chrono::high_resolution_clock::now();
    }

    float stop() {
        end = std::chrono::high_resolution_clock::now();
        return std::chrono::duration_cast<std::chrono::microseconds>(end - beg).count() / 1000.0f;
    }
};

int main(int argc, char* argv[]) {
    printf("Local path: %s\n", argv[0]);

    SDL_Window* window = nullptr;
    SDL_Surface* wnd_surf = nullptr;

    if (SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "could not initialize sdl2 : % s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow("WanichusilkToolKit - test", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 640, 360, SDL_WINDOW_SHOWN);

    if (window == NULL) {
        fprintf(stderr, "could not create window: %s\n", SDL_GetError());
        return 1;
    }

    wnd_surf = SDL_GetWindowSurface(window);

    SDL_Event event = { 0 };
    bool is_quit = false;

    while (!is_quit) {
        while(SDL_PollEvent(&event))
            switch (event.type) {
            case SDL_QUIT: is_quit = true; break;
            default:
                break;
            }
    }

    SDL_FreeSurface(wnd_surf);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}

