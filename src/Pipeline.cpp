#include "Pipeline.hpp"

#include <iostream>
#include <fstream>
#include <stdexcept>

Pipeline::Pipeline(const std::string& vertFilepath, const std::string& fragFilepath) {
	createGraphicsPipeline(vertFilepath, fragFilepath);
}

std::vector<char> Pipeline::readFile(const std::string& path) {
	// ios::ate -> seek the end immediately to make get size easier, ios::binary -> file format
	std::ifstream file{path, std::ios::ate | std::ios::binary};
	
	if (!file.is_open()) {
		throw std::runtime_error("failed to open file: " + path);
	}
	
	size_t fileSize = static_cast<size_t>(file.tellg()); // tellg() is the end of the file, because ios::ate
	std::vector<char> buffer(fileSize);
	
	file.seekg(0);
	file.read(buffer.data(), fileSize);
	
	file.close();
	
	return buffer;
}

void Pipeline::createGraphicsPipeline(
	const std::string& vertFilepath, const std::string& fragFilepath) {
	auto vertCode = readFile(vertFilepath);
	auto fragCode = readFile(fragFilepath);
	
	std::cout << "Vertex Shader Code Size: " << vertCode.size() << "\n";
	std::cout << "Vertex Shader Code Size: " << fragCode.size() << "\n";
}
