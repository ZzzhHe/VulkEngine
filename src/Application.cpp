#include "Application.hpp"

void Application::run() {
	while (!m_appWindow.shouldClose()) {
		glfwPollEvents();
	}
}
