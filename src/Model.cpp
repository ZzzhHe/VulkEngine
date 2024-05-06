#include "Model.hpp"
#include "utils.hpp"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/hash.hpp>

#include <cassert>
#include <cstring>
#include <iostream>
#include <unordered_map>

namespace std {
	template<>
	struct hash<Model::Vertex> {
		size_t operator()(Model::Vertex const &vertex) const {
			size_t seed = 0;
			hashCombine(seed, vertex.position, vertex.color, vertex.normal, vertex.uv);
			return seed;
		}
	};
}

Model::Model(Device &device, const Builder &builder)
	:m_device_ref(device) {
		createVertexBuffers(builder.vertices);
		createIndexBuffers(builder.indices);
}

Model::~Model() {
	vkDestroyBuffer(m_device_ref.device(), m_vertexBuffer, nullptr);
	vkFreeMemory(m_device_ref.device(), m_vertexBufferMemory, nullptr);
	
	if (m_hasIndexBuffer) {
		vkDestroyBuffer(m_device_ref.device(), m_indexBuffer, nullptr);
		vkFreeMemory(m_device_ref.device(), m_indexBufferMemory, nullptr);
	}
}

 std::unique_ptr<Model> Model::createModelFromFile(Device &device, const std::string &filepath) {
	 Builder builder{};
	 builder.loadModel(filepath);
	 std::cout << "Vertex Count: " << builder.vertices.size() << std::endl;
	 return std::make_unique<Model>(device, builder);
}

void Model::createVertexBuffers(const std::vector<Vertex> &vertices) {
	m_vertexCount = static_cast<uint32_t>(vertices.size());
	assert( m_vertexCount >=3 && "Vertex count must be at least 3");
	
	VkDeviceSize bufferSize = sizeof(vertices[0]) * m_vertexCount;
	
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	
	m_device_ref.createBuffer(
			  bufferSize,
			  VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			  VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			  stagingBuffer, stagingBufferMemory); // Host: Cpu, Device: Gpu
	
	void *data;
	vkMapMemory(m_device_ref.device(), stagingBufferMemory, 0, bufferSize, 0, &data); // Host - Device
	memcpy(data, vertices.data(), static_cast<size_t>(bufferSize)); // auto flush() from Host to Device
	vkUnmapMemory(m_device_ref.device(), stagingBufferMemory);
	
	m_device_ref.createBuffer(
			  bufferSize,
			  VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			  VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			  m_vertexBuffer, m_vertexBufferMemory);
	
	m_device_ref.copyBuffer(stagingBuffer, m_vertexBuffer, bufferSize);
	
	vkDestroyBuffer(m_device_ref.device(), stagingBuffer, nullptr);
	vkFreeMemory(m_device_ref.device(), stagingBufferMemory, nullptr);
}

void Model::createIndexBuffers(const std::vector<uint32_t> &indices) {
	m_indexCount = static_cast<uint32_t>(indices.size());
	m_hasIndexBuffer = m_indexCount > 0;
	
	if (!m_hasIndexBuffer) {
		return ;
	}
	
	VkDeviceSize bufferSize = sizeof(indices[0]) * m_indexCount;
	
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	
	m_device_ref.createBuffer(
			  bufferSize,
			  VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
			  VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
			  stagingBuffer, stagingBufferMemory); // Host: Cpu, Device: Gpu
	
	void *data;
	vkMapMemory(m_device_ref.device(), stagingBufferMemory, 0, bufferSize, 0, &data); // Host - Device
	memcpy(data, indices.data(), static_cast<size_t>(bufferSize)); // auto flush() from Host to Device
	vkUnmapMemory(m_device_ref.device(), stagingBufferMemory);
	
	m_device_ref.createBuffer(
			  bufferSize,
			  VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
			  VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
			  m_indexBuffer, m_indexBufferMemory);

	m_device_ref.copyBuffer(stagingBuffer, m_indexBuffer, bufferSize);
	
	vkDestroyBuffer(m_device_ref.device(), stagingBuffer, nullptr);
	vkFreeMemory(m_device_ref.device(), stagingBufferMemory, nullptr);
}

