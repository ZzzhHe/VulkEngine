#include "Application.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"

#include <iostream>
#include <filesystem>
#include <stdexcept>

struct SimplePushConstantData {
	glm::mat2 transform{1.f};
	glm::vec2 offset;
	alignas(16) glm::vec3 color; // align requirement
};

Application::Application() {
	loadGameObject();
	createPipelineLayout();
	createPipeline();
	
}

Application::~Application() {
	vkDestroyPipelineLayout(m_device.device(), m_pipelineLayout, nullptr);
}

void Application::run() {
	while (!m_window.shouldClose()) {
		glfwPollEvents();
		
		if (auto commandBuffer = m_renderer.beginFrame()) {
			m_renderer.beginSwapChainRenderPass(commandBuffer);
			renderGameObjects(commandBuffer);
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

void Application::createPipelineLayout() {
	
	VkPushConstantRange pushConstantRange{};
	pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	pushConstantRange.offset = 0;
	pushConstantRange.size = sizeof(SimplePushConstantData);
	
	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 0;
	pipelineLayoutInfo.pSetLayouts = nullptr;
	pipelineLayoutInfo.pushConstantRangeCount = 1;
	pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
	if (vkCreatePipelineLayout(m_device.device(), &pipelineLayoutInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create pipeline layout!");
	}
}

void Application::createPipeline() {
	assert(m_pipelineLayout != nullptr && "Cannot create pipeline before pipeline");
	
	PipelineConfigInfo pipelineConfig{};
	Pipeline::defaultPipelineConfigInfo(pipelineConfig);
	pipelineConfig.renderPass = m_renderer.getSwapChainRenderPass();
	pipelineConfig.pipelineLayout = m_pipelineLayout;
	m_pipeline = std::make_unique<Pipeline>(
	   m_device,
	   "shaders/simple_shader.vert.spv",
	   "shaders/simple_shader.frag.spv",
	   pipelineConfig
   );
}


void Application::renderGameObjects(VkCommandBuffer commandBuffer) {
	m_pipeline->bind(commandBuffer);
	 
	for (auto& obj : m_gameObjects) {
		obj.transform2d.rotation = glm::mod(obj.transform2d.rotation + .01f, glm::two_pi<float>());
		SimplePushConstantData push {};
		push.offset = obj.transform2d.translation;
		push.color = obj.color;
		push.transform = obj.transform2d.mat2();
		
		vkCmdPushConstants(
		   commandBuffer,
		   m_pipelineLayout,
		   VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
		   0,
		   sizeof(SimplePushConstantData),
		   &push);
		
		obj.model->bind(commandBuffer);
		obj.model->draw(commandBuffer);
	}
}
