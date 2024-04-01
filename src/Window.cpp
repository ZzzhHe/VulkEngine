#include "Window.hpp"

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
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE); // not use glfw resize
	
	m_window = glfwCreateWindow(m_width, m_height, m_windowName.c_str(), nullptr, nullptr);
}

bool Window::shouldClose() {
	return glfwWindowShouldClose(m_window);
}
