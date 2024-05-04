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

	VkExtent2D getExtent();
	bool wasWindowResize();
	void resetWindowResizedFlag();
	
	GLFWwindow* getGLFWwindow() const;
	
	void createWindowSurface(VkInstance instance, VkSurfaceKHR *surface);
	
private:
	static void framebufferResizeCallback (GLFWwindow *window, int width, int height);
	void initWindow();
	
	int m_width;
	int m_height;
	bool m_framebufferResized = false;
	
	std::string m_windowName;
	GLFWwindow* m_window;
};
