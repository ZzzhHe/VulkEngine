#include "Application.hpp"

#include "SimpleRenderSystem.hpp"
#include "KeyboardController.hpp"
#include "Camera.hpp"
#include "Buffer.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"

#include <iostream>
#include <chrono>
#include <filesystem>
#include <stdexcept>

struct GlobalUniform { // Global Uniform Buffer Object
	glm::mat4 projectionView{1.f};
	glm::vec3 lightDirection = glm::normalize(glm::vec3{1.f, -3.f, -1.f});
};

Application::Application() {
	loadGameObject();
}

Application::~Application() { }

void Application::run() {
	
	std::vector<std::unique_ptr<Buffer>> uboBuffers(SwapChain::MAX_FRAMES_IN_FLIGHT);
	
	for (int i = 0; i < uboBuffers.size(); i ++) {
		uboBuffers[i] = std::make_unique<Buffer>(
			m_device,
			sizeof(GlobalUniform),
			1,
			VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
			VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
		uboBuffers[i]->map();
	}
	
	SimpleRenderSystem simpleRenderSystem {m_device, m_renderer.getSwapChainRenderPass()};
	
	Camera camera{};
//	camera.setViewDirection(glm::vec3(0.0f), glm::vec3(0.5f, 0.0f, 1.0f));
	camera.setViewTarget(glm::vec3(-1.0f, -2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 2.5f));
	
	auto viewerObject = GameObject::createGameObject(); // store camera current state
	KeyboardController cameraController{};
	
	auto currentTime = std::chrono::high_resolution_clock::now();
	
	while (!m_window.shouldClose()) {
		glfwPollEvents();
		
		auto newTime = std::chrono::high_resolution_clock::now();
		float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
		currentTime = newTime;
		
		cameraController.moveInPlaneXZ(m_window.getGLFWwindow(), frameTime, viewerObject);
		camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);
		
		float aspect = m_renderer.getAspectRatio();
		camera.setPerspectProjection(glm::radians(60.f), aspect, 0.1f, 10.f);
		
		if (auto commandBuffer = m_renderer.beginFrame()) {
			int frameIndex = m_renderer.getFrameIndex();
			
			FrameInfo frameInfo {
				frameIndex,
				frameTime,
				commandBuffer,
				camera
			};
			
			// update
			GlobalUniform ubo{};
			ubo.projectionView = camera.getProjection() * camera.getView();
			uboBuffers[frameIndex]->writeToBuffer(&ubo);
			uboBuffers[frameIndex]->flush();
			
			// render
			m_renderer.beginSwapChainRenderPass(commandBuffer);
			simpleRenderSystem.renderGameObjects(frameInfo, m_gameObjects);
			m_renderer.endSwapChainRenderPass(commandBuffer);
			m_renderer.endFrame();
		}
		
		vkDeviceWaitIdle(m_device.device()); // fix: All submitted commands that refer to pipeline must have completed execution
	}
}




void Application::loadGameObject() {
	std::shared_ptr<Model> model = Model::createModelFromFile(m_device, "models/flat_vase.obj");
	// one model can be used in multiple game objects
   
	auto flatVase = GameObject::createGameObject();
	flatVase.model = model;
	flatVase.transform.translation = {-.5f, .5f, 2.5f};
	flatVase.transform.scale = {.5f, .5f, .5f};
	
	m_gameObjects.push_back(std::move(flatVase));
	
	model = Model::createModelFromFile(m_device, "models/smooth_vase.obj");
	// one model can be used in multiple game objects
   
	auto smoothVase = GameObject::createGameObject();
	smoothVase.model = model;
	smoothVase.transform.translation = {.5f, .5f, 2.5f};
	smoothVase.transform.scale = {.5f, .5f, .5f};
	
	m_gameObjects.push_back(std::move(smoothVase));
}
