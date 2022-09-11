#include "global.h"

Camera* cam = new Camera();

DirectionLight* dirLight = new DirectionLight();

bool drawHalfSpace = true;

std::shared_ptr<Shader> shader = std::make_shared<Shader>();

std::shared_ptr<FrameBuffer> framebuffer = std::make_shared<FrameBuffer>(800, 600);

std::shared_ptr<FrameBuffer> shadowBuffer = std::make_shared<FrameBuffer>(800, 600, true);