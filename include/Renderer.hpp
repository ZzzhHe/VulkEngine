#pragma once

#include "Window.hpp"
#include "Device.hpp"
#include "SwapChain.hpp"

#include <memory>
#include <vector>

class Renderer {
public:
	
	Renderer(Window &window, Device &device);
	~Renderer();
	
	Renderer(const Renderer&) = delete;
	Renderer &operator=(const Renderer&) = delete;
	
	VkCommandBuffer beginFrame();
	void endFrame();
	void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
	void endSwapChainRenderPass(VkCommandBuffer commandBuffer);
	
	bool isFrameInProgress() const;
	VkRenderPass getSwapChainRenderPass() const;
	VkCommandBuffer getCurrentCommandBuffer() const;
	int getFrameIndex() const;
	
private:
	void createCommandBuffers();
	void freeCommandBuffers();
	void drawFrame();
	void recreateSwapChain();
	
	Window& m_window;
	Device& m_device;
	std::unique_ptr<SwapChain> m_swapChain;
	std::vector<VkCommandBuffer> m_commandBuffers;
	
	uint32_t m_currentImageIndex{0};
	int m_currentFrameIndex{0};
	bool m_isFrameStarted{false};
};
