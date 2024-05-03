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

std::unique_ptr<Model> createCubeModel(Device& device, glm::vec3 offset) {
  std::vector<Model::Vertex> vertices{
 
	  // left face (white)
	  {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
	  {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
	  {{-.5f, -.5f, .5f}, {.9f, .9f, .9f}},
	  {{-.5f, -.5f, -.5f}, {.9f, .9f, .9f}},
	  {{-.5f, .5f, -.5f}, {.9f, .9f, .9f}},
	  {{-.5f, .5f, .5f}, {.9f, .9f, .9f}},
 
	  // right face (yellow)
	  {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
	  {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
	  {{.5f, -.5f, .5f}, {.8f, .8f, .1f}},
	  {{.5f, -.5f, -.5f}, {.8f, .8f, .1f}},
	  {{.5f, .5f, -.5f}, {.8f, .8f, .1f}},
	  {{.5f, .5f, .5f}, {.8f, .8f, .1f}},
 
	  // top face (orange, remember y axis points down)
	  {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
	  {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
	  {{-.5f, -.5f, .5f}, {.9f, .6f, .1f}},
	  {{-.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
	  {{.5f, -.5f, -.5f}, {.9f, .6f, .1f}},
	  {{.5f, -.5f, .5f}, {.9f, .6f, .1f}},
 
	  // bottom face (red)
	  {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
	  {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
	  {{-.5f, .5f, .5f}, {.8f, .1f, .1f}},
	  {{-.5f, .5f, -.5f}, {.8f, .1f, .1f}},
	  {{.5f, .5f, -.5f}, {.8f, .1f, .1f}},
	  {{.5f, .5f, .5f}, {.8f, .1f, .1f}},
 
	  // nose face (blue)
	  {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
	  {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
	  {{-.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
	  {{-.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
	  {{.5f, -.5f, 0.5f}, {.1f, .1f, .8f}},
	  {{.5f, .5f, 0.5f}, {.1f, .1f, .8f}},
 
	  // tail face (green)
	  {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
	  {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
	  {{-.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
	  {{-.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
	  {{.5f, -.5f, -0.5f}, {.1f, .8f, .1f}},
	  {{.5f, .5f, -0.5f}, {.1f, .8f, .1f}},
 
  };
  for (auto& v : vertices) {
	v.position += offset;
  }
  return std::make_unique<Model>(device, vertices);
}


void Application::loadGameObject() {
	std::shared_ptr<Model> model = createCubeModel(m_device, {.0f, .0f, .0f});
	// one model can be used in multiple game objects
   
	auto cube = GameObject::createGameObject();
	cube.model = model;
	cube.transform.translation = {.0f, .0f, .5f};
	cube.transform.scale = {.5f, .5f, .5f};
	
	m_gameObjects.push_back(std::move(cube));
}
