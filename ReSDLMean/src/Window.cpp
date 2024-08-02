#include "Window.h"
#include <stdexcept>

namespace graphic_system_lib
{
	Window::Window(const char* name, int width, int height) {
		mpWindow = nullptr;
		mpWindowSurface = nullptr;
		mWindowName = nullptr;
		if (!create(name, width, height))
			throw std::runtime_error((std::string("You can't create a window: ") + SDL_GetError()).c_str());

	}

	bool Window::create(const char* name, int width, int height) noexcept
	{
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
		if ((mpWindow = SDL_CreateWindow(mWindowName, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, SDL_WINDOW_SHOWN)) == nullptr)
			return false;
		else
			if((mpWindowSurface = SDL_GetWindowSurface(mpWindow)) == nullptr) return false; // Get window surface

		if(mfOnCreate)
			mfOnCreate();
		return true;
	}

	void Window::terminate()
	{
		if (mpWindow == nullptr) return;

		SDL_FreeSurface(mpWindowSurface);
		SDL_DestroyWindow(mpWindow);

		if(mWindowName)
			delete[] mWindowName;

		mpWindowSurface = nullptr;
		mpWindow = nullptr;
		mWindowName = nullptr;

		if (mfOnDestroy)
			mfOnDestroy();
	}
}
