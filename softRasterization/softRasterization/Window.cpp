#include"window.h"
#include<sstream>
#include<windows.h>
#include<iostream>
#include"Geometry.h"
#include"Transform.h"

extern Camera* cam;

extern bool drawHalfSpace;

extern DirectionLight* dirLight;

extern std::shared_ptr<Shader> shader;

extern std::shared_ptr<ShadowMapShader> shadowMapShader;

extern std::shared_ptr<FrameBuffer> framebuffer;

extern std::shared_ptr<FrameBuffer> shadowBuffer;

extern int screenWidth;
extern int screenHeight;

extern const int shadowMapWidth;

extern const int shadowMapHeight;

extern DirectionLight* dirLight;

extern std::shared_ptr<Material> boxMat;
extern std::shared_ptr<Material> planeMat;

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
        cam->RotatePitch(1.0);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
        cam->RotateYaw(-1.0);
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
        cam->RotateYaw(+1.0);
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
        cam->RotatePitch(-1.0);
    }
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
        cam->MoveForward(0.1);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
        cam->MoveForward(-0.1);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
        cam->MoveRight(-0.1);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
        cam->MoveRight(0.1);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) {
        cam->MoveUp(0.1);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS) {
        cam->MoveUp(-0.1);
    }
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS) {
        drawHalfSpace = !drawHalfSpace;
    }
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS) {
        dirLight->Pos.z -= 0.1f;
        dirLight->Pos.z = max(dirLight->Pos.z, -8.0f);
        dirLight->update();
    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS) {
        dirLight->Pos.z += 0.1f;
        dirLight->Pos.z = min(dirLight->Pos.z, 8.0f);
        dirLight->update();
    }
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS) {
        dirLight->Pos.x -= 0.1f;
        dirLight->Pos.x = max(dirLight->Pos.x, -8.0f);
        dirLight->update();
    }
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS) {
        dirLight->Pos.x += 0.1f;
        dirLight->Pos.x = min(dirLight->Pos.x, 8.0f);
        dirLight->update();
    }
    if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) {
        boxMat->addRoughness(-0.01f);
    }
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) {
        boxMat->addRoughness(0.01f);
    }
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {
        boxMat->addMetalness(-0.01f);
    }
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS) {
        boxMat->addMetalness(0.01f);
    }
}

int Window::init()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    m_pwindow = glfwCreateWindow(m_width, m_height, "Rasterization", NULL, NULL);
    if (m_pwindow == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(m_pwindow);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glViewport(0, 0, m_width, m_height);

    glfwSetFramebufferSizeCallback(m_pwindow, framebuffer_size_callback);
}

void Window::run()
{
    Mesh box = Geometry::CreateBox(glm::vec3(0, 0.5, 0), 0.5);
    Mesh plane = Geometry::CreatePlane(glm::vec3(-4, 0, -4), glm::vec3(-4, 0, 4), glm::vec3(4, 0, 4), glm::vec3(4, 0, -4), glm::vec3(0, 1, 0));
    Mesh scence;
    scence.addMesh(box);
    scence.addMesh(plane);

    while (!glfwWindowShouldClose(m_pwindow))
    {
        glfwPollEvents();
        processInput(m_pwindow);
        showFPS();
        glfwGetWindowSize(m_pwindow, &m_width, &m_height);
        cam->UpdateAspect(m_width, m_height);
        screenWidth = m_width;
        screenHeight =  m_height;

        //shadow pass
        re.shader = shader;
        re.buffer = shadowBuffer;
        re.shader->setView(dirLight->lightView);
        re.shader->setProj(dirLight->lightProj);
        re.update(shadowMapWidth, shadowMapHeight);
        re.buffer->resize(shadowMapWidth, shadowMapHeight);
        render(box);
        
        //color pass
        re.buffer = framebuffer;                           //bind render target
        re.shader->setProj(cam->PerspectiveMatrix());       
        re.shader->setView(cam->ViewMatrix());
        re.update(m_width, m_height);                      //update viewport  
        re.buffer->resize(m_width,m_height);               //clear frame buffer binded
        re.shader->setMatrial(boxMat);
        render(box);                                       //render  

        re.shader = shadowMapShader;
        re.shader->setProj(cam->PerspectiveMatrix());
        re.shader->setView(cam->ViewMatrix());
        re.shader->setMatrial(planeMat);
        render(plane);

        glDrawPixels(m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, re.buffer->m_colorBuffer.data());
        glFlush();
        glfwSwapBuffers(m_pwindow);
        
    }
    glfwTerminate();
}

void Window::render(Mesh &m)
{
    if (drawHalfSpace)
        re.drawMeshHalfSpace(m);
    else
        re.drawMeshScanLine(m);
}

void Window::showFPS()
{
    ++fps;
    // Measure speed
    double currentTime = glfwGetTime();
    double delta = currentTime - lastTime;
    
    if (delta >= 1.0) {
        std::stringstream ss;
        if (drawHalfSpace) {
            ss << "half space";
        }
        else {
            ss << "scan line";
        }
        ss << "-" << "'T' to change mode";
        ss << " [" << fps << " FPS] " << m_width << "x" << m_height<<" "<<"roughness:"<<boxMat->roughness
            <<" metalness:"<<boxMat->metalness;
        
        glfwSetWindowTitle(m_pwindow, ss.str().c_str());
        lastTime = currentTime;
        fps = 0;
    }
}

