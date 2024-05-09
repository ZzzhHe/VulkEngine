#pragma once

#include "Window.hpp"
#include "Device.hpp"
#include "GameObject.hpp"
#include "Renderer.hpp"
#include "Descriptors.hpp"

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
	void loadGameObject();
	
	Window m_window {WIDTH, HEIGHT, "VulkEngine!"};
	Device m_device {m_window};
	Renderer m_renderer{m_window, m_device};
	
	
	// the order of declaration matters
	std::unique_ptr<DescriptorPool> m_globalPool{};
	GameObject::Map m_gameObjects;
};
