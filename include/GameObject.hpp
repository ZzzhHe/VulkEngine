#pragma once

#include "Model.hpp"

#include <glm/gtc/matrix_transform.hpp>

#include <memory>
#include <unordered_map>

class GameObject {
	
	struct TransformComponent {
		glm::vec3 translation{};
		glm::vec3 scale{1.f, 1.f, 1.f};
		glm::vec3 rotation;
		
		glm::mat4 mat4();
		glm::mat4 normalTransformMatrix();
	};
	
public:
	using obj_id_t = unsigned int;
	using Map = std::unordered_map<id_t, GameObject>;
	
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
