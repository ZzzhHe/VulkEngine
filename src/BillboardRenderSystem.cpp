#include "BillboardRenderSystem.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "glm/glm.hpp"
#include "glm/gtc/constants.hpp"

#include <iostream>
#include <filesystem>
#include <stdexcept>

BillboardRenderSystem::BillboardRenderSystem(Device& device, VkRenderPass renderPass, VkDescriptorSetLayout globalDescriptorSetLayout)
: m_device(device) {
	createPipelineLayout(globalDescriptorSetLayout);
	createPipeline(renderPass);
}

BillboardRenderSystem::~BillboardRenderSystem() {
	vkDestroyPipelineLayout(m_device.device(), m_pipelineLayout, nullptr);
}

void BillboardRenderSystem::createPipelineLayout(VkDescriptorSetLayout globalDescriptorSetLayout) {
	
//	VkPushConstantRange pushConstantRange{};
//	pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT;
//	pushConstantRange.offset = 0;
//	pushConstantRange.size = sizeof(SimplePushConstantData);
	
	std::vector<VkDescriptorSetLayout> descriptorSetLayouts{globalDescriptorSetLayout};
	
	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = static_cast<uint32_t>(descriptorSetLayouts.size());
	pipelineLayoutInfo.pSetLayouts = descriptorSetLayouts.data();
//	pipelineLayoutInfo.pushConstantRangeCount = 1;
//	pipelineLayoutInfo.pPushConstantRanges = &pushConstantRange;
	
	pipelineLayoutInfo.pushConstantRangeCount = 0;
	pipelineLayoutInfo.pPushConstantRanges = nullptr;
	
	if (vkCreatePipelineLayout(m_device.device(), &pipelineLayoutInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create pipeline layout!");
	}
}

void BillboardRenderSystem::createPipeline(VkRenderPass renderPass) {
	assert(m_pipelineLayout != nullptr && "Cannot create pipeline before pipeline");
	
	PipelineConfigInfo pipelineConfig{};
	Pipeline::defaultPipelineConfigInfo(pipelineConfig);
	pipelineConfig.renderPass = renderPass;
	pipelineConfig.pipelineLayout = m_pipelineLayout;
	m_pipeline = std::make_unique<Pipeline>(
	   m_device,
	   "shaders/point_light.vert.spv",
	   "shaders/point_light.frag.spv",
	   pipelineConfig
   );
}


void BillboardRenderSystem::render(FrameInfo& frameInfo){
	
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
	 
	vkCmdDraw(frameInfo.commandBuffer, 6, 1, 0, 0);
}
