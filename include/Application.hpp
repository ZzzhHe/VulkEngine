#pragma once

#include "Window.hpp"
#include "Pipeline.hpp"
#include "Device.hpp"
#include "SwapChain.hpp"

#include <memory>
#include <vector>

class Application {
public:
	static constexpr int WIDTH = 800;
	static constexpr int HEIGHT = 600;
	
	Application();
	~Application();
	
	Application(const Application&) = delete;
	Application &operator=(const Application&) = delete;
	
	void run();
	
private:
	void createPipelineLayout();
	void createPipeline();
	void createCommandBuffers();
	void drawFrame();
	
	Window m_window {WIDTH, HEIGHT, "VulkEngine!"};
	Device m_device {m_window};
	SwapChain m_swapChain {m_device, m_window.getExtent()};
	std::unique_ptr<Pipeline> m_pipeline;
	VkPipelineLayout m_pipelineLayout;
	std::vector<VkCommandBuffer> m_commandBuffers;
};
