#include "Window.h"
#include <stdexcept>

namespace resdl_mean_lib
{
	bool BasicWindow::create(const char* name, int width, int height, SDL_WindowFlags flags) noexcept
	{
		if (SDLRoot::getInstance() & (Uint32)SDLRoot::InitPart::video) return false;

#pragma warning(suppress : 4554)
		if (width >> sizeof(width) * 8 - 1) width = ((width << 1) >> 1);
#pragma warning(suppress : 4554)
		if (height >> sizeof(height) * 8 - 1) height = ((height << 1) >> 1);

		auto name_len = strlen(name);
		if(mWindowName == nullptr)
			mWindowName = new char[name_len + 1];
		else if (strlen(mWindowName) != name_len) {
			delete[] mWindowName;
			mWindowName = new char[name_len + 1];
		}

		memcpy(mWindowName, name, name_len);
		mWindowName[name_len] = 0;

		//Create window
		if ((mpWindow = SDL_CreateWindow(mWindowName, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags)) == nullptr)
			return false;
		else
			if(!createSurface()) return false; // Get window surface

		if(mfOnCreate)
			mfOnCreate();

		
		return true;
	}

	void BasicWindow::terminate() {
		if (mpWindow == nullptr) return;

		SDL_DestroyWindow(mpWindow);

		if (mWindowName)
			delete[] mWindowName;

		terminateSurface();
		mpWindow = nullptr;
		mWindowName = nullptr;

		if (mfOnDestroy)
			mfOnDestroy();
	}

	WindowCPU::WindowCPU(const char* name, int width, int height, SDL_WindowFlags flags) {
		if (!create(name, width, height, flags))
			throw std::runtime_error((std::string("You can't create a window: ") + SDL_GetError()).c_str());

	}

	bool WindowCPU::createSurface() {
		return (mpWindowSurface = SDL_GetWindowSurface(mpWindow));
	}

	void WindowCPU::terminateSurface() {
		mpWindowSurface = nullptr;
	}


	WindowGPU::WindowGPU(const char* name, int width, int height, SDL_WindowFlags flags) {
		if (!create(name, width, height, flags))
			throw std::runtime_error((std::string("You can't create a window: ") + SDL_GetError()).c_str());

	}

	bool WindowGPU::createSurface() {
		return (mpRenderer = SDL_GetRenderer(mpWindow)) == nullptr;
	}

	void WindowGPU::terminateSurface() {
		SDL_DestroyRenderer(mpRenderer);
		mpRenderer = nullptr;
	}
}
