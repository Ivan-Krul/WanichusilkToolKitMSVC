#include "SDLRoot.h"

namespace resdl_mean_lib {
    bool SDLRoot::init(Uint32 init_parts) {
        return mInitParts = SDL_Init(init_parts);
    }

    void SDLRoot::quit() {
        SDL_Quit();
    }

    SDLRoot SDLRoot::mInstance;
}
