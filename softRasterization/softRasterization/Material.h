#pragma once
#include"glm/glm.hpp"
class Material
{
public:
	Material() = default;
	Material(glm::vec3 baseColor) :albedo(baseColor) {}
public:
	glm::vec3 albedo;

};

