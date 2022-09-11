#pragma once
#include"glm/glm.hpp"
#include<algorithm>


class Light {
public:
	Light(const glm::vec3 &pos=glm::vec3(0,0,0),const glm::vec3 &color = glm::vec3(1,1,1),const glm::vec3& dir = glm::vec3(0,1,0),const float& intensity=1.0f)
		:Pos(pos),Color(color),Dir(glm::normalize(dir)),Intensity(intensity){
		
	}
	glm::vec3 reflect(const glm::vec3& lightDir, const glm::vec3& normal) {
		return 2 * glm::dot(normal, lightDir) * normal - lightDir;
	}

public:
	glm::vec3 Pos;
	glm::vec3 Color;
	glm::vec3 Dir;    //point to the light source
	float Intensity;
};

class DirectionLight :public Light {
public:
	DirectionLight(const glm::vec3& pos = glm::vec3(3, 5, 10), const glm::vec3& color = glm::vec3(255), const glm::vec3& dir = glm::vec3(3, 5, 10), const float& intensity = 1.0f) :
		Light(pos, color, dir, intensity) {
		lightView = glm::lookAt(pos, glm::vec3(0.0f), glm::vec3(0, 1, 0));
		lightProj = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f,0.3f,50.0f);
	}
	glm::vec3 BlinPhong(const glm::vec3 &normal,const glm::vec3 &viewDir,const glm::vec3 &albedo) {
		float diff = std::max(glm::dot(glm::normalize(normal), Dir),0.0f);
		glm::vec3 reflectDir = glm::normalize(reflect(Dir, normal));
		glm::vec3 halfVec = glm::normalize(Dir + reflectDir);
		float spec = pow(std::max(glm::dot(normal, halfVec), 0.0f), 5);
		return (spec * Color + diff * albedo) * Intensity;
	}
public:
	glm::mat4 lightView;
	glm::mat4 lightProj;
};