void Model::bind(VkCommandBuffer commandBuffer) {
	VkBuffer buffers[] = {m_vertexBuffer};
	VkDeviceSize offsets[] = {0};
	vkCmdBindVertexBuffers(commandBuffer, 0, 1, buffers, offsets);
	
	if (m_hasIndexBuffer) {
		vkCmdBindIndexBuffer(commandBuffer, m_indexBuffer, 0, VK_INDEX_TYPE_UINT32);
	}
}

void Model::draw(VkCommandBuffer commandBuffer) {
	if (m_hasIndexBuffer) {
		vkCmdDrawIndexed(commandBuffer, m_indexCount, 1, 0, 0, 0);
	} else {
		vkCmdDraw(commandBuffer, m_vertexCount, 1, 0, 0);
	}
}

std::vector<VkVertexInputBindingDescription> Model::Vertex::getBindingDescription() {
	std::vector<VkVertexInputBindingDescription> bindingDescriptions(1);
	bindingDescriptions[0].binding = 0;
	bindingDescriptions[0].stride = sizeof(Vertex);
	bindingDescriptions[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
	return bindingDescriptions;
}

std::vector<VkVertexInputAttributeDescription> Model::Vertex::getAttributeDescriptions() {
	std::vector<VkVertexInputAttributeDescription> attributeDescriptions(4);
	
	attributeDescriptions[0].binding = 0;
	attributeDescriptions[0].location = 0;
	attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[0].offset = offsetof(Vertex, position);
	
	attributeDescriptions[1].binding = 0;
	attributeDescriptions[1].location = 1;
	attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[1].offset = offsetof(Vertex, color);
	
	attributeDescriptions[2].binding = 0;
	attributeDescriptions[2].location = 2;
	attributeDescriptions[2].format = VK_FORMAT_R32G32B32_SFLOAT;
	attributeDescriptions[2].offset = offsetof(Vertex, normal);
	
	attributeDescriptions[3].binding = 0;
	attributeDescriptions[3].location = 3;
	attributeDescriptions[3].format = VK_FORMAT_R32G32_SFLOAT;
	attributeDescriptions[3].offset = offsetof(Vertex, uv);
	
	return attributeDescriptions;
}

void Model::Builder::loadModel(const std::string& filepath) {
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	std::string warn, err;
	
	if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filepath.c_str())) {
		throw std::runtime_error(warn + err);
	}
	
	vertices.clear();
	indices.clear();
	
	std::unordered_map<Vertex, uint32_t> uniqueVertices{};
	
	for (const auto &shape : shapes) {
		for (const auto &index : shape.mesh.indices) {
			Vertex vertex{};
			
			if (index.vertex_index >= 0) {
				vertex.position = {
					attrib.vertices[3 * index.vertex_index + 0],
					attrib.vertices[3 * index.vertex_index + 1],
					attrib.vertices[3 * index.vertex_index + 2],
				};
				
				vertex.color = {
					attrib.colors[3 * index.vertex_index + 0],
					attrib.colors[3 * index.vertex_index + 1],
					attrib.colors[3 * index.vertex_index + 2],
				};
			}
			if (index.normal_index >= 0) {
				vertex.normal = {
					attrib.normals[3 * index.normal_index + 0],
					attrib.normals[3 * index.normal_index + 1],
					attrib.normals[3 * index.normal_index + 2],
				};
			}
			if (index.texcoord_index >= 0) {
				vertex.uv = {
					attrib.texcoords[2 * index.texcoord_index + 0],
					attrib.texcoords[2 * index.texcoord_index + 1],
				};
			}
			
			if (uniqueVertices.count(vertex) == 0) {
				// if vertex is new
				uniqueVertices[vertex] = static_cast<uint32_t>(vertices.size());
				vertices.push_back(vertex);
			}
			indices.push_back(uniqueVertices[vertex]);
		}
	}
	
}
