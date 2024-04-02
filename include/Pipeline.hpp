#pragma once

#include <string>
#include <vector>

class Pipeline {
public:
	Pipeline(const std::string& vertFilepath, const std::string& fragFilepath);
private:
	static std::vector<char> readFile(const std::string& path);
	void createGraphicsPipeline(const std::string& vertFilepath, const std::string& fragFilepath);
};
