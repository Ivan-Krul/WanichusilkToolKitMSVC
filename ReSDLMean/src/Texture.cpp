#include "Texture.h"
#include <stdexcept>

namespace resdl_mean_lib {
    void Texture::create(const char* src, SDL_Renderer* renderer) {
        SDL_Surface* surf = IMG_Load(src);
        if (surf == NULL) {
            mTexture = NULL;
            throw std::runtime_error("Texture haven't loaded properly");
        } else {
            mTexture = SDL_CreateTextureFromSurface(renderer, surf);
            SDL_FreeSurface(surf);

            if (mTexture == NULL) throw std::runtime_error("Texture haven't loaded properly");
        }
    }

    void Texture::clear() {
        SDL_Texture
    }
}
