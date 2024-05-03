#pragma once

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include "glm/glm.hpp"

class Camera {
public:
	void setOrthoProjection(float left, float right, float top, float bottom, float near, float far);
	void setPerspectProjection(float fovy, float aspect, float near, float far);
	
	const glm::mat4& getProjection() const;

private:
	glm::mat4 m_projectionMatrix{1.f};
};
