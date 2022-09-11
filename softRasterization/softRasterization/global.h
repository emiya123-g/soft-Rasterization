#pragma once
#include"Camera.h"
#include"Light.h"
#include<memory>
#include"Shader.h"
#include"FrameBuffer.h"
#include"glm/glm.hpp"

extern Camera* cam;

extern DirectionLight* dirLight;

extern bool drawHalfSpace;

extern std::shared_ptr<Shader> shader;

extern std::shared_ptr<ShadowMapShader> shadowMapShader;

extern std::shared_ptr<FrameBuffer> framebuffer;

extern std::shared_ptr<FrameBuffer> shadowBuffer;

extern glm::mat4 viewPort;

extern int screenWidth;

extern int screenHeight;

extern const int shadowMapWidth;

extern const int shadowMapHeight;
