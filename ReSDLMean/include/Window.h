#pragma once
#include <functional>
#include <string>

#include "SDL.h"

namespace graphic_system_lib
{
	class Window
	{
	public:
		inline Window() noexcept = default;
		Window(const char* name, int width, int height);

		bool create(const char* name, int width, int height) noexcept;


		inline int getWidth() const noexcept { return mpWindowSurface ? mpWindowSurface->w : -1; }
		inline int getHeight() const noexcept { return mpWindowSurface ? mpWindowSurface->h : -1; }


		void setOnCreate(void(*onCreate)(void)) { mfOnCreate = onCreate; }
		void setOnDestroy(void(*onDestroy)(void)) { mfOnDestroy = onDestroy; }

		SDL_Window* getWindow() { return mpWindow; }
		SDL_Surface* getWindowSufracef() { return mpWindowSurface; }

		void terminate();

		~Window() {
			terminate();
		}
	private:
		SDL_Window* mpWindow = nullptr;
		SDL_Surface* mpWindowSurface = nullptr;

		char* mWindowName = nullptr;

		void(*mfOnCreate)(void) = nullptr;
		void(*mfOnDestroy)(void) = nullptr;
	};
}
