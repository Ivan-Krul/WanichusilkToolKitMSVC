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

    if (resdl_mean_lib::SDLRoot::getInstance().init()) {
        fprintf(stderr, "could not initialize sdl2 : % s\n", SDL_GetError());
        return 1;
    }

    resdl_mean_lib::Window window("WTK - test", 640, 480);

    auto l = [](){printf("Destroyed\n"); };
    window.setOnDestroy(l);

    SDL_Event event = { 0 };
    bool is_quit = false;
    float t_s = 0;

    while (!is_quit) {
        while(SDL_PollEvent(&event))
            switch (event.type) {
            case SDL_QUIT: is_quit = true; break;
            default:
                break;
            }

        SDL_Rect rect{ rand() % 100 + 100,rand() % 100 + 100,rand() % 100 + 50,rand() % 100 + 50 };
        //Fill the surface white
        SDL_FillRect(window.getWindowSufrace(), NULL, SDL_MapRGB(window.getWindowSufrace()->format, 0xFF, 0xFF, 0xFF));
        SDL_FillRect(window.getWindowSufrace(), &rect, SDL_MapRGB(window.getWindowSufrace()->format, 0xFF, ((int)t_s), 0x00));

        //Update the surface
        SDL_UpdateWindowSurface(window.getWindow());

        SDL_Delay(10);
        t_s += 1;
        
    }

    window.terminate();

    SDL_Quit();
    

    std::cout << t.stop() << "ms\n";
    return 0;
}

