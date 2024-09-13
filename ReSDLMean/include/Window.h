#pragma once
#include <functional>
#include <string>

#include "SDL.h"
#undef main

#include "SDLRoot.h"

namespace resdl_mean_lib
{
	class BasicWindow {
	public:
		inline BasicWindow() noexcept = default;

		bool create(const char* name, int width, int height, SDL_WindowFlags flags = SDL_WINDOW_SHOWN) noexcept;

		inline int getWidth() const noexcept { return mWindowWidth; }
		inline int getHeight() const noexcept { return mWindowHeight; }

		void setOnCreate(void(*onCreate)(void)) { mfOnCreate = onCreate; }
		void setOnDestroy(void(*onDestroy)(void)) { mfOnDestroy = onDestroy; }

		SDL_Window* getWindow() { return mpWindow; }

		void terminate();

		~BasicWindow() {
			terminate();
		}
	protected:
		virtual bool createSurface() = 0;
		virtual void terminateSurface() = 0;

	protected:
		SDL_Window* mpWindow = nullptr;

		int mWindowWidth = -1;
		int mWindowHeight = -1;

		char* mWindowName = nullptr;

		void(*mfOnCreate)(void) = nullptr;
		void(*mfOnDestroy)(void) = nullptr;
	};

	class WindowCPU : public BasicWindow
	{
	public:
		inline WindowCPU() noexcept = default;
		WindowCPU(const char* name, int width, int height, SDL_WindowFlags flags = SDL_WINDOW_SHOWN);

		SDL_Surface* getWindowSufrace() { return mpWindowSurface; }
	public:
		bool createSurface() override;
		void terminateSurface() override;

	private:
		SDL_Surface* mpWindowSurface = nullptr;
	};

	class WindowGPU : public BasicWindow {
	public:
		inline WindowGPU() noexcept = default;
		WindowGPU(const char* name, int width, int height, SDL_WindowFlags flags = SDL_WINDOW_SHOWN);

		SDL_Renderer* getWindowSufrace() { return mpRenderer; }
	public:
		bool createSurface() override;
		void terminateSurface() override;

	private:
		SDL_Renderer* mpRenderer = nullptr;
	};

	using Window = WindowCPU;
}
