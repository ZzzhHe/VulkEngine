#pragma once

#include "Camera.hpp"
#include "Pipeline.hpp"
#include "Device.hpp"
#include "GameObject.hpp"

#include <memory>
#include <vector>

class SimpleRenderSystem {
public:
	
	SimpleRenderSystem(Device& device, VkRenderPass renderPass);
	~SimpleRenderSystem();
	
	SimpleRenderSystem(const SimpleRenderSystem&) = delete;
	SimpleRenderSystem &operator=(const SimpleRenderSystem&) = delete;
	void renderGameObjects(
		   VkCommandBuffer commandBuffer,
		   std::vector<GameObject>& gameObjects,
		   const Camera &camera);
	
private:
	void createPipelineLayout();
	void createPipeline(VkRenderPass renderPass);
	
	
	Device &m_device;
	std::unique_ptr<Pipeline> m_pipeline;
	VkPipelineLayout m_pipelineLayout;
};
