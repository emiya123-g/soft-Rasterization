#include "global.h"

Camera* cam = new Camera();

DirectionLight* dirLight = new DirectionLight();

bool drawHalfSpace = true;

std::shared_ptr<Shader> shader = std::make_shared<Shader>();

std::shared_ptr<ShadowMapShader> shadowMapShader = std::make_shared<ShadowMapShader>();

std::shared_ptr<FrameBuffer> framebuffer = std::make_shared<FrameBuffer>(800, 600);

std::shared_ptr<FrameBuffer> shadowBuffer = std::make_shared<FrameBuffer>(800, 600, true);

glm::mat4 viewPort = glm::mat4(
	400,0,0,0,
	0,300,0,0,
	0,0,1,0,
	400,300,0,1
);

int screenWidth = 800;
int screenHeight = 600;

const int shadowMapWidth = 1024;

const int shadowMapHeight = 1024;