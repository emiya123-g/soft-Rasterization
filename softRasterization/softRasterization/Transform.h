#pragma once
#include"glm\glm.hpp"


namespace GeometryStage{
	glm::mat4 getViewPortMatrix(int ox, int oy, int width, int height);
	glm::mat4 getViewMatrix(glm::vec3 pos, glm::vec3 front, glm::vec3 right, glm::vec3 up);
	glm::mat4 getPerspectiveMatrix(const float& fovy, const float& aspect, const float& near, const float& far); //[-1,1]->[near->far]
	void toNDC(glm::vec4& posH); //z->[0,1]
	bool FaceCulling(const glm::vec4& v1, const glm::vec4& v2, const glm::vec4& v3); //in ndc ,the view dir is (0,0,1)
	bool notInNDC(glm::vec4& posH);
};