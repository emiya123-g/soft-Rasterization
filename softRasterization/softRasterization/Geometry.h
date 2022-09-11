#pragma once
#include"Triangle.h"

class Geometry {
public:
	static Mesh CreatePlane(const glm::vec3& leftTop, const glm::vec3& leftBottom, const glm::vec3& rightBottom, const glm::vec3& rightTop, const glm::vec3& normal) {
		Mesh ret(4, 6);
		ret.vertexBuffer[0].position = glm::vec4(leftTop, 1.0f);
		ret.vertexBuffer[0].normal = normal;
		ret.vertexBuffer[0].color = glm::vec4(255, 0, 0, 255);
		ret.vertexBuffer[0].texcoord = glm::vec2(0.0f, 1.0f);
		ret.vertexBuffer[1].position = glm::vec4(rightTop, 1.0f);
		ret.vertexBuffer[1].normal = normal;
		ret.vertexBuffer[1].color = glm::vec4(0, 255, 0, 255);
		ret.vertexBuffer[1].texcoord = glm::vec2(1.0f, 1.0f);
		ret.vertexBuffer[2].position = glm::vec4(rightBottom, 1.0f);
		ret.vertexBuffer[2].normal = normal;
		ret.vertexBuffer[2].color = glm::vec4(0, 0, 255, 0);
		ret.vertexBuffer[2].texcoord = glm::vec2(1.0f, 0.0f);
		ret.vertexBuffer[3].position = glm::vec4(leftBottom, 1.0f);
		ret.vertexBuffer[3].normal = normal;
		ret.vertexBuffer[3].color = glm::vec4(255, 0, 255, 255);
		ret.vertexBuffer[3].texcoord = glm::vec2(0.0f, 0.0f);
		
		ret.indexBuffer[0] = 0;
		ret.indexBuffer[1] = 2;
		ret.indexBuffer[2] = 1;
		ret.indexBuffer[3] = 0;
		ret.indexBuffer[4] = 3;
		ret.indexBuffer[5] = 2;
		return ret;
	}
	static Mesh CreateBox(const glm::vec3& center, float radius) {
		Mesh result;
		Mesh front = CreatePlane(
			center + glm::vec3(-radius, radius, radius),
			center + glm::vec3(-radius, -radius, radius),
			center + glm::vec3(radius, -radius, radius),
			center + glm::vec3(radius, radius, radius),
			glm::vec3(0, 0, 1)
		);
		result.addMesh(front);

		Mesh left = CreatePlane(
			center + glm::vec3(-radius, radius, -radius),
			center + glm::vec3(-radius, -radius, -radius),
			center + glm::vec3(-radius, -radius, radius),
			center + glm::vec3(-radius, radius, radius),
			glm::vec3(-1, 0, 0)
		);
		result.addMesh(left);

		Mesh right = CreatePlane(
			center + glm::vec3(radius, radius, radius),
			center + glm::vec3(radius, -radius, radius),
			center + glm::vec3(radius, -radius, -radius),
			center + glm::vec3(radius, radius, -radius),
			glm::vec3(1, 0, 0)
		);
		result.addMesh(right);

		Mesh back = CreatePlane(
			center + glm::vec3(radius, radius, -radius),
			center + glm::vec3(radius, -radius, -radius),
			center + glm::vec3(-radius, -radius, -radius),
			center + glm::vec3(-radius, radius, -radius),
			glm::vec3(0, 0, -1)
		);
		result.addMesh(back);

		Mesh up = CreatePlane(
			center + glm::vec3(-radius, radius, -radius),
			center + glm::vec3(-radius, radius, radius),
			center + glm::vec3(radius, radius, radius),
			center + glm::vec3(radius, radius, -radius),
			glm::vec3(0, 1, 0)
		);
		result.addMesh(up);

		Mesh down = CreatePlane(
			center + glm::vec3(-radius, -radius, radius),
			center + glm::vec3(-radius, -radius, -radius),
			center + glm::vec3(radius, -radius, -radius),
			center + glm::vec3(radius, -radius, radius),
			glm::vec3(0, -1, 0)
		);
		result.addMesh(down);

		return result;
	}
};