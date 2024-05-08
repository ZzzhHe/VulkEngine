#pragma once
#include "Device.hpp"
#include "Buffer.hpp"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "glm/glm.hpp"

#include <vector>
#include <memory>

class Model {
public:
	
	struct Vertex {
		glm::vec3 position;
		glm::vec3 color;
		glm::vec3 normal{};
		glm::vec2 uv{}; // texture coordinates
		
		static std::vector<VkVertexInputBindingDescription> getBindingDescription();
		static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();
		
		bool operator == (const Vertex &other) const {
			return position == other.position && color == other.color
				&& normal == other.normal && uv == other.uv;
		}
	};
	
	struct Builder {
		std::vector<Vertex> vertices{};
		std::vector<uint32_t> indices{};
		
		void loadModel(const std::string & filepath);
	};
	
	Model(Device &device, const Builder &builder);
	~Model();
	
	Model(const Model&) = delete;
	Model &operator=(const Model&) = delete;
	
	static std::unique_ptr<Model> createModelFromFile(Device &device, const std::string &filepath);
	
	void bind(VkCommandBuffer commandBuffer);
	void draw(VkCommandBuffer commandBuffer);

private:
	
	void createVertexBuffers(const std::vector<Vertex> &vertices);
	void createIndexBuffers(const std::vector<uint32_t> &indices);
	
	Device& m_device_ref;
	
	std::unique_ptr<Buffer> m_vertexBuffer;
	uint32_t m_vertexCount;
	
	bool m_hasIndexBuffer = false;
	std::unique_ptr<Buffer> m_indexBuffer;
	uint32_t m_indexCount;
};
