#include "global.h"

int screenWidth = 600;
int screenHeight = 400;

const int shadowMapWidth = 1024;

const int shadowMapHeight = 1024;

Camera* cam = new Camera();

DirectionLight* dirLight = new DirectionLight();

bool drawHalfSpace = true;

std::shared_ptr<Shader> shader = std::make_shared<Shader>();

std::shared_ptr<ShadowMapShader> shadowMapShader = std::make_shared<ShadowMapShader>();

std::shared_ptr<FrameBuffer> framebuffer = std::make_shared<FrameBuffer>(screenWidth, screenHeight);

std::shared_ptr<FrameBuffer> shadowBuffer = std::make_shared<FrameBuffer>(screenWidth, screenHeight, true);

glm::mat4 viewPort = glm::mat4(
	screenWidth/2,0,0,0,
	0, screenHeight/2,0,0,
	0,0,1,0,
	screenWidth/2, screenHeight/2,0,1
);

std::shared_ptr<Material> boxMat = std::make_shared<Material>(glm::vec3(1.0, 0.0, 0.0));
std::shared_ptr<Material> planeMat = std::make_shared<Material>(glm::vec3(0.0, 1.0, 0.0));