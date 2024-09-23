#include "Texture.h"

#include "SDLRoot.h"
#include <stdexcept>

namespace resdl_mean_lib {
    void Texture::create(const char* src, SDL_Renderer* renderer) {
        if (mTexture) return;

#ifdef PRECHECK_INSTANCE
        if ((SDLRoot::getInstance() & (Uint32)SDLRoot::InitPart::video) == 0)
            throw std::runtime_error("SDL wasn't initialized with video tag");
#endif

        SDL_Surface* surf = IMG_Load(src);
        if (surf == NULL) {
            mTexture = NULL;
            throw std::runtime_error("Texture haven't loaded properly");
        } else {
            mTexture = SDL_CreateTextureFromSurface(renderer, surf);
            mRectTo.w = surf->w;
            mRectTo.h = surf->h;
            SDL_FreeSurface(surf);

            if (mTexture == NULL) throw std::runtime_error("Texture haven't loaded properly");

            mRendererOrigin = renderer;
        }
    }

    void Texture::render() {
        SDL_RenderCopy(mRendererOrigin, mTexture, &mRectFrom, &mRectTo);
    }

    void Texture::clear() {
        SDL_DestroyTexture(mTexture);
        mTexture = NULL;
    }
}
