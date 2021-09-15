#ifndef _WINDOW_H
#define _WINDOW_H

#include <atomic>

/**
 * Window to render stuff into.
 * Handles OpenGL initialization and is an OpenGL context.
*/
class Window
{
private:
	static std::atomic<bool> m_initialized;

public:
	Window();

};

#endif // _WINDOW_H