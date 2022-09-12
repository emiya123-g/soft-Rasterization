#pragma once
#include "stb_image.h"
#include <string>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Texture2D {
public:
	Texture2D() = default;
	Texture2D(const std::string& Path) {
		data = stbi_load(Path.c_str(), &width, &height, &channels, 0);
		assert(data != nullptr);
	}
	~Texture2D() {
		if (data)
			free(data);
	}
	void LoadTexture(const std::string& Path) {
		if (data)
			free(data);
		data = stbi_load(Path.c_str(), &width, &height, &channels, 0);
	}
	glm::vec4 Sample2D(const glm::vec2& texcoord) {
		float x = texcoord.x - (float)floor(texcoord.x);
		float y = texcoord.y - (float)floor(texcoord.y);
		return GetColor(x * (width - 1), y * (height - 1));
	}
	glm::vec4 GetColor(int x, int y) {
		if (x <0 || x > width || y < 0 || y> height)
			return glm::vec4(0, 0, 0, 0);
		int xy = channels * (y * width + x);
		return glm::vec4(*(data + xy)/255.0f, *(data + xy + 1)/255.0f, *(data + xy + 2)/255.0f, *(data + xy + 3)/255.0f);
	}
public:
	int width;
	int height;
	int channels;
	unsigned char* data;
};