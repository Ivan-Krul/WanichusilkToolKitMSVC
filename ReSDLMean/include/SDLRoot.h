#pragma once
#include "SDL.h"

namespace resdl_mean_lib {
    class SDLRoot {
    public:
        enum class InitPart : Uint32 {
        timer = SDL_INIT_TIMER,
        audio = SDL_INIT_AUDIO,
        video = SDL_INIT_VIDEO,                    /**< SDL_INIT_VIDEO implies SDL_INIT_EVENTS */
        joystick = SDL_INIT_JOYSTICK,              /**< SDL_INIT_JOYSTICK implies SDL_INIT_EVENTS */
        haptic = SDL_INIT_HAPTIC,
        gamecontroller = SDL_INIT_GAMECONTROLLER,  /**< SDL_INIT_GAMECONTROLLER implies SDL_INIT_JOYSTICK */
        events = SDL_INIT_EVENTS,
        sensor = 0x00008000u,                      /* istg it was defined */
        noparachute = SDL_INIT_NOPARACHUTE,        /**< compatibility; this flag is ignored. */
        everything = (\
            timer | audio | video | events | \
            joystick | haptic | gamecontroller | sensor \
            )

        };

        inline bool init(Uint32 init_parts = ((Uint32)InitPart::events | (Uint32)InitPart::video));
        inline void quit();

        inline Uint32 getInitPart() const noexcept { return mInitParts; }
        inline operator Uint32() const noexcept { return getInitPart(); }
        inline ~SDLRoot() { quit(); }

        inline static SDLRoot& getInstance() { return mInstance; }

    private:
        bool           mIsQuit = false;
        Uint32         mInitParts = 0;

        static SDLRoot mInstance;
        SDLRoot() = default;

    };
}
