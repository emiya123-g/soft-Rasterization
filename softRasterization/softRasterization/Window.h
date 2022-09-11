#pragma once
#include"glad\glad.h"
#include"GLFW\glfw3.h"
#include<iostream>
#include"Draw.h"
#include"Camera.h"
#include"shader.h"
#include"Draw.h"
#include"FrameBuffer.h"
#pragma comment(lib,"glfw3.lib")

class Window
{
public:
	Window(int w, int h) :m_width(w), m_height(h),m_pwindow(nullptr),re(w,h),currentTime(0),lastTime(0){
		shader = std::make_shared<Shader>();

		framebuffer = std::make_shared<FrameBuffer>(800, 600);

		shadowBuffer = std::make_shared<FrameBuffer>(800, 600, true);
	}
	int init();
	void run();
	void render(Mesh& m);
	void showFPS();

	

private:
	int m_width;
	int m_height;
	GLFWwindow* m_pwindow;
	RasterEngine re;
	double currentTime;
	double lastTime;
	int fps = 0;

	std::shared_ptr<Shader> shader;

	std::shared_ptr<FrameBuffer> framebuffer;

	std::shared_ptr<FrameBuffer> shadowBuffer;


};

