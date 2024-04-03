#include "Application.hpp"
#include <iostream>
#include <filesystem>
#include <stdexcept>

Application::Application() {
	createPipelineLayout();
	createPipeline();
	createCommandBuffers();
}

Application::~Application() {
	vkDestroyPipelineLayout(m_device.device(), m_pipelineLayout, nullptr);
}

void Application::run() {
	while (!m_window.shouldClose()) {
		glfwPollEvents();
		drawFrame();
		
		vkDeviceWaitIdle(m_device.device()); // fix: All submitted commands that refer to pipeline must have completed execution
	}
}

void Application::createPipelineLayout() {
	VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
	pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutInfo.setLayoutCount = 0;
	pipelineLayoutInfo.pSetLayouts = nullptr;
	pipelineLayoutInfo.pushConstantRangeCount = 0;
	pipelineLayoutInfo.pPushConstantRanges = nullptr;
	if (vkCreatePipelineLayout(m_device.device(), &pipelineLayoutInfo, nullptr, &m_pipelineLayout) != VK_SUCCESS) {
		throw std::runtime_error("failed to create pipeline layout!");
	}
}

void Application::createPipeline() {
	auto pipelineConfig = Pipeline::defaultPipelineConfigInfo(m_swapChain.width(), m_swapChain.height());
	pipelineConfig.renderPass = m_swapChain.getRenderPass();
	pipelineConfig.pipelineLayout = m_pipelineLayout;
	m_pipeline = std::make_unique<Pipeline>(
	   m_device,
	   "shaders/simple_shader.vert.spv",
	   "shaders/simple_shader.frag.spv",
	   pipelineConfig
   );
}

void Application::createCommandBuffers(){
	m_commandBuffers.resize(m_swapChain.imageCount());
	
	VkCommandBufferAllocateInfo allocInfo{};
	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = m_device.getCommandPool();
	allocInfo.commandBufferCount = static_cast<uint32_t>(m_commandBuffers.size());
	
	if(vkAllocateCommandBuffers(m_device.device(), &allocInfo, m_commandBuffers.data()) != VK_SUCCESS) {
		throw std::runtime_error("failed to allocate command buffers!");
	}
	
	for (int i = 0; i < m_commandBuffers.size(); i ++) {
		VkCommandBufferBeginInfo beginInfo{};
		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		
		if (vkBeginCommandBuffer(m_commandBuffers[i], &beginInfo) != VK_SUCCESS) {
			throw std::runtime_error("failed to begin recording command buffer!");
		}
		
		VkRenderPassBeginInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = m_swapChain.getRenderPass();
		renderPassInfo.framebuffer = m_swapChain.getFrameBuffer(i);
		
		renderPassInfo.renderArea.offset = {0, 0};
		renderPassInfo.renderArea.extent = m_swapChain.getSwapChainExtent();
		
		std::array<VkClearValue, 2> clearValues{};
		clearValues[0].color = {0.1f, 0.1f, 0.1f, 1.0f};
		clearValues[1].depthStencil = {1.0f, 0};
		
		renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
		renderPassInfo.pClearValues = clearValues.data();
		
		vkCmdBeginRenderPass(m_commandBuffers[i], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);
		
		m_pipeline->bind(m_commandBuffers[i]);
		vkCmdDraw(m_commandBuffers[i], 3, 1, 0, 0);
		
		vkCmdEndRenderPass(m_commandBuffers[i]);
		if (vkEndCommandBuffer(m_commandBuffers[i]) != VK_SUCCESS) {
			throw std::runtime_error("failed to record command buffer!");
		}
	}
}

void Application::drawFrame() {
	uint32_t imageIndex;
	auto result = m_swapChain.acquireNextImage(&imageIndex);
	
	if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
		throw std::runtime_error("failed to acquire swap chain image!");
	}
	
	result = m_swapChain.submitCommandBuffers(&m_commandBuffers[imageIndex], &imageIndex);
	
	if (result != VK_SUCCESS) {
		throw std::runtime_error("failed to present swap chain image!");
	}
}

