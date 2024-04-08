#pragma once

#include "Window.hpp"
#include "Pipeline.hpp"
#include "Device.hpp"
#include "SwapChain.hpp"
#include "Model.hpp"

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
	void loadModel();
	void createPipelineLayout();
	void createPipeline();
	void createCommandBuffers();
	void drawFrame();
	void recreateSwapChain();
	void recordCommandBuffer(int imageIndex);
	
	Window m_window {WIDTH, HEIGHT, "VulkEngine!"};
	Device m_device {m_window};
	std::unique_ptr<SwapChain> m_swapChain;
	std::unique_ptr<Pipeline> m_pipeline;
	VkPipelineLayout m_pipelineLayout;
	std::vector<VkCommandBuffer> m_commandBuffers;
	std::unique_ptr<Model> m_model;
};
