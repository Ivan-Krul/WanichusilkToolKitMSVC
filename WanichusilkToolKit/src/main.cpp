#include <iostream>
#include <chrono>

#include "Window.h"
#include "Texture.h"

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
        fprintf(stderr, "could not initialize sdl2 : %s\n", SDL_GetError());
        return 1;
    }

    resdl_mean_lib::WindowGPU* window;
    resdl_mean_lib::Texture* tex;

    try {
        window = new resdl_mean_lib::WindowGPU("WTK - test", 640, 480);
        tex = new resdl_mean_lib::Texture("../assets/jania new.png", window->getWindowSufrace());
    }
    catch (const std::string& eror) {
        fprintf(stderr, "could not initialize the project : %s\n", SDL_GetError());
        return 0;
    }

    // wtf is going on with exception handling?????

    auto l = []() {printf("Destroyed\n"); };
    window->setOnDestroy(l);

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


        SDL_RenderClear(window->getWindowSufrace());

        SDL_RenderPresent(window->getWindowSufrace());

        //Update the surface
        //SDL_UpdateWindowSurface(window.getWindow());

        SDL_Delay(10);
        t_s += 1;
        
    }

    window->terminate();

    SDL_Quit();
    

    std::cout << t.stop() << "ms\n";
    return 0;
}

