#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>

class Window {
public:
	Window(int width, int height, std::string name);
	~Window();
	
	// in case unsafely copy
	Window(const Window &) = delete; // delete default copy constructor
	Window &operator = (const Window &) = delete; // delete default copy assignment operator
	
	bool shouldClose();
	
	void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);
	
private:
	void initWindow();
	std::string m_windowName;
	const int m_width;
	const int m_height;
	GLFWwindow* m_window;
};
