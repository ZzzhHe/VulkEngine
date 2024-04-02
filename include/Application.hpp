#pragma once

#include "Window.hpp"
#include "Pipeline.hpp"

class Application {
public:
	static constexpr int WIDTH = 800;
	static constexpr int HEIGHT = 600;
	
	void run();
private:
	Window m_appWindow {WIDTH, HEIGHT, "VulkEngine!"};
	Pipeline m_pipeline {"shaders/simple_shader.vert.spv", "shaders/simple_shader.frag.spv"};
};
