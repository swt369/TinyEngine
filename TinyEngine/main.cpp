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

	//Lights
	vector<glm::vec3> lightPositions({
		glm::vec3(0.0f, 0.5f, 1.5f),
		glm::vec3(-4.0f, 0.5f, -3.0f),
		glm::vec3(3.0f, 0.5f, 1.0f),
		glm::vec3(-0.8f, 2.4f, -1.0f),
		});
	vector<glm::vec3> lightColors({
		glm::vec3(5.0f, 5.0f, 5.0f),
		glm::vec3(10.0f, 0.0f, 0.0f),
		glm::vec3(0.0f, 0.0f, 15.0f),
		glm::vec3(0.0f, 5.0f, 0.0f),
		});
	vector<Object*> lightObjs;
	for (int i = 0; i < lightColors.size(); i++)
	{
		Object* lightObj = ObjectBuilder::CreateObject(lightPositions[i]);
		Light* light = lightObj->AddComponent<PointLight>();
		light->diffuse = lightColors[i];
		LightManager::getInstance().registerLight(light);
	}

	//LightCube
	Shader shaderLightCube("Shaders/lightCube.vs", "Shaders/lightCube.fs");
	Geometry* mesh = LoadManager::getInstance().LoadGeometryData("cube.mesh");
	vector<Material*> lightCubeMats;
	for (int i = 0; i < lightPositions.size(); i++)
	{
		Material* lightCubeMat = new Material(&shaderLightCube);
		lightCubeMat->setVec3("color", lightColors[i]);
		lightCubeMats.push_back(lightCubeMat);
		ObjectBuilder::CreateObject(mesh, lightCubeMat, 1000, lightPositions[i]);
	}

	//Floor
	Shader shader("Shaders/phong.vs", "Shaders/phong.fs");
	Texture woodTexture("../Resources/wood.png");
	Material woodMaterial(&shader);
	woodMaterial.setTexture("texture_diffuse_0", &woodTexture);
	ObjectBuilder::CreateObject(mesh, &woodMaterial, 1000, glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f), glm::vec3(12.5f, 0.5f, 12.5f));

	//Box
	Texture boxTexture("../Resources/container2.png");
	Material boxMaterial(&shader);
	boxMaterial.setTexture("texture_diffuse_0", &boxTexture);
	ObjectBuilder::CreateObject(mesh, &boxMaterial, 1000, glm::vec3(0.0f, 1.5f, 0.0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f));
	ObjectBuilder::CreateObject(mesh, &boxMaterial, 1000, glm::vec3(2.0f, 0.0f, 1.0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f));
	ObjectBuilder::CreateObject(mesh, &boxMaterial, 1000, glm::vec3(-1.0f, -1.0f, 2.0), glm::vec3(60.0f, 0.0f, 60.0f), glm::vec3(1.0f));
	ObjectBuilder::CreateObject(mesh, &boxMaterial, 1000, glm::vec3(0.0f, 2.7f, 4.0), glm::vec3(23.0f, 0.0f, 23.0f), glm::vec3(1.25f));
	ObjectBuilder::CreateObject(mesh, &boxMaterial, 1000, glm::vec3(-2.0f, 1.0f, -3.0), glm::vec3(124.0f, 0.0f, 124.0f), glm::vec3(1.0f));
	ObjectBuilder::CreateObject(mesh, &boxMaterial, 1000, glm::vec3(-3.0f, 0.0f, 0.0), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f));

	//Shader shader("Shaders/lightCube.vs", "Shaders/lightCube.fs");
	//Texture woodTexture("../Resources/wood.png");
	//Material material(&shader);
	//material.ambient = glm::vec3(0.0f);
	//material.diffuse = glm::vec3(1.0f);
	//material.specular = glm::vec3(0.0f);
	//material.setTexture("texture_diffuse_0", &woodTexture);
	//Geometry* mesh = LoadManager::getInstance().LoadGeometryData("cube.mesh");
	//ObjectBuilder::CreateObject(mesh, &material, 1000, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f), glm::vec3(2.5f, 2.5f, 27.5f));

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