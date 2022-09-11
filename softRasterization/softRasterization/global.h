#pragma once
#include"Camera.h"
#include"Light.h"
#include<memory>
#include"Shader.h"
#include"FrameBuffer.h"

extern Camera* cam;

extern DirectionLight* dirLight;

extern bool drawHalfSpace;

extern std::shared_ptr<Shader> shader;

extern std::shared_ptr<FrameBuffer> framebuffer;

extern std::shared_ptr<FrameBuffer> shadowBuffer;
