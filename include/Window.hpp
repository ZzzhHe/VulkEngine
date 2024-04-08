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
	
	bool shouldClose() { return glfwWindowShouldClose(m_window); }

	VkExtent2D getExtent() { return { static_cast<uint32_t>(m_width), static_cast<uint32_t>(m_height) }; }
	bool wasWindowResize() { return m_framebufferResized; }
	void resetWindowResizedFlag() { m_framebufferResized = false; }
	
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
