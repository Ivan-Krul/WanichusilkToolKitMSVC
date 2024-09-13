#pragma once
#include "SDL_image.h"

namespace resdl_mean_lib {
    class Texture {
    public:
        inline Texture() = default;
        inline Texture(const char* src, SDL_Renderer* renderer) { create(src, renderer); }
        void   create(const char* src, SDL_Renderer* renderer);

        inline SDL_Texture* getTexture() noexcept { return mTexture; }
        inline SDL_Rect     getRectFrom() const noexcept { return mRectFrom; }
        inline SDL_Rect     getRectTo()   const noexcept { return mRectTo;   }

        void   clear();
        inline ~Texture() { clear(); }
    private:
        SDL_Texture* mTexture;
        SDL_Rect     mRectFrom;
        SDL_Rect     mRectTo;

    };
}
