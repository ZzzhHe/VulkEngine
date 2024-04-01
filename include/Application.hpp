#pragma once

#include "Window.hpp"

class Application {
public:
	static constexpr int WIDTH = 800;
	static constexpr int HEIGHT = 600;
	
	void run();
private:
	Window m_appWindow {WIDTH, HEIGHT, "VulkEngine!"};
};
