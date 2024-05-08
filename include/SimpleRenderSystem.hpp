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
	
	SimpleRenderSystem(Device& device, VkRenderPass renderPass);
	~SimpleRenderSystem();
	
	SimpleRenderSystem(const SimpleRenderSystem&) = delete;
	SimpleRenderSystem &operator=(const SimpleRenderSystem&) = delete;
	void renderGameObjects(FrameInfo& frameInfo, std::vector<GameObject>& gameObjects);
	
private:
	void createPipelineLayout();
	void createPipeline(VkRenderPass renderPass);
	
	
	Device &m_device;
	std::unique_ptr<Pipeline> m_pipeline;
	VkPipelineLayout m_pipelineLayout;
};
