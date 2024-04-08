#include "Window.hpp"

#include <stdexcept>

Window::Window(int w, int h, std::string s)
: m_width(w), m_height(h), m_windowName(s) {
	initWindow();
}

Window::~Window() {
	glfwDestroyWindow(m_window);
	glfwTerminate();
}

void Window::initWindow() {
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); // tell glfw to not use OPENGL
	glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
	
	m_window = glfwCreateWindow(m_width, m_height, m_windowName.c_str(), nullptr, nullptr);
	glfwSetWindowUserPointer(m_window, this); // to parents 
	glfwSetFramebufferSizeCallback(m_window, framebufferResizeCallback);
}

void Window::createWindowSurface(VkInstance instance, VkSurfaceKHR *surface) {
	if (glfwCreateWindowSurface(instance, m_window, nullptr, surface) != VK_SUCCESS) {
		throw std::runtime_error("failed to create the window surface");
	}
}


void Window::framebufferResizeCallback (GLFWwindow *window, int width, int height) {
	auto resize_window = reinterpret_cast<Window *>(glfwGetWindowUserPointer(window));
	resize_window->m_framebufferResized = true;
	resize_window->m_width = width;
	resize_window->m_height = height;
}
