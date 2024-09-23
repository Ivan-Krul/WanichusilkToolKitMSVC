#pragma once
#include "SDL_image.h"

namespace resdl_mean_lib {
    class Texture {
    public:
        inline Texture() = default;
        inline Texture(const char* src, SDL_Renderer* renderer) { create(src, renderer); }
        void   create(const char* src, SDL_Renderer* renderer);

        inline void setWidth(int w) { mRectTo.w = w; }
        inline void setHeight(int h) { mRectTo.h = h; }
        inline void setOffsetX(int x) { mRectFrom.x = x; }
        inline void setOffsetY(int y) { mRectFrom.y = y; }

        inline SDL_Texture* getTexture()        noexcept { return mTexture; }
        inline SDL_Rect     getRectFrom() const noexcept { return mRectFrom; }
        inline SDL_Rect     getRectTo()   const noexcept { return mRectTo;   }

        void render();

        void   clear();
        inline ~Texture() { clear(); }
    private:
        SDL_Texture* mTexture = NULL;
        SDL_Rect     mRectFrom;
        SDL_Rect     mRectTo;

        SDL_Renderer* mRendererOrigin = NULL;

    };
}
