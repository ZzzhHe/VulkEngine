#pragma once

#include "Camera.hpp"

#include <vulkan/vulkan.h>


struct FrameInfo {
	int frameIndex;
	float frameTime;
	VkCommandBuffer commandBuffer;
	Camera &camera;
};
