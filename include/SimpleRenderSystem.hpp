#pragma once

#include "Camera.hpp"
#include "Pipeline.hpp"
#include "Device.hpp"
#include "GameObject.hpp"
#include "FrameInfo.hpp"

#include <memory>
#include <vector>

class SimpleRenderSystem {
public:
	
	SimpleRenderSystem(Device& device, VkRenderPass renderPass, VkDescriptorSetLayout globalDescriptorSetLayout);
	~SimpleRenderSystem();
	
	SimpleRenderSystem(const SimpleRenderSystem&) = delete;
	SimpleRenderSystem &operator=(const SimpleRenderSystem&) = delete;
	void renderGameObjects(FrameInfo& frameInfo);
	
private:
	void createPipelineLayout(VkDescriptorSetLayout globalDescriptorSetLayout);
	void createPipeline(VkRenderPass renderPass);
	
	
	Device &m_device;
	std::unique_ptr<Pipeline> m_pipeline;
	VkPipelineLayout m_pipelineLayout;
};
