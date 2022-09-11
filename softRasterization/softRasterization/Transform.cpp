#include"Transform.h"

glm::mat4 GeometryStage::getViewPortMatrix(int ox, int oy, int width, int height)
{
	glm::mat4 ret = glm::mat4(1.0f);
	ret[0][0] = width / 2.0f;
	ret[3][0] = ox + (width / 2.0f);
	ret[1][1] = height / 2.0f;
	ret[3][1] = oy + (height / 2.0f);
	return ret;
}

glm::mat4 GeometryStage::getViewMatrix(glm::vec3 pos, glm::vec3 front, glm::vec3 right, glm::vec3 up)
{
	glm::mat4 ret = glm::mat4(1.0f);
	ret[0][0] = right.x;
	ret[1][0] = right.y;
	ret[2][0] = right.z;
	ret[3][0] = -glm::dot(right, pos);
	ret[0][1] = up.x;
	ret[1][1] = up.y;
	ret[2][1] = up.z;
	ret[3][1] = -glm::dot(up, pos);
	ret[0][2] = -front.x;
	ret[1][2] = -front.y;
	ret[2][2] = -front.z;
	ret[3][2] = glm::dot(front, pos);
	return ret;
}

glm::mat4 GeometryStage::getPerspectiveMatrix(const float& fovy, const float& aspect, const float&near, const float& far)
{
	glm::mat4 ret = glm::mat4(0.0f);
	 float tanHalfFov = tan(fovy * 0.5f);  
	ret[0][0] = 1.0f / (aspect * tanHalfFov);
	ret[1][1] = 1.0f / (tanHalfFov);
	ret[2][2] = -(far + near) / (far - near);
	ret[2][3] = -1.0f;
	ret[3][2] = (-2.0f * near * far) / (far - near);
	return ret;
}

void GeometryStage::toNDC(glm::vec4& posH)
{
	posH /= posH.w;
	posH.z = (posH.z + 1.0) * 0.5;
}

bool GeometryStage::FaceCulling(const glm::vec4& v1, const glm::vec4& v2, const glm::vec4& v3)
{
	glm::vec3 tmp1 = glm::vec3(v2.x - v1.x, v2.y - v1.y, v2.z - v1.z);
	glm::vec3 tmp2 = glm::vec3(v3.x - v1.x, v3.y - v1.y, v3.z - v1.z);

	glm::vec3 normal = glm::normalize(glm::cross(tmp1, tmp2));
	
	glm::vec3 view = glm::vec3(0, 0, 1);
	return glm::dot(normal, view) < 0;
}

bool GeometryStage::notInNDC(glm::vec4& posH)
{
	return posH.x > 1 || posH.x < -1 || posH.y > 1 || posH.y < -1;
}

