#include"Shader.h"
#include<memory>
#include<iostream>
#include"FrameBuffer.h"

extern Camera* cam;
extern DirectionLight* dirLight;
extern std::shared_ptr<FrameBuffer> shadowBuffer;

extern int screenWidth;
extern int screenHeight;

extern const int shadowMapWidth;
extern const int shadowMapHeight;

glm::vec4 Shader::FS(const V2f& v)
{
	glm::vec3 color = glm::vec3(0.0);
	glm::vec3 worldPos = glm::vec3(v.posW.x, v.posW.y, v.posW.z);
	glm::vec3 viewDir = glm::normalize(cam->Position - worldPos);

	glm::vec3 albedo = glm::vec3(1.0);

	if (material != nullptr) {
		if (material->texture.data != nullptr) {
			albedo = material->texture.Sample2D(v.texcoord);
		}
		else
			albedo = material->albedo;
	}
	if (material != nullptr)
		color += material->cookTorrance(v.normal, viewDir, dirLight->Dir, albedo);
	else
		color += dirLight->BlinPhong(v.normal, viewDir, albedo);

	color += 0.2f * albedo;

	color /= (color + glm::vec3(1.0));
	return glm::vec4(color * 255.0f, 255);   //if the value not in 0-255 this will overflow and make some mistake
}

glm::vec4 ShadowMapShader::FS(const V2f& v)
{
	glm::vec3 color = glm::vec3(0.0);
	glm::vec3 worldPos = glm::vec3(v.posW.x, v.posW.y, v.posW.z);
	glm::vec3 viewDir = glm::normalize(cam->Position - worldPos);

	glm::vec3 albedo = glm::vec3(1.0);

	if (material != nullptr) {
		if (material->texture.data != nullptr) {
			albedo = material->texture.Sample2D(v.texcoord);
		}
		else
			albedo = material->albedo;
	}

	//if (material != nullptr)
	//	color += material->cookTorrance(v.normal, viewDir, dirLight->Dir, albedo);
	//else
	color += dirLight->BlinPhong(v.normal, viewDir, albedo);


	glm::vec4 lightPos = (dirLight->lightProj) * (dirLight->lightView) * v.posW;
	lightPos /= lightPos.w;
	lightPos.z = (lightPos.z + 1.0f) * 0.5;
	lightPos.x = (lightPos.x * shadowMapWidth / 2) + shadowMapWidth / 2;
	lightPos.y = (lightPos.y * shadowMapHeight / 2) + shadowMapHeight / 2;

	int x = lightPos.x;
	int y = lightPos.y;
	//shadow map
	float shadow = lightPos.z-0.002f > shadowBuffer->getDepth(x, y) ? 0.0f : 1.0f;
	color *= shadow;

	color += 0.2f * albedo;
	//pcf
	//float shadowCount = 0;
	//float total = 0.0;
	//for (int i = -1; i <= 1; i++) {
	//	for (int j = -1; j <= 1; j++) {
	//		int newX = x + i;
	//		int newY = y + j;
	//		if (newX >= 0 && newX < shadowMapWidth && newY >= 0 && newY < shadowMapHeight) {
	//			++total;
	//			if (lightPos.z-0.001f > shadowBuffer->getDepth(newX, newY))
	//				++shadowCount;
	//		}
	//	}
	//}
	//float shadow = 1.0f- shadowCount / total;
	//color *= shadow;
	color /= (color + glm::vec3(1.0));
	return glm::vec4(color*255.0f, 255);   //if the value not in 0-255 this will overflow and make some mistake
}
