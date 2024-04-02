#pragma once

#include "Window.hpp"
#include "Pipeline.hpp"
#include "Device.hpp"

class Application {
public:
	static constexpr int WIDTH = 800;
	static constexpr int HEIGHT = 600;
	
	void run();
private:
	Window m_appWindow {WIDTH, HEIGHT, "VulkEngine!"};
	Device m_appDevice {m_appWindow};
	Pipeline m_appPipeline {
		m_appDevice,
		"shaders/simple_shader.vert.spv",
		"shaders/simple_shader.frag.spv",
		Pipeline::defaultPipelineConfigInfo(WIDTH, HEIGHT)
	};
};
