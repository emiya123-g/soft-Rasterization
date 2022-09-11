#pragma once
#include"glm/glm.hpp"
#include"glm/gtc/matrix_transform.hpp"
class Vertex {
public:
	Vertex() = default;


public:
	glm::vec4 position;
	glm::vec4 color;
	glm::vec3 normal;
	glm::vec2 texcoord;
};