#pragma once

#include "Model.hpp"

#include <memory>


class GameObject {
	
	struct Transform2dComponent {
		glm::vec2 translation{};
		glm::vec2 scale{1.f, 1.f};
		float rotation;
		
		glm::mat2 mat2() {
			const float s = glm::sin(rotation);
			const float c = glm::cos(rotation);
			glm::mat2 rotMatrix{{c, s}, {-s, c}};
			
			glm::mat2 scaleMat = {{scale.x, 0.0f}, {0.0f, scale.y}};
			return rotMatrix * scaleMat;
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
	
	Transform2dComponent transform2d{};
	
private:
	GameObject(obj_id_t id) : id(id) {}
	obj_id_t id;
};
