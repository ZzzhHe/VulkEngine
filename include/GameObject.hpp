#pragma once

#include "Model.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <memory>

class GameObject {
	
	struct TransformComponent {
		glm::vec3 translation{};
		glm::vec3 scale{1.f, 1.f, 1.f};
		glm::vec3 rotation;
		
		glm::mat4 mat4() {
			auto transform = glm::translate(glm::mat4{1.f}, translation);
			// Rotation convention uses tait-bryan angles with axis order X(1), Y(2), Z(3)
			transform = glm::rotate(transform, rotation.x, {1.f, 0.f, 0.f});
			transform = glm::rotate(transform, rotation.y, {0.f, 1.f, 0.f});
			transform = glm::rotate(transform, rotation.z, {0.f, 0.f, 1.f});
			transform = glm::scale(transform, scale);
			return transform;
		}
	};
	
public:
	using obj_id_t = unsigned int;
	
	static GameObject createGameObject() {
		static obj_id_t currentId = 0;
		return GameObject{currentId ++};
	}
	
	GameObject (const GameObject &) = delete;
	GameObject &operator=(const GameObject &) = delete;
	GameObject (GameObject &&) = default;
	GameObject &operator=(GameObject &&) = default;
	
	obj_id_t getId() {return id;}
	
	std::shared_ptr<Model> model{};
	glm::vec3 color;
	
	TransformComponent transform{};
	
private:
	GameObject(obj_id_t id) : id(id) {}
	obj_id_t id;
};
