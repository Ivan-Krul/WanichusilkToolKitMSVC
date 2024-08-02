#include "Window.h"
#include <stdexcept>

namespace graphic_system_lib
{
	Window::Window(const char* name, size_t width, size_t height) {
		mpWindow = nullptr;
		mpWindowSurface = nullptr;
		if (!create(name, width, height))
			throw std::runtime_error((std::string("You can't create a window: ") + SDL_GetError()).c_str());

	}

	bool Window::create(const char* name, size_t width, size_t height) noexcept
	{
		mWindowName = name;

		//Create window
		if ((mpWindow = SDL_CreateWindow(mWindowName.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN)) == nullptr)
			return false;
		else
			if((mpWindowSurface = SDL_GetWindowSurface(mpWindow)) == nullptr)
				return false; //Get window surface

		if(mfOnCreate)
			mfOnCreate();
		return true;
	}

	void Window::terminate()
	{
		if (mpWindow == nullptr) return;

		SDL_FreeSurface(mpWindowSurface);
		SDL_DestroyWindow(mpWindow);

		mpWindowSurface = nullptr;
		mpWindow = nullptr;

		if (mfOnDestroy)
			mfOnDestroy();
	}
}
