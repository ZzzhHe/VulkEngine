#include "Application.hpp"

#include "SimpleRenderSystem.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"

#include <iostream>
#include <filesystem>
#include <stdexcept>

Application::Application() {
	loadGameObject();
}

Application::~Application() { }

void Application::run() {
	
	SimpleRenderSystem simpleRenderSystem {m_device, m_renderer.getSwapChainRenderPass()};
	
	while (!m_window.shouldClose()) {
		glfwPollEvents();
		
		if (auto commandBuffer = m_renderer.beginFrame()) {
			m_renderer.beginSwapChainRenderPass(commandBuffer);
			simpleRenderSystem.renderGameObjects(commandBuffer, m_gameObjects);
			m_renderer.endSwapChainRenderPass(commandBuffer);
			m_renderer.endFrame();
		}
		
		vkDeviceWaitIdle(m_device.device()); // fix: All submitted commands that refer to pipeline must have completed execution
	}
}

void Application::loadGameObject() {
	std::vector<Model::Vertex> vertices {
		{{0.0, -0.5}, {1.0f, 0.0f, 0.0f}},
		{{0.5, 0.5}, {0.0f, 1.0f, 0.0f}},
		{{-0.5, 0.5}, {0.0f, 0.0f, 1.0f}}
	};
	
	auto model = std::make_shared<Model>(m_device, vertices); // one model can be used in multiple game objects
	
	auto triangle = GameObject::createGameObject();
	triangle.model = model;
	triangle.color = {.1f, .5f, .1f};
	triangle.transform2d.translation.x = .2f;
	triangle.transform2d.scale = {2.0f, 0.5f};
	triangle.transform2d.rotation = .45f * glm::two_pi<float>();
	
	m_gameObjects.push_back(std::move(triangle));
}
