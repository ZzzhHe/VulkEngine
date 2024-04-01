#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>

class Window {
public:
	Window(int width, int height, std::string name);
	~Window();
	
	Window(const Window &) = delete;
	Window &operator = (const Window &) = delete;
	
	bool shouldClose();
private:
	void initWindow();
	std::string m_windowName;
	const int m_width;
	const int m_height;
	GLFWwindow* m_window;
};
