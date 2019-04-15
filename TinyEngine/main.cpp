#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stb_image.h>

#include <iostream>
#include <functional>
#include <vector>
#include <map>
using namespace std;

#include "Camera.h"
#include "Cubemap.h"
#include "FrameBuffer.h"
#include "Geometry.h"
#include "Light.h"
#include "LightManager.h"
#include "LoadManager.h"
#include "Mesh.h"
#include "MeshRenderer.h"
#include "RenderManager.h"
#include "Shader.h"
#include "SystemSettings.h"
#include "Texture.h"
#include "Object.h"
#include "ObjectBuilder.h"
#include "ObjectManager.h"
#include "Vertex.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(Camera* camera, GLFWwindow* pWindow, float deltaTime)
{
	if (glfwGetKey(pWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(pWindow, true);
	}

	if (glfwGetKey(pWindow, GLFW_KEY_W) == GLFW_PRESS)
	{
		camera->ProcessKeyboard(FORWARD, deltaTime);
	}

	if (glfwGetKey(pWindow, GLFW_KEY_S) == GLFW_PRESS)
	{
		camera->ProcessKeyboard(BACKWARD, deltaTime);
	}

	if (glfwGetKey(pWindow, GLFW_KEY_A) == GLFW_PRESS)
	{
		camera->ProcessKeyboard(LEFT, deltaTime);
	}

	if (glfwGetKey(pWindow, GLFW_KEY_D) == GLFW_PRESS)
	{
		camera->ProcessKeyboard(RIGHT, deltaTime);
	}

	if (glfwGetKey(pWindow, GLFW_KEY_F1) == GLFW_PRESS)
	{
		RenderManager::getInstance().SetMSAA(true);
	}

	if (glfwGetKey(pWindow, GLFW_KEY_F2) == GLFW_PRESS)
	{
		RenderManager::getInstance().SetMSAA(false);
	}
}

void processScroll(GLFWwindow* pWindow, double xoffset, double yoffset)
{
	Camera::GetWorldCamera()->ProcessScroll((float)yoffset);
}

float lastMouseX;
float lastMouseY;
float firstMouseInput = true;
void processMouseMove(GLFWwindow* pWindow, double xPos, double yPos)
{
	if (firstMouseInput)
	{
		firstMouseInput = false;
	}
	else
	{
		Camera::GetWorldCamera()->ProcessMouse(xPos - lastMouseX, yPos - lastMouseY);
	}

	lastMouseX = (float)xPos;
	lastMouseY = (float)yPos;
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* pWindow = glfwCreateWindow(SystemSettings::WINDOW_WIDTH, SystemSettings::WINDOW_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (pWindow == NULL)
	{
		std::cout << "Fail to create GLFW window." << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(pWindow);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Fail to initialize GLAD." << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);
	glfwSetFramebufferSizeCallback(pWindow, framebuffer_size_callback);
	glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(pWindow, processMouseMove);
	glfwSetScrollCallback(pWindow, processScroll);

	Shader shader("normal.vs", "normal.fs");
	Material material(&shader);
	Geometry* mesh = LoadManager::getInstance().LoadGeometryData("cube.mesh");
	ObjectBuilder::CreateObject(mesh, &material, 1000, glm::vec3(5.0f), glm::vec3(0.0f), glm::vec3(1.0f));

	RenderManager::getInstance().Prepare();

	float lastTime = glfwGetTime();
	while (!glfwWindowShouldClose(pWindow))
	{
		Camera* worldCamera = Camera::GetWorldCamera();

		float curTime = glfwGetTime();
		processInput(worldCamera, pWindow, curTime - lastTime);
		lastTime = curTime;

		RenderManager::getInstance().RenderWorld(worldCamera);

		glfwSwapBuffers(pWindow);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}