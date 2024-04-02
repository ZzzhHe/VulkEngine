#include "Application.hpp"
#include <iostream>
#include <filesystem>

void Application::run() {
	while (!m_appWindow.shouldClose()) {
		glfwPollEvents();
	}
}
