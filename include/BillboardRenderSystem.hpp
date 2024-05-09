#pragma once

#include "Camera.hpp"
#include "Pipeline.hpp"
#include "Device.hpp"
#include "GameObject.hpp"
#include "FrameInfo.hpp"

#include <memory>
#include <vector>

class BillboardRenderSystem {
public:
	
	BillboardRenderSystem(Device& device, VkRenderPass renderPass, VkDescriptorSetLayout globalDescriptorSetLayout);
	~BillboardRenderSystem();
	
	BillboardRenderSystem(const BillboardRenderSystem&) = delete;
	BillboardRenderSystem &operator=(const BillboardRenderSystem&) = delete;
	void render(FrameInfo& frameInfo);
	
private:
	void createPipelineLayout(VkDescriptorSetLayout globalDescriptorSetLayout);
	void createPipeline(VkRenderPass renderPass);
	
	
	Device &m_device;
	std::unique_ptr<Pipeline> m_pipeline;
	VkPipelineLayout m_pipelineLayout;
};
