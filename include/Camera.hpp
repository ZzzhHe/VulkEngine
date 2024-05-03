#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "glm/glm.hpp"

class Camera {
public:
	void setOrthoProjection(float left, float right, float top, float bottom, float near, float far);
	void setPerspectProjection(float fovy, float aspect, float near, float far);
	
	void setViewDirection(glm::vec3 position, glm::vec3 direction, glm::vec3 up = glm::vec3{0.f, -1.f, 0.f});
	
	void setViewTarget(glm::vec3 position, glm::vec3 target, glm::vec3 up = glm::vec3{0.f, -1.f, 0.f});
	
	void setViewXYZ(glm::vec3 position, glm::vec3 rotation);
	
	const glm::mat4& getProjection() const;
	const glm::mat4& getView() const;

private:
	glm::mat4 m_projectionMatrix{1.f};
	glm::mat4 m_viewMatrix{1.f};
};
