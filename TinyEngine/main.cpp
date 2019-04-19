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
#include "ShadowMapRenderer.h"
#include "SystemSettings.h"
#include "Texture.h"
#include "Object.h"
#include "ObjectBuilder.h"
#include "ObjectController.h"
#include "ObjectManager.h"
#include "Vertex.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow* pWindow, float deltaTime)
{
	if (glfwGetKey(pWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(pWindow, true);
	}

	if (glfwGetKey(pWindow, GLFW_KEY_W) == GLFW_PRESS)
	{
		ObjectController::getInstance().ProcessKeyboard(FORWARD, deltaTime);
	}

	if (glfwGetKey(pWindow, GLFW_KEY_S) == GLFW_PRESS)
	{
		ObjectController::getInstance().ProcessKeyboard(BACKWARD, deltaTime);
	}

	if (glfwGetKey(pWindow, GLFW_KEY_A) == GLFW_PRESS)
	{
		ObjectController::getInstance().ProcessKeyboard(LEFT, deltaTime);
	}

	if (glfwGetKey(pWindow, GLFW_KEY_D) == GLFW_PRESS)
	{
		ObjectController::getInstance().ProcessKeyboard(RIGHT, deltaTime);
	}

	if (glfwGetKey(pWindow, GLFW_KEY_F1) == GLFW_PRESS)
	{
		RenderManager::getInstance().SetMSAA(true);
	}

	if (glfwGetKey(pWindow, GLFW_KEY_F2) == GLFW_PRESS)
	{
		RenderManager::getInstance().SetMSAA(false);
	}

	if (glfwGetKey(pWindow, GLFW_KEY_F3) == GLFW_PRESS)
	{
		Camera::GetWorldCamera()->isPerspective = !Camera::GetWorldCamera()->isPerspective;
	}

	if (glfwGetKey(pWindow, GLFW_KEY_TAB) == GLFW_PRESS)
	{
		ObjectController::getInstance().SetTarget(ObjectManager::getInstance().Next());
	}

	if (glfwGetKey(pWindow, GLFW_KEY_Q) == GLFW_PRESS)
	{
		ObjectController::getInstance().SetTarget(Camera::GetWorldCamera()->GetOwner());
	}

	if (glfwGetKey(pWindow, GLFW_KEY_E) == GLFW_PRESS)
	{
		ObjectController::getInstance().SetTarget(LightManager::getInstance().getMainLight()->GetOwner());
	}

	//if (glfwGetKey(pWindow, GLFW_KEY_R) == GLFW_PRESS)
	//{
	//	IShadowMapRenderer::getInstance().shadowMapCamera->depth = 100.0f;
	//}

	//if (glfwGetKey(pWindow, GLFW_KEY_T) == GLFW_PRESS)
	//{
	//	IShadowMapRenderer::getInstance().shadowMapCamera->depth = -100.0f;
	//}
}

void processScroll(GLFWwindow* pWindow, double xoffset, double yoffset)
{
	ObjectController::getInstance().ProcessScroll((float)yoffset);
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
		ObjectController::getInstance().ProcessMouse(xPos - lastMouseX, yPos - lastMouseY);
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

	Shader shader("Shaders/phong.vs", "Shaders/phong.fs");
	Texture woodTexture("../Resources/wood.png");
	Material material(&shader);
	material.setTexture("texture_diffuse_0", &woodTexture);
	Geometry* mesh = LoadManager::getInstance().LoadGeometryData("cube.mesh");
	//ObjectBuilder::CreateObject(mesh, &material, 1000, glm::vec3(0.0f, 3.0f, 0.0f), glm::vec3(0.0f), glm::vec3(20.0f, 0.1f, 20.0f));
	ObjectBuilder::CreateObject(mesh, &material, 1000, glm::vec3(0.0f, 1.5f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
	ObjectBuilder::CreateObject(mesh, &material, 1000, glm::vec3(2.0f, 0.0f, 1.0f), glm::vec3(0.0f), glm::vec3(1.0f));
	ObjectBuilder::CreateObject(mesh, &material, 1000, glm::vec3(-5.0f, -0.5f, 0.0f), glm::vec3(0.0f), glm::vec3(1.0f));
	ObjectBuilder::CreateObject(mesh, &material, 1000, glm::vec3(-5.0f, 0.0f, 2.0f), glm::vec3(20.0f, 0.0f, 80.0f), glm::vec3(1.0f));
	ObjectBuilder::CreateObject(mesh, &material, 1000, glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f), glm::vec3(100.0f, 0.1f, 100.0f));

	//vector<Object*> model = LoadManager::getInstance().LoadModelData("../Resources/nanosuit/nanosuit.obj");
	//for (auto obj : model)
	//{
	//	MeshRenderer* renderer = (MeshRenderer*)(obj->GetComponent(MeshRenderer::MESH_RENDERER_NAME));
	//	renderer->GetMaterial()->setShader(&shader);
	//}

	Object* mainLightObj = ObjectBuilder::CreateObject(glm::vec3(0.0f, 5.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	Light* mainLight = mainLightObj->AddComponent<PointLight>();
	LightManager::getInstance().registerMainLight(mainLight);

	//Object* mainLightObj2 = ObjectBuilder::CreateObject(glm::vec3(10.0f, 16.0f, 5.0f), glm::vec3(-45.0f, 120.0f, 0.0f));
	//Light* mainLight2 = mainLightObj2->AddComponent<DirectionalLight>();
	//LightManager::getInstance().registerMainLight(mainLight2);

	Object* cameraObj = ObjectBuilder::CreateObject(glm::vec3(0.0f));
	cameraObj->AddComponent<Camera>();
	ObjectController::getInstance().SetTarget(cameraObj);

	RenderManager::getInstance().Prepare();

	float lastTime = glfwGetTime();
	while (!glfwWindowShouldClose(pWindow))
	{
		float curTime = glfwGetTime();
		processInput(pWindow, curTime - lastTime);
		lastTime = curTime;

		RenderManager::getInstance().RenderWorld();

		glfwSwapBuffers(pWindow);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}