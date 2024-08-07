#include <iostream>
#include <chrono>

#include "Window.h"

//#include "ComponentPropertyKit.h"

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

    TimeStop t;
    
    if (SDL_Init(SDL_INIT_EVENTS | SDL_INIT_VIDEO) < 0) {
        fprintf(stderr, "could not initialize sdl2 : % s\n", SDL_GetError());
        return 1;
    }

    graphic_system_lib::Window window("WTK - test", 640, 480);

    auto l = [](){printf("Destroyed\n"); };
    window.setOnDestroy(l);

    SDL_Event event = { 0 };
    bool is_quit = false;

    while (!is_quit) {
        while(SDL_PollEvent(&event))
            switch (event.type) {
            case SDL_QUIT: is_quit = true; break;
            default:
                break;
            }

        //Fill the surface white
        SDL_FillRect(window.getWindowSufrace(), NULL, SDL_MapRGB(window.getWindowSufrace()->format, 0xFF, 0xFF, 0xFF));

        //Update the surface
        SDL_UpdateWindowSurface(window.getWindow());

        SDL_Delay(10);
    }

    window.terminate();

    SDL_Quit();
    

    std::cout << t.stop() << "ms\n";
    return 0;
}

