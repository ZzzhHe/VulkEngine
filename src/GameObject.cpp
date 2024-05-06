#include <GameObject.hpp>

glm::mat4 GameObject::TransformComponent::mat4() {
	auto transform = glm::translate(glm::mat4{1.f}, translation);
	// Rotation convention uses tait-bryan angles with XYZ
	transform = glm::rotate(transform, rotation.x, {1.f, 0.f, 0.f});
	transform = glm::rotate(transform, rotation.y, {0.f, 1.f, 0.f});
	transform = glm::rotate(transform, rotation.z, {0.f, 0.f, 1.f});
	transform = glm::scale(transform, scale);
	return transform;
}


glm::mat4 GameObject::TransformComponent::normalTransformMatrix() {
	auto normalTransformMatrix = glm::mat4{1.f};
	normalTransformMatrix = glm::rotate(normalTransformMatrix, rotation.x, {1.f, 0.f, 0.f});
	normalTransformMatrix = glm::rotate(normalTransformMatrix, rotation.y, {0.f, 1.f, 0.f});
	normalTransformMatrix = glm::rotate(normalTransformMatrix, rotation.z, {0.f, 0.f, 1.f});
	normalTransformMatrix = glm::scale(normalTransformMatrix, 1.0f / scale);
	return normalTransformMatrix;
 }
