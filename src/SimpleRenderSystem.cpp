#include "SimpleRenderSystem.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"

#include <iostream>
#include <filesystem>
#include <stdexcept>

struct SimplePushConstantData {
	glm::mat4 ModelMatrix{1.f};
	glm::mat4 normalTransformMatrix{1.f};
};

SimpleRenderSystem::SimpleRenderSystem(Device& device, VkRenderPass renderPass, VkDescriptorSetLayout globalDescriptorSetLayout)
: m_device(device) {
	createPipelineLayout(globalDescriptorSetLayout);
	createPipeline(renderPass);
}

SimpleRenderSystem::~SimpleRenderSystem() {
	vkDestroyPipelineLayout(m_device.device(), m_pipelineLayout, nullptr);
}

void SimpleRenderSystem::createPipelineLayout(VkDescriptorSetLayout globalDescriptorSetLayout) {
	
	VkPushConstantRange pushConstantRange{};
	pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
	pushConstantRange.offset = 0;
	pushConstantRange.size = sizeof(SimplePushConstantData);
	
	std::vector<VkDescriptorSetLayout> descriptorSetLayouts{globalDescriptorSetLayout};
	
	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
	pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
	pipelineLayoutInfo.pushConstantRangeCount = 1;
	pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
	if (vkCreatePipelineLayout(m_device.device(), &pipelineLayoutInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create pipeline layout!");
	}
}

void SimpleRenderSystem::createPipeline(VkRenderPass renderPass) {
	assert(m_pipelineLayout != nullptr && "Cannot create pipeline before pipeline");
	
	PipelineConfigInfo pipelineConfig{};
	Pipeline::defaultPipelineConfigInfo(pipelineConfig);
	pipelineConfig.renderPass = renderPass;
	pipelineConfig.pipelineLayout = m_pipelineLayout;
	m_pipeline = std::make_unique<Pipeline>(
	   m_device,
	   "shaders/simple_shader.vert.spv",
	   "shaders/simple_shader.frag.spv",
	   pipelineConfig
   );
}


void SimpleRenderSystem::renderGameObjects(
	   FrameInfo& frameInfo){
	
	m_pipeline->bind(frameInfo.commandBuffer);
	
	vkCmdBindDescriptorSets(
		frameInfo.commandBuffer,
		VK_PIPELINE_BIND_POINT_GRAPHICS,
		m_pipelineLayout,
		0,
		1,
		&frameInfo.globalDescriptorSet,
		0,
		nullptr);
	 
	for (auto& kv : frameInfo.gameObjects) {
		
		auto& obj = kv.second;
		
		SimplePushConstantData push {};
		
		push.ModelMatrix = obj.transform.mat4();
		push.normalTransformMatrix = obj.transform.normalTransformMatrix();
		
		vkCmdPushConstants(
		   frameInfo.commandBuffer,
		   m_pipelineLayout,
		   VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT,
		   0,
		   sizeof(SimplePushConstantData),
		   &push);
		
		obj.model->bind(frameInfo.commandBuffer);
		obj.model->draw(frameInfo.commandBuffer);
	}
}
